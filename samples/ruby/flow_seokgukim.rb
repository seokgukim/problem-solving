# Max Flow and related algorithms implemented in Ruby
class FlowGraph
  class Edge
    attr_accessor :from, :to, :capacity, :flow, :reverse_edge

    def initialize(from, to, capacity)
      @from = from
      @to = to
      @capacity = capacity
      @flow = 0
      @reverse_edge = nil
    end
  end

  attr_reader :graph

  def initialize(flow_max = 10**18)
    @flow_max = flow_max
    @graph = Hash.new { |h, k| h[k] = [] }
  end

  # Add edge to the flow graph
  # This creates both forward and backward edges
  def add_edge(from, to, capacity)
    forward_edge = Edge.new(from, to, capacity)
    backward_edge = Edge.new(to, from, 0)
    forward_edge.reverse_edge = backward_edge
    backward_edge.reverse_edge = forward_edge
    @graph[from] << forward_edge
    @graph[to] << backward_edge
  end

  # Dinic's algorithm for Maximum Flow
  # Time complexity: O(V^2 * E) in general, O(E * sqrt(V)) for unit networks
  def dinics(source, sink)
    max_flow = 0

    while (level = bfs_dinics(source, sink))
      break if level[sink].nil? # No more augmenting path

      iter = Hash.new(0)
      while (flow = dfs_dinics(source, sink, @flow_max, level, iter)) > 0 # Find blocking flow
        max_flow += flow
      end
    end

    max_flow
  end

  # Edmonds-Karp algorithm for Maximum Flow
  # Time complexity: O(V * E^2)
  def edmonds_karp(source, sink)
    max_flow = 0
    while (parent = bfs_edmonds_karp(source, sink)) # Find an augmenting path
      break if parent[sink].nil? # No more augmenting path

      increment = @flow_max
      # Trace back the path and find the minimum capacity
      v = sink
      while v != source
        edge = parent[v]
        increment = [increment, edge.capacity - edge.flow].min
        v = edge.from
      end
      # Update the flow along the path
      v = sink
      while v != source
        edge = parent[v]
        edge.flow += increment
        edge.reverse_edge.flow -= increment if edge.reverse_edge
        v = edge.from
      end
      max_flow += increment # Add the increment to the total flow
    end
    max_flow
  end

  # Bipartite Matching using Dinic's algorithm
  def bipartite_match(left_size, right_size, edges) # edges: array of [u, v] where u in left, v in right
    source = :source
    sink = :sink
    (0...left_size).each { |u| add_edge(source, u, 1) }
    (0...right_size).each { |v| add_edge(left_size + v, sink, 1) }
    edges.each { |u, v| add_edge(u, left_size + v, 1) }
    dinics(source, sink)
  end

  private

  def bfs_dinics(source, sink)
    level = {}
    queue = Queue.new
    queue << source
    level[source] = 0

    until queue.empty?
      node = queue.pop
      @graph[node].each do |edge|
        if edge.capacity - edge.flow > 0 && level[edge.to].nil?
          level[edge.to] = level[node] + 1
          queue << edge.to
        end
      end
    end

    level
  end

  def dfs_dinics(node, sink, flow, level, iter)
    return flow if node == sink

    while iter[node] < @graph[node].size # Explore edges
      edge = @graph[node][iter[node]]
      if edge.capacity - edge.flow > 0 && level[node] < level[edge.to]
        d = dfs_dinics(edge.to, sink, [flow, edge.capacity - edge.flow].min, level, iter)
        if d > 0
          edge.flow += d
          edge.reverse_edge.flow -= d if edge.reverse_edge
          return d
        end
      end
      iter[node] += 1 # Move to the next edge
    end

    0
  end

  def bfs_edmonds_karp(source, sink)
    parent = {}
    queue = Queue.new
    queue << source
    parent[source] = nil

    until queue.empty?
      node = queue.pop
      if node == sink
        return parent # Found an augmenting path
      end

      @graph[node].each do |edge|
        if edge.capacity - edge.flow > 0 && parent[edge.to].nil?
          parent[edge.to] = edge
          queue << edge.to
        end
      end
    end

    nil # No augmenting path found
  end
end

# Run some tests if this file is executed directly
if __FILE__ == $0
  # Example usage of the FlowGraph class
  graph = FlowGraph.new
  graph.add_edge(0, 1, 10)
  graph.add_edge(0, 2, 5)
  graph.add_edge(1, 2, 15)
  graph.add_edge(1, 3, 10)
  graph.add_edge(2, 4, 10)
  graph.add_edge(3, 4, 10)
  graph.add_edge(3, 5, 10)
  graph.add_edge(4, 5, 10)

  puts "Max Flow (Dinic's): #{graph.dinics(0, 5)}" # Expected output: Max Flow (Dinic's): 15

  # Reset flows for Edmonds-Karp test
  graph.graph.each_value do |edges|
    edges.each { |edge| edge.flow = 0 }
  end

  puts "Max Flow (Edmonds-Karp): #{graph.edmonds_karp(0, 5)}" # Expected output: Max Flow (Edmonds-Karp): 15

  # Bipartite Matching example
  bipartite_graph = FlowGraph.new
  edges = [[0, 0], [0, 1], [1, 0], [2, 1]]
  puts "Bipartite Matching: #{bipartite_graph.bipartite_match(3, 2, edges)}" # Expected output: Bipartite Matching: 2
end
