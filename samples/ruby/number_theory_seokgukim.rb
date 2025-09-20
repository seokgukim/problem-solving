# Algorithms for Number Threory(정수론)

def mul_inv(a, b)
  # Extended Euclidean Algorithm to find the modular inverse of a mod b
  b0 = b
  x0, x1 = 0, 1
  return 1 if b == 1

  while a > 1
    q = a / b
    a, b = b, a % b
    x0, x1 = x1 - q * x0, x0
  end
  x1 += b0 if x1 < 0
  x1
end

def crt(congruences)
  # Chinese Remainder Theorem implementation
  prod = congruences.reduce(1) { |acc, (_, mod)| acc * mod }
  sum = congruences.reduce(0) { |acc, (rem, mod)| acc + rem * (prod / mod) * mul_inv(prod / mod, mod) }
  sum % prod
end

def extended_gcd(a, b)
  # Extended Euclidean Algorithm (non-recursive)
  # Returns [gcd, x, y] where gcd = a*x + b*y

  prev_r, r = a, b
  prev_s, s = 1, 0
  prev_t, t = 0, 1

  while r != 0
    quotient = prev_r / r
    prev_r, r = r, prev_r - quotient * r
    prev_s, s = s, prev_s - quotient * s
    prev_t, t = t, prev_t - quotient * t
  end

  [prev_r, prev_s, prev_t]
end

def crt_general(congruences)
  # General Chinese Remainder Theorem implementation
  # Returns [x, m] where x is the solution and m is the modulus

  a0 = congruences[0][0]; m0 = congruences[0][1]
  congruences[1..].each do |(a, m)|
    g = m0.gcd(m)
    return [-1, -1] if (a0 - a) % g != 0

    gg, pp, qq = extended_gcd(m0 / g, m)
    mod = m0.lcm(m)
    x = (a0 * (m / g) * qq + a * (m0 / g) * pp) % mod
    x += mod if x < 0
    a0, m0 = x, mod
  end
  [a0 % m0, m0]
end
