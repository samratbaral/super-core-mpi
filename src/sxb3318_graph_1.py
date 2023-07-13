import re
import matplotlib.pyplot as plt

def extract_data(filename):
    data = {
        'processors': [],
        'execution_time': [],
        'primes_found': [],
        'given_number': []
    }

    with open(filename, 'r') as file:
        lines = file.readlines()

    for line in lines:
        match = re.search(r'Processor (\d+) took ([\d.]+) seconds to found (\d+) primes less than or equal to in the given number (\d+)', line)
        if match:
            processors = int(match.group(1))
            execution_time = float(match.group(2))
            primes_found = int(match.group(3))
            given_number = int(match.group(4))

            data['processors'].append(processors)
            data['execution_time'].append(execution_time)
            data['primes_found'].append(primes_found)
            data['given_number'].append(given_number)

    return data

def calculate_speedup(data, sequential_time):
    speedup = []
    for time in data['execution_time']:
        speedup.append(sequential_time / time)
    return speedup

def plot_speedup(data, sequential_time):
    speedup = calculate_speedup(data, sequential_time)
    processors = data['processors']

    plt.plot(processors, speedup, marker='o')
    plt.xlabel('Number of Processors')
    plt.ylabel('Speedup')
    plt.title('Speedup vs Processor Plot')
    plt.grid(True)

    # Save the graph as a PNG file
    plt.savefig('sxb3318_problem_1_graph.png')
    plt.show()

# Example usage
data = extract_data('output_1.txt')
sequential_time = data['execution_time'][0]  # Assuming the sequential time is the first execution time
plot_speedup(data, sequential_time)
