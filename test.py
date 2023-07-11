import pandas as pd
import matplotlib.pyplot as plt

def read_csv(filename):
    df = pd.read_csv(filename, header=1, names=['Data Size (bytes)', 'Latency (seconds)', 'Communication Time (seconds)'])
    return df

def plot_latency(data, title, filename=None):
    plt.figure()
    plt.plot(data['Data Size (bytes)'], data['Latency (seconds)'])
    plt.xlabel('Data size (bytes)')
    plt.ylabel('Latency (seconds)')
    plt.title(title)
    if filename:
        plt.savefig(filename)
    else:
        plt.show()

def plot_communication(data, title, filename=None):
    plt.figure()
    plt.plot(data['Data Size (bytes)'], data['Communication Time (seconds)'])
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
    results = read_csv('timings.csv')

    # Filter data by data size
    filtered_data1 = results[results['Data Size (bytes)'].isin(data_sizes1)]
    filtered_data2 = results[results['Data Size (bytes)'].isin(data_sizes2)]

    # plot_latency(filtered_data1, 'Hot Potato latency (0-512 bytes, 32-byte increments)', 'sxb3318_problem_3b_graph_latency_0-512.png')
    plot_communication(filtered_data1, 'Hot Potato communication time (0-512 bytes, 32-byte increments)', 'sxb3318_problem_3b_graph_communication_0-512.png')
    # plot_latency(filtered_data2, 'Hot Potato latency (1-128 KB, 1 KB increments)', 'sxb3318_problem_3b_graph_latency_1-128k.png')
    plot_communication(filtered_data2, 'Hot Potato communication time (1-128 KB, 1 KB increments)', 'sxb3318_problem_3b_graph_communication_1-128k.png')
