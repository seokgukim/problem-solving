# Line intersection calculation using vector cross products
# Reference: https://stackoverflow.com/a/565282/1233763
# Assumes lines are given as [[x1, y1], [x2, y2]] format

def line_intersection(l1, l2)
  # Check if l1 and l2 cross each other
  # Return intersection point [x, y] or nil if they don't intersect

  # Validate input
  return nil unless l1.size == 2 && l2.size == 2
  return nil unless l1.all? { |point| point.size == 2 } && l2.all? { |point| point.size == 2 }

  (x1, y1), (x2, y2) = l1
  (x3, y3), (x4, y4) = l2

  denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
  return nil if denom.zero? # Lines are parallel or coincident

  px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denom.to_f
  py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denom.to_f

  [px, py]
end
