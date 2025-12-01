#!/bin/bash

# --- 配置 ---
START_N=3
END_N=120
STEP_N=1
TRIALS=1
MAX_VAL=10

# 停止限制 (N > limit 将跳过该算法)
STOP_BT=120
STOP_MASK=30
STOP_DIMEN=60
# ---------------------

SAVE_FILES=false

# 解析参数: 支持 -f 以启用文件保存
while getopts "f" opt; do
    case "$opt" in
        f) SAVE_FILES=true ;;
        *) echo "用法: $0 [-f] (保存文件)" >&2; exit 1 ;;
    esac
done

# 构建项目
echo "正在构建项目..."
if [ ! -d "../build" ]; then
    mkdir ../build
fi
cd ../build
cmake .. > /dev/null
make > /dev/null
if [ $? -ne 0 ]; then
    echo "构建失败!"
    exit 1
fi
cd ../test

# 如果需要，创建测试用例目录
if [ "$SAVE_FILES" = true ]; then
    mkdir -p ../../testcases
fi

# 初始化结果文件
RESULTS_FILE="results_n.csv"
echo "N,Type,Algorithm,AvgTime" > $RESULTS_FILE

echo "开始测试 (变化 N, 固定 K=3)..."
echo "范围: $START_N - $END_N, 步长: $STEP_N, 次数: $TRIALS, 最大值: $MAX_VAL"

for (( n=$START_N; n<=$END_N; n+=$STEP_N )); do
    echo "--------------------------------------------------"
    echo "测试 N = $n"
    
    # 确定运行哪些算法
    RUN_BT=1
    if [ $n -gt $STOP_BT ]; then RUN_BT=0; fi
    
    RUN_MASK=1
    if [ $n -gt $STOP_MASK ]; then RUN_MASK=0; fi
    
    RUN_DIMEN=1
    if [ $n -gt $STOP_DIMEN ]; then RUN_DIMEN=0; fi

    echo "  激活状态: BT=$RUN_BT, Mask=$RUN_MASK, Dimen=$RUN_DIMEN"
    
    # 初始化总和
    sum_divisible_bt=0
    sum_divisible_dp=0
    sum_divisible_dimen=0
    
    sum_nearmiss_bt=0
    sum_nearmiss_dp=0
    sum_nearmiss_dimen=0
    
    sum_yes_bt=0
    sum_yes_dp=0
    sum_yes_dimen=0

    for (( i=1; i<=$TRIALS; i++ )); do
        echo "  第 $i/$TRIALS 次试验..."

        # --- 1. 随机可整除情况 ---
        python3 ./generator.py $n $MAX_VAL 2 3 > temp_input.txt
        ../build/Test $RUN_BT $RUN_MASK $RUN_DIMEN 3 < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        time_dimen=${times[2]}
        
        sum_divisible_bt=$(awk "BEGIN {print $sum_divisible_bt + $time_bt}")
        sum_divisible_dp=$(awk "BEGIN {print $sum_divisible_dp + $time_dp}")
        sum_divisible_dimen=$(awk "BEGIN {print $sum_divisible_dimen + $time_dimen}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../../testcases/input_${n}_${i}_divisible.in"
            cp temp_output.txt "../../testcases/output_${n}_${i}_divisible.out"
        fi

        # --- 2. 差一点情况 (Near Miss) ---
        python3 ./generator.py $n $MAX_VAL 3 3 > temp_input.txt
        ../build/Test $RUN_BT $RUN_MASK $RUN_DIMEN 3 < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        time_dimen=${times[2]}
        
        sum_nearmiss_bt=$(awk "BEGIN {print $sum_nearmiss_bt + $time_bt}")
        sum_nearmiss_dp=$(awk "BEGIN {print $sum_nearmiss_dp + $time_dp}")
        sum_nearmiss_dimen=$(awk "BEGIN {print $sum_nearmiss_dimen + $time_dimen}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../../testcases/input_${n}_${i}_nearmiss.in"
            cp temp_output.txt "../../testcases/output_${n}_${i}_nearmiss.out"
        fi

        # --- 3. 肯定有解情况 (Yes Case) ---
        python3 ./generator.py $n $MAX_VAL 1 3 > temp_input.txt
        ../build/Test $RUN_BT $RUN_MASK $RUN_DIMEN 3 < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        time_dimen=${times[2]}
        
        sum_yes_bt=$(awk "BEGIN {print $sum_yes_bt + $time_bt}")
        sum_yes_dp=$(awk "BEGIN {print $sum_yes_dp + $time_dp}")
        sum_yes_dimen=$(awk "BEGIN {print $sum_yes_dimen + $time_dimen}")

        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../../testcases/input_${n}_${i}_yes.in"
            cp temp_output.txt "../../testcases/output_${n}_${i}_yes.out"
        fi
    done

    # 计算平均值
    avg_divisible_bt=$(awk "BEGIN {print $sum_divisible_bt / $TRIALS}")
    avg_divisible_dp=$(awk "BEGIN {print $sum_divisible_dp / $TRIALS}")
    avg_divisible_dimen=$(awk "BEGIN {print $sum_divisible_dimen / $TRIALS}")
    
    avg_nearmiss_bt=$(awk "BEGIN {print $sum_nearmiss_bt / $TRIALS}")
    avg_nearmiss_dp=$(awk "BEGIN {print $sum_nearmiss_dp / $TRIALS}")
    avg_nearmiss_dimen=$(awk "BEGIN {print $sum_nearmiss_dimen / $TRIALS}")
    
    avg_yes_bt=$(awk "BEGIN {print $sum_yes_bt / $TRIALS}")
    avg_yes_dp=$(awk "BEGIN {print $sum_yes_dp / $TRIALS}")
    avg_yes_dimen=$(awk "BEGIN {print $sum_yes_dimen / $TRIALS}")

    # 保存到 CSV
    echo "$n,RandomDivisible,Backtracking,$avg_divisible_bt" >> $RESULTS_FILE
    echo "$n,RandomDivisible,BitmaskDP,$avg_divisible_dp" >> $RESULTS_FILE
    echo "$n,RandomDivisible,DimenDP,$avg_divisible_dimen" >> $RESULTS_FILE
    
    echo "$n,NearMiss,Backtracking,$avg_nearmiss_bt" >> $RESULTS_FILE
    echo "$n,NearMiss,BitmaskDP,$avg_nearmiss_dp" >> $RESULTS_FILE
    echo "$n,NearMiss,DimenDP,$avg_nearmiss_dimen" >> $RESULTS_FILE
    
    echo "$n,Yes,Backtracking,$avg_yes_bt" >> $RESULTS_FILE
    echo "$n,Yes,BitmaskDP,$avg_yes_dp" >> $RESULTS_FILE
    echo "$n,Yes,DimenDP,$avg_yes_dimen" >> $RESULTS_FILE

    # 打印摘要
    echo "  [平均结果]"
    echo "    随机可整除: BT=${avg_divisible_bt}s, DP=${avg_divisible_dp}s, Dimen=${avg_divisible_dimen}s"
    echo "    差一点:     BT=${avg_nearmiss_bt}s, DP=${avg_nearmiss_dp}s, Dimen=${avg_nearmiss_dimen}s"
    echo "    肯定有解:   BT=${avg_yes_bt}s, DP=${avg_yes_dp}s, Dimen=${avg_yes_dimen}s"
done

# 清理
rm -f temp_input.txt temp_output.txt

# 生成图表
echo "正在生成图表..."
python3 plot_n.py

echo "--------------------------------------------------"
echo "完成。结果已保存至 $RESULTS_FILE，图表已保存至 ../../documents/"
