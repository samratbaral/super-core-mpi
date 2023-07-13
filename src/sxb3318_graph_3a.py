import csv
import matplotlib.pyplot as plt

def read_csv(filename):
    data = []
    with open(filename, 'r') as f:
        reader = csv.reader(f)
        next(reader)  # skip header row
        for row in reader:
            data.append((int(row[0]), float(row[1]), float(row[2])))
    return data

def plot_latency(data, title, filename=None):
    plt.figure()
    plt.plot([d[0] for d in data], [d[1] for d in data])
    plt.xlabel('Data size (bytes)')
    plt.ylabel('Latency (seconds)')
    plt.title(title)
    if filename:
        plt.savefig(filename)
    else:
        plt.show()

def plot_communication(data, title, filename=None):
    plt.figure()
    plt.plot([d[0] for d in data], [d[2] for d in data])
    plt.xlabel('Data size (bytes)')
    plt.ylabel('Communication time (seconds)')
    plt.title(title)
    if filename:
        plt.savefig(filename)
    else:
        plt.show()

if __name__ == '__main__':
    data_sizes1 = range(0, 513, 32)
    data_sizes2 = range(1024, 129*1024+1, 1024)
    results = read_csv('output_3a.csv')

    # Filter data by data size
    filtered_data1 = [(d[0], d[1], d[2]) for d in results if d[0] in data_sizes1]
    filtered_data2 = [(d[0], d[1], d[2]) for d in results if d[0] in data_sizes2]

    plot_latency(filtered_data1, 'Ping-pong latency (0-512 bytes, 32-byte increments)', 'sxb3318_problem_3a_graph_latency_0-512.png')
    plot_communication(filtered_data1, 'Ping-pong communication time (0-512 bytes, 32-byte increments)', 'sxb3318_problem_3a_graph_communication_0-512.png')
    plot_latency(filtered_data2, 'Ping-pong latency (1-128 KB, 1 KB increments)', 'sxb3318_problem_3a_graph_latency_1-128k.png')
    plot_communication(filtered_data2, 'Ping-pong communication time (1-128 KB, 1 KB increments)', 'sxb3318_problem_3a_graph_communication_1-128k.png')
