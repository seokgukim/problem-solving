# Simulated Annealing heuristic optimization algorithm implementation in Ruby
class SimulatedAnnealing
  attr_reader :initial_temp, :final_temp, :alpha, :max_iterations, :cost, :neighbor

  def initialize(initial_temp: 1000, final_temp: 1, alpha: 0.9, max_iterations: 1000, cost: nil, neighbor: nil)
    @initial_temp = initial_temp
    @final_temp = final_temp
    @alpha = alpha
    @max_iterations = max_iterations
    @cost = cost || method(:default_cost)
    @neighbor = neighbor || method(:default_neighbor)
  end

  def optimize(initial_solution)
    current_solution = initial_solution
    current_cost = @cost.call(current_solution)
    best_solution = current_solution
    best_cost = current_cost
    temperature = @initial_temp

    while temperature > @final_temp
      @max_iterations.times do
        new_solution = @neighbor.call(current_solution)
        new_cost = @cost.call(new_solution)
        delta_cost = new_cost - current_cost

        if delta_cost < 0 || acceptance_probability(delta_cost, temperature) > rand
          current_solution = new_solution
          current_cost = new_cost

          if current_cost < best_cost
            best_solution = current_solution
            best_cost = current_cost
          end
        end
      end
      temperature *= @alpha
    end

    best_solution
  end

  private

  def default_cost(solution)
    # Define the cost function for the problem at hand.
    # We find a solution that minimizes this cost.
    # This is a placeholder and should be replaced with the actual implementation.
    solution.reduce(0) { |sum, x| sum + x**2 }
  end

  def default_neighbor(solution)
    # Generate a neighboring solution by making a small change to the current solution.
    # This is a placeholder and should be replaced with the actual implementation.
    new_solution = solution.dup
    index = rand(new_solution.size)
    new_solution[index] += rand(-1..1)
    new_solution
  end

  def acceptance_probability(delta_cost, temperature)
    # Acceptance probability function
    # Even if the new solution is worse, it may be accepted with a certain probability
    Math.exp(-delta_cost / temperature)
  end
end

# Run an example optimization if this file is executed directly
if __FILE__ == $0
  # Default example with random initial solution
  initial_solution = Array.new(10) { rand(-10..10) }
  sa = SimulatedAnnealing.new
  best_solution = sa.optimize(initial_solution)
  puts "Initial Solution: #{initial_solution}"
  puts "Best Solution: #{best_solution}"
  puts "Best Cost: #{sa.cost.call(best_solution)}"
  puts "When cost is sum of squares."
  puts '-' * 40
  # Custiomized example with user-defined cost and neighbor functions
  # Assume cost is x^2 - 4x + 4 and neighbor changes x by ±1
  custom_cost = lambda { |x| (x - 2)**2 }
  custom_neighbor = lambda { |x| x + rand(-1..1) }
  initial_solution = rand(-10..10)
  sa_custom = SimulatedAnnealing.new(cost: custom_cost, neighbor: custom_neighbor)
  best_solution_custom = sa_custom.optimize(initial_solution)
  puts "Initial Solution (Custom): #{initial_solution}"
  puts "Best Solution (Custom): #{best_solution_custom}"
  puts "Best Cost (Custom): #{sa_custom.cost.call(best_solution_custom)}"
  puts "When cost is (x-2)^2."
end
