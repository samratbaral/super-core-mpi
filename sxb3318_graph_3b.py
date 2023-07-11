import csv
import matplotlib.pyplot as plt

def read_csv(filename):
    with open(filename, 'r') as csvfile:
        reader = csv.reader(csvfile)
        next(reader) # skip the header row
        data = []
        for row in reader:
            data.append({'Data Size (bytes)': int(row[0]), 'Latency (seconds)': float(row[1]), 'Communication Time (seconds)': float(row[2])})
        return data

def plot_latency(data, title, filename=None):
    plt.figure()
    data_sizes = [row['Data Size (bytes)'] for row in data]
    latencies = [row['Latency (seconds)'] for row in data]
    plt.plot(data_sizes, latencies)
    plt.xlabel('Data size (bytes)')
    plt.ylabel('Latency (seconds)')
    plt.title(title)
    if filename:
        plt.savefig(filename)
    else:
        plt.show()

def plot_communication(data, title, filename=None):
    plt.figure()
    data_sizes = [row['Data Size (bytes)'] for row in data]
    comm_times = [row['Communication Time (seconds)'] for row in data]
    plt.plot(data_sizes, comm_times)
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
    results = read_csv('output_3b.csv')

    # Filter data by data size
    filtered_data1 = [row for row in results if row['Data Size (bytes)'] in data_sizes1]
    filtered_data2 = [row for row in results if row['Data Size (bytes)'] in data_sizes2]

    plot_latency(filtered_data1, 'Hot Potato latency (0-512 bytes, 32-byte increments)', 'sxb3318_problem_3b_graph_latency_0-512.png')
    plot_communication(filtered_data1, 'Hot Potato communication time (0-512 bytes, 32-byte increments)', 'sxb3318_problem_3b_graph_communication_0-512.png')
    plot_latency(filtered_data2, 'Hot Potato latency (1-128 KB, 1 KB increments)', 'sxb3318_problem_3b_graph_latency_1-128k.png')
    plot_communication(filtered_data2, 'Hot Potato communication time (1-128 KB, 1 KB increments)', 'sxb3318_problem_3b_graph_communication_1-128k.png')
