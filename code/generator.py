import sys
import random

def generate_random_case(n, max_val):
    numbers = [random.randint(1, max_val) for _ in range(n)]
    return numbers

def generate_random_divisible_case(n, max_val):
    numbers = [random.randint(1, max_val) for _ in range(n)]
    total_sum = sum(numbers)
    remainder = total_sum % 3
    
    if remainder != 0:
        # Adjust one number to make sum divisible by 3
        # We want (total_sum - numbers[i] + new_val) % 3 == 0
        # => new_val % 3 == (numbers[i] - total_sum) % 3
        # Simplest way: subtract remainder from a number.
        # If number becomes <= 0, add 3.
        
        idx = random.randint(0, n - 1)
        numbers[idx] -= remainder
        while numbers[idx] < 1:
            numbers[idx] += 3
            
    return numbers

def generate_yes_case(n, max_val):
    if n < 3: n = 3
    
    low_bound = n
    high_bound = n * max_val
    if low_bound > high_bound: low_bound = high_bound
    
    target_sum = random.randint(low_bound, high_bound) // 3
    max_bucket_size = (n + 2) // 3
    if target_sum < max_bucket_size:
        target_sum = max_bucket_size

    all_numbers = []
    
    for i in range(3):
        current_sum = 0
        count = n // 3 
        if i == 2: count = n - len(all_numbers)
        
        bucket_nums = []
        for k in range(count - 1):
            nums_left_after = count - 1 - k
            upper_bound = target_sum - current_sum - nums_left_after
            limit = min(max_val, upper_bound)
            if limit < 1: limit = 1
            
            val = random.randint(1, limit)
            bucket_nums.append(val)
            current_sum += val
            
        last_val = target_sum - current_sum
        if last_val < 1: last_val = 1 
        
        bucket_nums.append(last_val)
        all_numbers.extend(bucket_nums)
        
    random.shuffle(all_numbers)
    return all_numbers

def generate_near_miss_case(n, max_val):
    # Start with a valid Yes case
    nums = generate_yes_case(n, max_val)
    
    # Perturb it: +1 to one, -1 to another to keep sum constant
    # but break the specific constructed solution
    idx1, idx2 = random.sample(range(n), 2)
    
    # Ensure we don't create 0 or negative numbers
    if nums[idx2] > 1:
        nums[idx1] += 1
        nums[idx2] -= 1
    else:
        nums[idx1] -= 1
        nums[idx2] += 1
        
    return nums

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 generator.py <N> <MaxVal> [Mode: 0=Random, 1=Yes, 2=Divisible, 3=NearMiss]", file=sys.stderr)
        sys.exit(1)

    n = int(sys.argv[1])
    max_val = int(sys.argv[2])
    mode = int(sys.argv[3]) if len(sys.argv) > 3 else 0

    if mode == 1:
        nums = generate_yes_case(n, max_val)
    elif mode == 2:
        nums = generate_random_divisible_case(n, max_val)
    elif mode == 3:
        nums = generate_near_miss_case(n, max_val)
    else:
        nums = generate_random_case(n, max_val)

    print(n)
    print(" ".join(map(str, nums)))
