class Treap
  class Node
    attr_accessor :key, :priority, :size, :left, :right

    def initialize(key)
      @key = key
      @priority = rand(0..1_000_000_000) # Large range for priorities
      @size = 1
      @left = nil
      @right = nil
    end

    def update_size
      @size = 1 + (@left ? @left.size : 0) + (@right ? @right.size : 0)
    end

    # Helper to avoid direct access and ensure size update
    def set_left(node)
      @left = node
      update_size
    end

    def set_right(node)
      @right = node
      update_size
    end
  end

  attr_accessor :root

  def initialize
    @root = nil
  end

  def empty?
    @root.nil?
  end

  def size
    @root ? @root.size : 0
  end

  def insert(key)
    new_node = Node.new(key)
    @root = _insert(@root, new_node)
  end

  def erase(key)
    @root = _erase(@root, key)
  end

  def find(key)
    current = @root
    while current
      if key < current.key
        current = current.left
      elsif key > current.key
        current = current.right
      else
        return current # Or current.key if you just want the value
      end
    end
    nil
  end

  def kth(k) # 0-indexed
    _kth(@root, k)
  end

  def count_less(key)
    _count_less(@root, key)
  end

  private

  # Returns [left_subtree, right_subtree]
  def _split(node, key)
    return [nil, nil] unless node

    if node.key < key
      left_subtree, right_subtree = _split(node.right, key)
      node.set_right(left_subtree)
      [node, right_subtree]
    else
      left_subtree, right_subtree = _split(node.left, key)
      node.set_left(right_subtree)
      [left_subtree, node]
    end
  end

  def _merge(left_node, right_node)
    return right_node unless left_node
    return left_node unless right_node

    if left_node.priority < right_node.priority
      right_node.set_left(_merge(left_node, right_node.left))
      right_node
    else
      left_node.set_right(_merge(left_node.right, right_node))
      left_node
    end
  end

  def _insert(parent, node_to_insert)
    return node_to_insert unless parent

    if node_to_insert.priority > parent.priority
      left_subtree, right_subtree = _split(parent, node_to_insert.key)
      node_to_insert.set_left(left_subtree)
      node_to_insert.set_right(right_subtree)
      node_to_insert
    elsif node_to_insert.key < parent.key
      parent.set_left(_insert(parent.left, node_to_insert))
      parent
    else # node_to_insert.key >= parent.key (handle duplicates by inserting to the right, or disallow)
      parent.set_right(_insert(parent.right, node_to_insert))
      parent
    end
  end

  def _erase(parent, key)
    return nil unless parent

    if key < parent.key
      parent.set_left(_erase(parent.left, key))
    elsif key > parent.key
      parent.set_right(_erase(parent.right, key))
    else # key == parent.key
      merged_children = _merge(parent.left, parent.right)
      return merged_children
    end
    parent # Return parent if key was not found in this branch or after updating children
  end

  def _kth(node, k) # 0-indexed
    return nil if !node || k >= node.size

    left_size = node.left ? node.left.size : 0

    if k < left_size
      _kth(node.left, k)
    elsif k > left_size
      _kth(node.right, k - left_size - 1)
    else # k == left_size
      node
    end
  end

  def _count_less(node, key)
    return 0 unless node

    if key <= node.key 
      _count_less(node.left, key)
    else
      (node.left ? node.left.size : 0) + 1 + _count_less(node.right, key)
    end
  end
end