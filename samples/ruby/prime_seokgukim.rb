require "prime" # Use the 'prime' library for prime number operations

# Pollard's Rho algorithm for integer factorization
def pollards_rho(n)
  return n if n.prime?
  return 1 if n <= 1

  ret = n
  cnt = 0
  while !ret.prime? && cnt < 500
    x = rand(2...n); y = x; d = 1; c = rand(1...n)
    while d == 1
      x = (x * x + c) % n
      y = (y * y + c) % n
      y = (y * y + c) % n
      d = (x - y).abs.gcd(n)
    end
    ret = d
    cnt += 1
  end

  return ret if ret.prime?

  (2..n).each do |i|
    if n % i == 0
      return i if i.prime?
    end
  end

  return nil
end

# Factorization function using Pollard's Rho
def factorize(n)
  factors = []
  while n > 1
    factor = pollards_rho(n)
    factors << factor
    n /= factor while n % factor == 0
  end
  return factors.sort
end

# Function to calculate Euler's Totient function
def eulers_phi(n)
  return 1 if n == 1

  factors = factorize(n).uniq
  result = n
  factors.each do |p|
    result *= (1 - 1.0 / p)
  end
  return result.to_i
end

# O(sqrt(n)) primality test
def naive_is_prime?(n)
  return false if n <= 1

  (2..Integer.sqrt(n)).each do |i|
    return false if n % i == 0
  end
  return true
end

# Common Sieve of Eratosthenes implementation
def sieve(n)
  primes = []
  is_prime = Array.new(n + 1, true)
  is_prime[0] = is_prime[1] = false

  (2..n).each do |i|
    if is_prime[i]
      primes << i
      (i * i..n).step(i) { |j| is_prime[j] = false }
    end
  end

  return primes
end
