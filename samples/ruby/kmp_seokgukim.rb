# Knuth-Morris-Pratt (KMP) string matching algorithm implementation

def kmp_make_table(pattern)
  # Make the KMP failure table
  m = pattern.length
  table = Array.new(m, 0)
  j = 0

  (1...m).each do |i|
    while j > 0 && pattern[i] != pattern[j]
      j = table[j - 1]
    end
    if pattern[i] == pattern[j]
      j += 1
      table[i] = j
    end
  end

  table
end

def kmp_search(source, pattern)
  # KMP search algorithm
  n = source.length
  m = pattern.length
  table = kmp_make_table(pattern)
  j = 0
  occurrences = []

  (0...n).each do |i|
    while j > 0 && source[i] != pattern[j]
      j = table[j - 1]
    end
    if text[i] == pattern[j]
      if j == m - 1
        occurrences << (i - m + 1) # Match found, store the starting index
        j = table[j]
      else
        j += 1
      end
    end
  end

  occurrences
end
