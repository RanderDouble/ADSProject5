#!/bin/bash

# --- 配置 ---
FIXED_N=120
START_K=38
END_K=41
STEP_K=2
TRIALS=50
MAX_VAL=100

# 停止限制 (K > limit 将跳过该算法)
STOP_BT=100
STOP_MASK=1
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
RESULTS_FILE="results_k.csv"
# echo "K,Type,Algorithm,AvgTime" > $RESULTS_FILE

echo "开始测试 (固定 N=$FIXED_N, 变化 K)..."
echo "范围: K=$START_K - $END_K, 步长: $STEP_K, 次数: $TRIALS, 最大值: $MAX_VAL"

for (( k=$START_K; k<=$END_K; k+=$STEP_K )); do
    # 检查 K 是否大于 N
    if [ $k -gt $FIXED_N ]; then
        echo "跳过: K ($k) 不能大于 N ($FIXED_N)"
        continue
    fi

    echo "--------------------------------------------------"
    echo "测试 K = $k (N = $FIXED_N)"
    
    # 确定运行哪些算法
    RUN_BT=1
    if [ $k -gt $STOP_BT ]; then RUN_BT=0; fi
    
    RUN_MASK=1
    if [ $k -gt $STOP_MASK ]; then RUN_MASK=0; fi
    
    # DimenDP 仅支持 K=3，因此在此通用 K 测试中禁用它
    RUN_DIMEN=0

    echo "  激活状态: BT=$RUN_BT, Mask=$RUN_MASK, Dimen=$RUN_DIMEN"
    
    # 初始化总和
    sum_divisible_bt=0
    sum_divisible_dp=0
    
    sum_nearmiss_bt=0
    sum_nearmiss_dp=0

    sum_yes_bt=0
    sum_yes_dp=0

    for (( i=1; i<=$TRIALS; i++ )); do
        echo "  第 $i/$TRIALS 次试验..."

        # --- 1. 随机可整除情况 ---
        python3 ./generator.py $FIXED_N $MAX_VAL 2 $k > temp_input.txt
        ../build/Test $RUN_BT $RUN_MASK $RUN_DIMEN $k < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        
        sum_divisible_bt=$(awk "BEGIN {print $sum_divisible_bt + $time_bt}")
        sum_divisible_dp=$(awk "BEGIN {print $sum_divisible_dp + $time_dp}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../../testcases/input_k${k}_${i}_divisible.in"
            cp temp_output.txt "../../testcases/output_k${k}_${i}_divisible.out"
        fi

        # --- 2. 差一点情况 (Near Miss) ---
        python3 ./generator.py $FIXED_N $MAX_VAL 3 $k > temp_input.txt
        ../build/Test $RUN_BT $RUN_MASK $RUN_DIMEN $k < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        
        sum_nearmiss_bt=$(awk "BEGIN {print $sum_nearmiss_bt + $time_bt}")
        sum_nearmiss_dp=$(awk "BEGIN {print $sum_nearmiss_dp + $time_dp}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../../testcases/input_k${k}_${i}_nearmiss.in"
            cp temp_output.txt "../../testcases/output_k${k}_${i}_nearmiss.out"
        fi

        # --- 3. 肯定有解情况 (Yes Case) ---
        python3 ./generator.py $FIXED_N $MAX_VAL 1 $k > temp_input.txt
        ../build/Test $RUN_BT $RUN_MASK $RUN_DIMEN $k < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        
        sum_yes_bt=$(awk "BEGIN {print $sum_yes_bt + $time_bt}")
        sum_yes_dp=$(awk "BEGIN {print $sum_yes_dp + $time_dp}")

        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../../testcases/input_k${k}_${i}_yes.in"
            cp temp_output.txt "../../testcases/output_k${k}_${i}_yes.out"
        fi
    done

    # 计算平均值
    avg_divisible_bt=$(awk "BEGIN {print $sum_divisible_bt / $TRIALS}")
    avg_divisible_dp=$(awk "BEGIN {print $sum_divisible_dp / $TRIALS}")
    
    avg_nearmiss_bt=$(awk "BEGIN {print $sum_nearmiss_bt / $TRIALS}")
    avg_nearmiss_dp=$(awk "BEGIN {print $sum_nearmiss_dp / $TRIALS}")

    avg_yes_bt=$(awk "BEGIN {print $sum_yes_bt / $TRIALS}")
    avg_yes_dp=$(awk "BEGIN {print $sum_yes_dp / $TRIALS}")

    # 保存到 CSV
    echo "$k,RandomDivisible,Backtracking,$avg_divisible_bt" >> $RESULTS_FILE
    echo "$k,RandomDivisible,BitmaskDP,$avg_divisible_dp" >> $RESULTS_FILE
    
    echo "$k,NearMiss,Backtracking,$avg_nearmiss_bt" >> $RESULTS_FILE
    echo "$k,NearMiss,BitmaskDP,$avg_nearmiss_dp" >> $RESULTS_FILE

    echo "$k,Yes,Backtracking,$avg_yes_bt" >> $RESULTS_FILE
    echo "$k,Yes,BitmaskDP,$avg_yes_dp" >> $RESULTS_FILE

    # 打印摘要
    echo "  [平均结果]"
    echo "    随机可整除: BT=${avg_divisible_bt}s, DP=${avg_divisible_dp}s"
    echo "    差一点:     BT=${avg_nearmiss_bt}s, DP=${avg_nearmiss_dp}s"
    echo "    肯定有解:   BT=${avg_yes_bt}s, DP=${avg_yes_dp}s"
done

# 清理
rm -f temp_input.txt temp_output.txt

# 生成图表
echo "正在生成图表..."
python3 plot_k.py

echo "--------------------------------------------------"
echo "完成。结果已保存至 $RESULTS_FILE，图表已保存至 ../../documents/"
