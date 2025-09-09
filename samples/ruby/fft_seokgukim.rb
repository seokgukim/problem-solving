# Fast Fourier Transform implementation in Ruby
class FFT
  attr_reader :data

  def initialize(data)
    @data = data
    # Pad data to the next power of two
    next_pow2 = 1 << (Math.log2(data.length).ceil)
    @data += [0] * (next_pow2 - data.length) if data.length != next_pow2
  end

  def fft(iterative=false)
    iterative ? iterative_fft : recursive_fft
  end

  def ifft(iterative=false)
    n = @data.length
    conjugated = @data.map(&:conj)
    fft_instance = FFT.new(conjugated)
    transformed = fft_instance.fft(iterative)
    transformed.map { |x| ((x.real + 0.01) / n).to_i }
  end

  def *(other)
    raise "You can only multiply with another FFT instance" unless other.is_a?(FFT)
    # Pad both to the next power of two of their combined length
    new_length = 1 << (Math.log2(@data.length + other.data.length - 1).ceil)
    @data += [0] * (new_length - @data.length) if @data.length != new_length
    other_data = other.data + [0] * (new_length - other.data.length) if other.data.length != new_length

    # Perform FFT on both
    fft1 = FFT.new(@data).fft(true)
    fft2 = FFT.new(other_data).fft(true)

    # Point-wise multiply
    convolved = Array.new(new_length) { |i| fft1[i] * fft2[i] }
    FFT.new(convolved).ifft(true)
  end

  private

  # Cooley-Tukey FFT algorithm
  def recursive_fft
    n = @data.length
    return @data if n <= 1
  
    # Divide and conquer
    even = FFT.new(@data.each_slice(2).map(&:first)).fft()
    odd = FFT.new(@data.each_slice(2).map(&:last)).fft()

    combined = Array.new(n)
    (0...n / 2).each do |k|
      t = Math::E**(-2.0i * Math::PI * k / n) * odd[k] # Twiddle factor
      combined[k] = even[k] + t
      combined[k + n / 2] = even[k] - t
    end
    combined
  end

  # Helper function to compute bit-reversed indices
  # For example, for n=8 (3 bits), indices 0-7 map to 0,4,2,6,1,5,3,7
  def bit_reverse_indices(n)
    indices = (0...n).to_a
    bits = Math.log2(n).to_i
    indices.map do |i|
      rev = 0
      bits.times do |j|
        rev = (rev << 1) | ((i >> j) & 1)
      end
      rev
    end
  end
  # Reorder data based on bit-reversed indices
  # if original data = [a0, a1, a2, a3, a4, a5, a6, a7]
  # then the reordered = [a0, a4, a2, a6, a1, a5, a3, a7]
  def reorder_data
    n = @data.length
    indices = bit_reverse_indices(n)
    reordered = Array.new(n)
    indices.each_with_index do |new_idx, old_idx|
      reordered[new_idx] = @data[old_idx]
    end
    reordered
  end

  # Iterative FFT implementation
  def iterative_fft
    @data = reorder_data # Bit-reversal permutation
    n = @data.length
    m = Math.log2(n).to_i
    (1..m).each do |s|
      m_val = 1 << s
      w_m = Math::E**(-2.0i * Math::PI / m_val)
      (0...n).step(m_val) do |k|
        w = 1
        (0...(m_val / 2)).each do |j|
          t = w * @data[k + j + m_val / 2]
          u = @data[k + j]
          @data[k + j] = u + t
          @data[k + j + m_val / 2] = u - t
          w *= w_m
        end
      end
    end
    @data
  end
end

# Run some tests if this file is executed directly
if __FILE__ == $0
  # Test FFT and IFFT
  data = [0, 1, 2, 3, 4, 5, 6, 7]
  fft_instance = FFT.new(data)
  puts "Original Data: #{data}"
  fft_result = fft_instance.fft(true)
  puts "FFT Result (Iterative): #{fft_result}"
  ifft_result = fft_instance.ifft(true)
  puts "Inverse FFT Result (Iterative): #{ifft_result}"
  fft_result_recursive = fft_instance.fft(false)
  puts "FFT Result (Recursive): #{fft_result_recursive}"
  ifft_result_recursive = fft_instance.ifft(false)
  puts "Inverse FFT Result (Recursive): #{ifft_result_recursive}"
  # Test convolution
  data1 = [1, 2, 3]
  data2 = [4, 5, 6]
  fft1 = FFT.new(data1)
  fft2 = FFT.new(data2)
  convolved = fft1 * fft2
  puts "Data1: #{data1}"
  puts "Data2: #{data2}"
  puts "Convolution Result: #{convolved}"
  data3 = [1, 2, 3, 4]
  data4 = [5, 6, 7]
  fft3 = FFT.new(data3)
  fft4 = FFT.new(data4)
  convolved2 = fft3 * fft4
  puts "Data3: #{data3}"
  puts "Data4: #{data4}"
  puts "Convolution Result 2: #{convolved2}"
end
