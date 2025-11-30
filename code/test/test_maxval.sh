#!/bin/bash

# --- 配置 ---
N=20
START_MAX_VAL=100
END_MAX_VAL=2000
STEP_MAX_VAL=100
TRIALS=1
# ---------------------

SAVE_FILES=false

# 解析参数: 仅支持 -f 以启用文件保存
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

# 输出文件
OUTPUT_FILE="results_maxval.csv"
echo "MaxVal,Type,Algorithm,AvgTime" > $OUTPUT_FILE

echo "开始测试 (固定 N=$N, 变化 MaxVal)..."

for (( max_val=$START_MAX_VAL; max_val<=$END_MAX_VAL; max_val+=$STEP_MAX_VAL )); do
    echo "--------------------------------------------------"
    echo "测试 MaxVal = $max_val"
    
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
        # --- 1. 随机可整除情况 ---
        python3 ./generator.py $N $max_val 2 3 > temp_input.txt
        ../build/Test 1 1 1 3 < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        time_dimen=${times[2]}
        
        sum_divisible_bt=$(awk "BEGIN {print $sum_divisible_bt + $time_bt}")
        sum_divisible_dp=$(awk "BEGIN {print $sum_divisible_dp + $time_dp}")
        sum_divisible_dimen=$(awk "BEGIN {print $sum_divisible_dimen + $time_dimen}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../../testcases/input_maxval_${max_val}_${i}_divisible.in"
            cp temp_output.txt "../../testcases/output_maxval_${max_val}_${i}_divisible.out"
        fi

        # --- 2. 差一点情况 (Near Miss) ---
        python3 ./generator.py $N $max_val 3 3 > temp_input.txt
        ../build/Test 1 1 1 3 < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        time_dimen=${times[2]}
        
        sum_nearmiss_bt=$(awk "BEGIN {print $sum_nearmiss_bt + $time_bt}")
        sum_nearmiss_dp=$(awk "BEGIN {print $sum_nearmiss_dp + $time_dp}")
        sum_nearmiss_dimen=$(awk "BEGIN {print $sum_nearmiss_dimen + $time_dimen}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../../testcases/input_maxval_${max_val}_${i}_nearmiss.in"
            cp temp_output.txt "../../testcases/output_maxval_${max_val}_${i}_nearmiss.out"
        fi

        # --- 3. 肯定有解情况 (Yes Case) ---
        python3 ./generator.py $N $max_val 1 3 > temp_input.txt
        ../build/Test 1 1 1 3 < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        time_dimen=${times[2]}
        
        sum_yes_bt=$(awk "BEGIN {print $sum_yes_bt + $time_bt}")
        sum_yes_dp=$(awk "BEGIN {print $sum_yes_dp + $time_dp}")
        sum_yes_dimen=$(awk "BEGIN {print $sum_yes_dimen + $time_dimen}")

        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../../testcases/input_maxval_${max_val}_${i}_yes.in"
            cp temp_output.txt "../../testcases/output_maxval_${max_val}_${i}_yes.out"
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
    echo "$max_val,RandomDivisible,Backtracking,$avg_divisible_bt" >> $OUTPUT_FILE
    echo "$max_val,RandomDivisible,BitmaskDP,$avg_divisible_dp" >> $OUTPUT_FILE
    echo "$max_val,RandomDivisible,DimenDP,$avg_divisible_dimen" >> $OUTPUT_FILE
    
    echo "$max_val,NearMiss,Backtracking,$avg_nearmiss_bt" >> $OUTPUT_FILE
    echo "$max_val,NearMiss,BitmaskDP,$avg_nearmiss_dp" >> $OUTPUT_FILE
    echo "$max_val,NearMiss,DimenDP,$avg_nearmiss_dimen" >> $OUTPUT_FILE
    
    echo "$max_val,Yes,Backtracking,$avg_yes_bt" >> $OUTPUT_FILE
    echo "$max_val,Yes,BitmaskDP,$avg_yes_dp" >> $OUTPUT_FILE
    echo "$max_val,Yes,DimenDP,$avg_yes_dimen" >> $OUTPUT_FILE

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
python3 plot_maxval.py

echo "--------------------------------------------------"
echo "完成。结果已保存至 $OUTPUT_FILE，图表已保存至 ../../documents/"
