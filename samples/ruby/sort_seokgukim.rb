# Sorting Algorithms sample code by Seokgukim

# Quick Sort Implementation (Non-recursive)
def quick_sort(arr, operator = :<)
  return arr if arr.length <= 1

  # Using stack to avoid recursion
  stack = []
  stack.push([0, arr.length - 1])

  while !stack.empty?
    s_idx, e_idx = stack.pop
    pivot = arr[(s_idx + e_idx) / 2]
    l, r = s_idx, e_idx

    while l <= r # Partitioning step
      l += 1 while arr[l].send(operator, pivot)
      r -= 1 while arr[r].send(operator, pivot) == false && arr[r] != pivot

      if l <= r # Swap elements
        arr[l], arr[r] = arr[r], arr[l]
        l += 1
        r -= 1
      end
    end
    # Push sub-arrays for next iteration
    stack.push([s_idx, r]) if s_idx < r
    stack.push([l, e_idx]) if l < e_idx
  end

  arr
end

# Dual pivot Quick Sort Implementation (Non-recursive)
def dual_pivot_quick_sort(arr, operator = :<)
  return arr if arr.length <= 1

  # Using stack to avoid recursion
  stack = []
  stack.push([0, arr.length - 1])

  while !stack.empty?
    s_idx, e_idx = stack.pop
    # Ensure arr[s_idx] <= arr[e_idx]
    if arr[s_idx].send(operator, arr[e_idx]) == false
      arr[s_idx], arr[e_idx] = arr[e_idx], arr[s_idx]
    end

    lp, rp = arr[s_idx], arr[e_idx]
    l, k, g = s_idx + 1, s_idx + 1, e_idx - 1

    while k <= g # Partitioning step
      if arr[k].send(operator, lp)
        arr[k], arr[l] = arr[l], arr[k]
        l += 1
      elsif arr[k].send(operator, rp) == false && arr[k] != rp
        g -= 1 while k < g && arr[g].send(operator, rp) == false && arr[g] != rp
        arr[k], arr[g] = arr[g], arr[k]
        g -= 1
        if arr[k].send(operator, lp)
          arr[k], arr[l] = arr[l], arr[k]
          l += 1
        end
      end
      k += 1
    end

    # Place pivots at their correct positions
    l -= 1
    g += 1
    arr[s_idx], arr[l] = arr[l], arr[s_idx]
    arr[e_idx], arr[g] = arr[g], arr[e_idx]

    # Push sub-arrays for next iteration
    stack.push([s_idx, l - 1]) if s_idx < l - 1
    stack.push([l + 1, g - 1]) if l + 1 < g - 1
    stack.push([g + 1, e_idx]) if g + 1 < e_idx
  end

  arr
end

# Merge Sort Implementation (Non-recursive)
def merge_sort(arr, operator = :<)
  return arr if arr.length <= 1

  n = arr.length
  temp = Array.new(n)

  size = 1
  while size < n # Do until the size covers the whole array
    left_start = 0 # Starting index of left subarray to be merged
    while left_start < n - 1 # Merge subarrays in bottom up manner
      mid = [left_start + size - 1, n - 1].min
      right_end = [left_start + 2 * size - 1, n - 1].min

      l, r, t = left_start, mid + 1, left_start

      while l <= mid && r <= right_end
        if arr[l].send(operator, arr[r])
          temp[t] = arr[l]
          l += 1
        else
          temp[t] = arr[r]
          r += 1
        end
        t += 1
      end

      while l <= mid
        temp[t] = arr[l]
        l += 1
        t += 1
      end

      while r <= right_end
        temp[t] = arr[r]
        r += 1
        t += 1
      end

      (left_start..right_end).each { |i| arr[i] = temp[i] }

      left_start += 2 * size # Move to the next pair of subarrays to merge
    end
    size *= 2 # Double the size for the next iteration
  end

  arr
end

# Insertion Sort Implementation
def insertion_sort(arr, operator = :<)
  return arr if arr.length <= 1

  (1...arr.length).each do |i|
    key = arr[i]
    j = i - 1

    # Move elements of arr[0..i-1], that are greater than key,
    # to one position ahead of their current position
    while j >= 0 && arr[j].send(operator, key) == false
      arr[j + 1] = arr[j]
      j -= 1
    end
    # Place key at after the element just smaller than it.
    arr[j + 1] = key
  end

  arr
end

# Bubble Sort Implementation
def bubble_sort(arr, operator = :<)
  n = arr.length
  return arr if n <= 1

  (0...n).each do |i|
    swapped = false
    (0...(n - i - 1)).each do |j|
      if arr[j].send(operator, arr[j + 1]) == false
        arr[j], arr[j + 1] = arr[j + 1], arr[j]
        swapped = true
      end
    end
    break unless swapped
  end

  arr
end

# Selection Sort Implementation
def selection_sort(arr, operator = :<)
  n = arr.length
  return arr if n <= 1

  (0...(n - 1)).each do |i|
    min_idx = i
    (i + 1...n).each do |j|
      if arr[j].send(operator, arr[min_idx])
        min_idx = j
      end
    end
    arr[i], arr[min_idx] = arr[min_idx], arr[i] if min_idx != i
  end

  arr
end

# Counting Sort Implementation
# Note: Only works for integers within a reasonable range
def counting_sort(arr, min_val = nil, max_val = nil)
  return arr if arr.empty?

  min_val = arr.min if min_val.nil?
  max_val = arr.max if max_val.nil?
  count = Array.new(max_val - min_val + 1, 0)
  ret = Array.new(arr.length)

  # Count the number and build the prefix sum array
  arr.each { |num| count[num - min_val] += 1 }
  (1...count.length).each { |i| count[i] += count[i - 1] }

  # Build the output array
  (arr.length - 1).downto(0) do |i|
    ret[count[arr[i] - min_val] - 1] = arr[i]
    count[arr[i] - min_val] -= 1
  end

  ret
end

# Run test cases, if this file is executed directly
if __FILE__ == $0
  # Get test arrays from stdin, if provided
  test_arrays = ARGF.each_line.map { |line| line.strip.split.map(&:to_i) }
  # Default test arrays if none provided
  test_arrays = [
    [64, 34, 25, 12, 22, 11, 90],
    [5, 1, 4, 2, 8],
    [38, 27, 43, 3, 9, 82, 10],
    [12, 11, 13, 5, 6],
    [29, 10, 14, 37, 13],
    [1, 2, 3, 4, 5],
    [5, 4, 3, 2, 1],
    [3, 3, 3, 3, 3],
    []
  ] if test_arrays.empty?

  sorting_algorithms = {
    "Quick Sort" => method(:quick_sort),
    "Dual Pivot Quick Sort" => method(:dual_pivot_quick_sort),
    "Merge Sort" => method(:merge_sort),
    "Insertion Sort" => method(:insertion_sort),
    "Bubble Sort" => method(:bubble_sort),
    "Selection Sort" => method(:selection_sort)
  }

  sorting_algorithms.each do |name, sort_method|
    puts "#{name}:"
    test_arrays.each do |arr|
      sorted_arr = sort_method.call(arr.dup)
      puts "Original: #{arr} => Sorted: #{sorted_arr}"
    end
    puts "-" * 40
  end
end
