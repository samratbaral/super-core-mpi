import re
import matplotlib.pyplot as plt

# Initialize lists to store data
simpsons_scalability = []
simple_scalability = []
simpsons_accuracy = []
simple_accuracy = []
processors = []

# Read the input lines
with open('output_2.txt', 'r') as file:
    lines = file.readlines()

# Process each line
for line in lines:
    line = line.strip()
    if line == "":
        continue

    # Extract the value of pi using regular expression
    match = re.search("Approximation of pi: ([\d.e+-]+)", line)
    if match:
        pi_value = float(match.group(1))
        if len(simpsons_scalability) < len(processors) + 1:
            simpsons_scalability.append(pi_value)
        else:
            simple_scalability.append(pi_value)
        processors.append(len(processors) + 1)

# Generate accuracy data
simpsons_accuracy = [abs(pi - simpsons_scalability[-1]) for pi in simpsons_scalability]
simple_accuracy = [abs(pi - simple_scalability[-1]) for pi in simple_scalability]

# Plot the scalability vs. processors
plt.figure(figsize=(10, 5))
plt.plot(processors[:len(simpsons_scalability)], simpsons_scalability, label="Simpson's Rule")
plt.plot(processors[:len(simple_scalability)], simple_scalability, label="Simple Scheme")
plt.xlabel('Processors')
plt.ylabel('Value of pi')
plt.title('Scalability vs. Processors')
plt.legend()
plt.savefig('sxb3318_problem_2_graph_scalability_vs_processors.png')
plt.show()

# Plot the accuracy vs. processors
plt.figure(figsize=(10, 5))
plt.plot(processors[:len(simpsons_accuracy)], simpsons_accuracy, label="Simpson's Rule")
plt.plot(processors[:len(simple_accuracy)], simple_accuracy, label="Simple Scheme")
plt.xlabel('Processors')
plt.ylabel('Accuracy')
plt.title('Accuracy vs. Processors')
plt.legend()
plt.savefig('sxb3318_problem_2_graph_accuracy_vs_processors.png')
plt.show()
