# Convex Hull O(N log N) implementation in Ruby

def ccw(p, q, r)
  # Cross product to determine the orientation of the triplet (p, q, r)
  # a.k.a. counter-clockwise test
  result = (q[0] - p[0]) * (r[1] - p[1]) - (q[1] - p[1]) * (r[0] - p[0])
  return 1 if result > 0
  return -1 if result < 0

  0
end

def euclidean_distance_sq(p, q)
  # Squared Euclidean distance between points p and q
  (p[0] - q[0])**2 + (p[1] - q[1])**2
end

def convex_hull(points)
  # Graham scan algorithm to find the convex hull of a set of 2D points
  return points if points.size <= 1

  # Find the point with the lowest y-coordinate (and leftmost in case of tie)
  points.sort_by! { |p| [p[1], p[0]] }
  pivot = points.shift

  # Sort points by polar angle with pivot
  points.sort! do |a, b|
    orientation = ccw(pivot, a, b)
    if orientation == 0
      euclidean_distance_sq(pivot, a) <=> euclidean_distance_sq(pivot, b)
    else
      -orientation
    end
  end

  # Build the convex hull using a stack
  hull = [pivot, points[0]]

  points[1..].each do |point|
    while hull.size >= 2 && ccw(hull[-2], hull[-1], point) <= 0
      hull.pop
    end
    hull.push(point)
  end

  hull
end
