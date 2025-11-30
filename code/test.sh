#!/bin/bash

# --- Configuration ---
START_N=3
END_N=100
STEP_N=1
TRIALS=10
MAX_VAL=1000

# Stop limits (N > limit will skip the algorithm)
STOP_BT=100
STOP_MASK=50
STOP_DIMEN=25
# ---------------------

SAVE_FILES=false

# Parse arguments: only -f is supported to enable file saving
while getopts "f" opt; do
    case "$opt" in
        f) SAVE_FILES=true ;;
        *) echo "Usage: $0 [-f] (to save files)" >&2; exit 1 ;;
    esac
done

# Build project
echo "Building project..."
if [ ! -d "build" ]; then
    mkdir build
fi
cd build
cmake .. > /dev/null
make > /dev/null
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi
cd ..

# Create testcases directory if needed
if [ "$SAVE_FILES" = true ]; then
    mkdir -p ../testcases
fi

# Initialize results file
RESULTS_FILE="results.csv"
echo "N,Type,Algorithm,AvgTime" > $RESULTS_FILE

echo "Starting tests..."
echo "Range: $START_N - $END_N, Step: $STEP_N, Trials: $TRIALS, MaxVal: $MAX_VAL"

for (( n=$START_N; n<=$END_N; n+=$STEP_N )); do
    echo "--------------------------------------------------"
    echo "Testing N = $n"
    
    # Determine which algorithms to run
    RUN_BT=1
    if [ $n -gt $STOP_BT ]; then RUN_BT=0; fi
    
    RUN_MASK=1
    if [ $n -gt $STOP_MASK ]; then RUN_MASK=0; fi
    
    RUN_DIMEN=1
    if [ $n -gt $STOP_DIMEN ]; then RUN_DIMEN=0; fi

    echo "  Active: BT=$RUN_BT, Mask=$RUN_MASK, Dimen=$RUN_DIMEN"
    
    # Initialize sums
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
        echo "  Trial $i/$TRIALS..."

        # --- 1. Random Divisible Case ---
        python3 generator.py $n $MAX_VAL 2 > temp_input.txt
        ./build/Test $RUN_BT $RUN_MASK $RUN_DIMEN < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        time_dimen=${times[2]}
        
        sum_divisible_bt=$(awk "BEGIN {print $sum_divisible_bt + $time_bt}")
        sum_divisible_dp=$(awk "BEGIN {print $sum_divisible_dp + $time_dp}")
        sum_divisible_dimen=$(awk "BEGIN {print $sum_divisible_dimen + $time_dimen}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../testcases/input_${n}_${i}_divisible.in"
            cp temp_output.txt "../testcases/output_${n}_${i}_divisible.out"
        fi

        # --- 2. Near Miss Case ---
        python3 generator.py $n $MAX_VAL 3 > temp_input.txt
        ./build/Test $RUN_BT $RUN_MASK $RUN_DIMEN < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        time_dimen=${times[2]}
        
        sum_nearmiss_bt=$(awk "BEGIN {print $sum_nearmiss_bt + $time_bt}")
        sum_nearmiss_dp=$(awk "BEGIN {print $sum_nearmiss_dp + $time_dp}")
        sum_nearmiss_dimen=$(awk "BEGIN {print $sum_nearmiss_dimen + $time_dimen}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../testcases/input_${n}_${i}_nearmiss.in"
            cp temp_output.txt "../testcases/output_${n}_${i}_nearmiss.out"
        fi

        # --- 3. Yes Case ---
        python3 generator.py $n $MAX_VAL 1 > temp_input.txt
        ./build/Test $RUN_BT $RUN_MASK $RUN_DIMEN < temp_input.txt > temp_output.txt
        
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        time_dimen=${times[2]}
        
        sum_yes_bt=$(awk "BEGIN {print $sum_yes_bt + $time_bt}")
        sum_yes_dp=$(awk "BEGIN {print $sum_yes_dp + $time_dp}")
        sum_yes_dimen=$(awk "BEGIN {print $sum_yes_dimen + $time_dimen}")

        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../testcases/input_${n}_${i}_yes.in"
            cp temp_output.txt "../testcases/output_${n}_${i}_yes.out"
        fi
    done

    # Calculate averages
    avg_divisible_bt=$(awk "BEGIN {print $sum_divisible_bt / $TRIALS}")
    avg_divisible_dp=$(awk "BEGIN {print $sum_divisible_dp / $TRIALS}")
    avg_divisible_dimen=$(awk "BEGIN {print $sum_divisible_dimen / $TRIALS}")
    
    avg_nearmiss_bt=$(awk "BEGIN {print $sum_nearmiss_bt / $TRIALS}")
    avg_nearmiss_dp=$(awk "BEGIN {print $sum_nearmiss_dp / $TRIALS}")
    avg_nearmiss_dimen=$(awk "BEGIN {print $sum_nearmiss_dimen / $TRIALS}")
    
    avg_yes_bt=$(awk "BEGIN {print $sum_yes_bt / $TRIALS}")
    avg_yes_dp=$(awk "BEGIN {print $sum_yes_dp / $TRIALS}")
    avg_yes_dimen=$(awk "BEGIN {print $sum_yes_dimen / $TRIALS}")

    # Save to CSV
    echo "$n,RandomDivisible,Backtracking,$avg_divisible_bt" >> $RESULTS_FILE
    echo "$n,RandomDivisible,BitmaskDP,$avg_divisible_dp" >> $RESULTS_FILE
    echo "$n,RandomDivisible,DimenDP,$avg_divisible_dimen" >> $RESULTS_FILE
    
    echo "$n,NearMiss,Backtracking,$avg_nearmiss_bt" >> $RESULTS_FILE
    echo "$n,NearMiss,BitmaskDP,$avg_nearmiss_dp" >> $RESULTS_FILE
    echo "$n,NearMiss,DimenDP,$avg_nearmiss_dimen" >> $RESULTS_FILE
    
    echo "$n,Yes,Backtracking,$avg_yes_bt" >> $RESULTS_FILE
    echo "$n,Yes,BitmaskDP,$avg_yes_dp" >> $RESULTS_FILE
    echo "$n,Yes,DimenDP,$avg_yes_dimen" >> $RESULTS_FILE

    # Print summary
    echo "  [Average Results]"
    echo "    RandomDivisible: BT=${avg_divisible_bt}s, DP=${avg_divisible_dp}s, Dimen=${avg_divisible_dimen}s"
    echo "    NearMiss:        BT=${avg_nearmiss_bt}s, DP=${avg_nearmiss_dp}s, Dimen=${avg_nearmiss_dimen}s"
    echo "    Yes:             BT=${avg_yes_bt}s, DP=${avg_yes_dp}s, Dimen=${avg_yes_dimen}s"
done

# Cleanup
rm -f temp_input.txt temp_output.txt

# Generate plots
echo "Generating plots..."
python3 plot_results.py

echo "--------------------------------------------------"
echo "Done. Results saved to $RESULTS_FILE and plots to ../documents/"
