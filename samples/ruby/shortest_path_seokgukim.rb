# Ruby implementation of Shoretest Path Algorithm
# Assumes a heap implementation is available from same directory
require_relative "heap_seokgukim"

class PathNode
  attr_reader :node, :cost
  def initialize(node, cost)
    @node = node
    @cost = cost
  end

  # Since the heap is a max-heap, we invert the comparison for min-heap behavior
  def <=>(other)
    other.cost <=> @cost
  end

  def >(other)
    self.<=>(other) > 0
  end

  def <=(other)
    !self.>(other)
  end    

  # Helper function to get values
  def to_a
    [@cost, @node]
  end
end

# Dijkstra's algorithm to find the shortest path in a graph
def dijkstra(graph, start)
  dist = [10**18] * graph.size
  dist[start] = 0

  heap = Heap.new
  heap.insert(PathNode.new(start, 0))

  until heap.empty?
    cur, u = heap.pop.to_a
    next if dist[u] < cur
    
    graph[u].each do |v, cost|
      next if dist[v] <= dist[u] + cost

      dist[v] = dist[u] + cost
      heap.insert(PathNode.new(v, dist[v]))
    end
  end
  dist
end

# SPFA (Shortest Path Faster Algorithm) to find the shortest path in a graph
# Includes negative cycle detection
def spfa(graph, start)
  dist = [10**18] * graph.size
  dist[start] = 0

  in_queue = [false] * graph.size
  cnt = [0] * graph.size
  queue = Queue.new
  queue.push(start)
  in_queue[start] = true

  until queue.empty?
    u = queue.pop
    in_queue[u] = false
    cnt[u] += 1
    return nil if cnt[u] >= graph.size # Negative cycle detected

    graph[u].each do |v, cost|
      next if dist[v] <= dist[u] + cost

      dist[v] = dist[u] + cost
      unless in_queue[v]
        queue.push(v)
        in_queue[v] = true
      end
    end
  end
  dist
end

# Run examples if this file is executed directly
if __FILE__ == $0
  graph = [
    [[1, 4], [2, 1]],
    [[3, 1]],
    [[1, 2], [3, 5]],
    []
  ]
  puts "Dijkstra's algorithm result:"
  p dijkstra(graph, 0) # Expected output: [0, 3, 1, 4]

  graph_with_negative_edge = [
    [[1, 4], [2, 1]],
    [[3, -2]],
    [[1, -2], [3, 5]],
    []
  ]
  puts "SPFA result:"
  p spfa(graph_with_negative_edge, 0) # Expected output: [0, -1, 1, -3]

  graph_with_negative_cycle = [
    [[1, 4]],
    [[2, -8]],
    [[0, 3]]
  ]
  puts "SPFA with negative cycle detection result:"
  p spfa(graph_with_negative_cycle, 0) # Expected output: nil (negative cycle detected)
end
