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
