#!/bin/bash

# --- Configuration ---
START_N=3
END_N=24
STEP_N=1
TRIALS=5
MAX_VAL=1000
# ---------------------

SAVE_FILES=false

# Parse arguments: only -f is supported to enable file saving
while getopts "f" opt; do
    case $opt in
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
    
    # Initialize sums
    sum_random_bt=0
    sum_random_dp=0
    sum_divisible_bt=0
    sum_divisible_dp=0
    sum_nearmiss_bt=0
    sum_nearmiss_dp=0
    sum_yes_bt=0
    sum_yes_dp=0

    for (( i=1; i<=$TRIALS; i++ )); do
        echo "  Trial $i/$TRIALS..."

        # --- 1. Random Case ---
        # echo "    [Random Case]"
        python3 generator.py $n $MAX_VAL 0 > temp_input.txt
        ./build/Test < temp_input.txt > temp_output.txt
        
        # Extract times
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        
        sum_random_bt=$(awk "BEGIN {print $sum_random_bt + $time_bt}")
        sum_random_dp=$(awk "BEGIN {print $sum_random_dp + $time_dp}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../testcases/input_${n}_${i}_random.in"
            cp temp_output.txt "../testcases/output_${n}_${i}_random.out"
        fi

        # --- 2. Random Divisible Case ---
        # echo "    [Random Divisible Case]"
        python3 generator.py $n $MAX_VAL 2 > temp_input.txt
        ./build/Test < temp_input.txt > temp_output.txt
        
        # Extract times
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        
        sum_divisible_bt=$(awk "BEGIN {print $sum_divisible_bt + $time_bt}")
        sum_divisible_dp=$(awk "BEGIN {print $sum_divisible_dp + $time_dp}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../testcases/input_${n}_${i}_divisible.in"
            cp temp_output.txt "../testcases/output_${n}_${i}_divisible.out"
        fi

        # --- 3. Near Miss Case ---
        # echo "    [Near Miss Case]"
        python3 generator.py $n $MAX_VAL 3 > temp_input.txt
        ./build/Test < temp_input.txt > temp_output.txt
        
        # Extract times
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        
        sum_nearmiss_bt=$(awk "BEGIN {print $sum_nearmiss_bt + $time_bt}")
        sum_nearmiss_dp=$(awk "BEGIN {print $sum_nearmiss_dp + $time_dp}")
        
        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../testcases/input_${n}_${i}_nearmiss.in"
            cp temp_output.txt "../testcases/output_${n}_${i}_nearmiss.out"
        fi

        # --- 4. Yes Case ---
        # echo "    [Yes Case]"
        python3 generator.py $n $MAX_VAL 1 > temp_input.txt
        ./build/Test < temp_input.txt > temp_output.txt
        
        # Extract times
        times=($(grep "Time:" temp_output.txt | awk '{print $2}'))
        time_bt=${times[0]}
        time_dp=${times[1]}
        
        sum_yes_bt=$(awk "BEGIN {print $sum_yes_bt + $time_bt}")
        sum_yes_dp=$(awk "BEGIN {print $sum_yes_dp + $time_dp}")

        if [ "$SAVE_FILES" = true ]; then
            cp temp_input.txt "../testcases/input_${n}_${i}_yes.in"
            cp temp_output.txt "../testcases/output_${n}_${i}_yes.out"
        fi
    done

    # Calculate averages
    avg_random_bt=$(awk "BEGIN {print $sum_random_bt / $TRIALS}")
    avg_random_dp=$(awk "BEGIN {print $sum_random_dp / $TRIALS}")
    avg_divisible_bt=$(awk "BEGIN {print $sum_divisible_bt / $TRIALS}")
    avg_divisible_dp=$(awk "BEGIN {print $sum_divisible_dp / $TRIALS}")
    avg_nearmiss_bt=$(awk "BEGIN {print $sum_nearmiss_bt / $TRIALS}")
    avg_nearmiss_dp=$(awk "BEGIN {print $sum_nearmiss_dp / $TRIALS}")
    avg_yes_bt=$(awk "BEGIN {print $sum_yes_bt / $TRIALS}")
    avg_yes_dp=$(awk "BEGIN {print $sum_yes_dp / $TRIALS}")

    # Save to CSV
    echo "$n,Random,Backtracking,$avg_random_bt" >> $RESULTS_FILE
    echo "$n,Random,BitmaskDP,$avg_random_dp" >> $RESULTS_FILE
    echo "$n,RandomDivisible,Backtracking,$avg_divisible_bt" >> $RESULTS_FILE
    echo "$n,RandomDivisible,BitmaskDP,$avg_divisible_dp" >> $RESULTS_FILE
    echo "$n,NearMiss,Backtracking,$avg_nearmiss_bt" >> $RESULTS_FILE
    echo "$n,NearMiss,BitmaskDP,$avg_nearmiss_dp" >> $RESULTS_FILE
    echo "$n,Yes,Backtracking,$avg_yes_bt" >> $RESULTS_FILE
    echo "$n,Yes,BitmaskDP,$avg_yes_dp" >> $RESULTS_FILE

    # Print summary
    echo "  [Average Results]"
    echo "    Random:          BT=${avg_random_bt}s, DP=${avg_random_dp}s"
    echo "    RandomDivisible: BT=${avg_divisible_bt}s, DP=${avg_divisible_dp}s"
    echo "    NearMiss:        BT=${avg_nearmiss_bt}s, DP=${avg_nearmiss_dp}s"
    echo "    Yes:             BT=${avg_yes_bt}s, DP=${avg_yes_dp}s"
done

# Cleanup
rm -f temp_input.txt temp_output.txt

# Generate plots
echo "Generating plots..."
python3 plot_results.py

echo "--------------------------------------------------"
echo "Done. Results saved to $RESULTS_FILE and plots to ../documents/"
