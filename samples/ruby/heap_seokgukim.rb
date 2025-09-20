class Heap
  def initialize
    @heap = []
  end

  def insert(value)
    @heap << value
    heapify_up(@heap.size - 1)
  end

  def pop
    return nil if @heap.empty?

    root = @heap[0]
    swap(0, @heap.size - 1)
    @heap.pop
    heapify_down(0)
    root
  end

  def peek
    return nil if @heap.empty?

    @heap[0]
  end

  def size
    @heap.size
  end

  def empty?
    @heap.empty?
  end

  private

  def heapify_up(index)
    parent_index = (index - 1) / 2
    return if index == 0 || @heap[index] <= @heap[parent_index]

    swap(index, parent_index)
    heapify_up(parent_index)
  end

  def heapify_down(index)
    left_child_index = index * 2 + 1
    right_child_index = index * 2 + 2
    largest_index = index

    largest_index = left_child_index if left_child_index < @heap.size && @heap[left_child_index] > @heap[largest_index]

    if right_child_index < @heap.size && @heap[right_child_index] > @heap[largest_index]
      largest_index = right_child_index
    end

    return if largest_index == index

    swap(index, largest_index)
    heapify_down(largest_index)
  end

  def swap(index1, index2)
    @heap[index1], @heap[index2] = @heap[index2], @heap[index1]
  end
end
