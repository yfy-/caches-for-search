import numpy as np
import matplotlib.pyplot as plt
from scipy import stats
import array
import pdb

def plot_zipf(data, color, label):
    _, data_counts = np.unique(data, return_counts=True)
    data_counts_lg = np.log2(np.sort(data_counts)[::-1])
    data_ranks = np.log2(np.array(range(1, data_counts.size + 1)))
    data_slope, data_intercept, _, _, _ = stats.linregress(data_ranks, data_counts_lg)
    print("%s slope, intercept = %f, %f" % (label, data_slope, data_intercept))
    plt.plot(data_ranks, data_slope * data_ranks + data_intercept, color=color, label=label)

def bin_to_vec(file):
    with open(file, 'rb') as f:
        data = np.array(array.array('i', f.read()))
    return data

data_size = 1000000
aol_data = []

with open('../resource/aol_big_train_sorted.txt') as f:
    for i in range(data_size):
        aol_data.append(f.readline().strip())

aol_data = np.array(aol_data)

# pdb.set_trace()

# plot_zipf(aol_data, 'tab:blue', 'aol')
# plot_zipf(bin_to_vec('zipf_0.1_1m.bin'), 'tab:orange', '0.1')
# plot_zipf(bin_to_vec('zipf_0.2_1m.bin'), 'tab:green', '0.2')
# plot_zipf(bin_to_vec('zipf_0.3_1m.bin'), 'tab:red', '0.3')
# plot_zipf(bin_to_vec('zipf_0.4_1m.bin'), 'tab:purple', '0.4')
# plot_zipf(bin_to_vec('zipf_0.5_1m.bin'), 'tab:brown', '0.5')
# plot_zipf(bin_to_vec('zipf_0.7_1m.bin'), 'tab:pink', '0.7')
# plot_zipf(bin_to_vec('zipf_0.9_1m.bin'), 'tab:gray', '0.9')
# plt.xlabel("lg(rank)")
# plt.ylabel("lg(frequency)")
# plt.legend()
# plt.show()

plot_zipf(aol_data, 'tab:blue', 'aol')
plot_zipf(np.random.zipf(1.1, data_size), 'tab:orange', 'np1.1')
plot_zipf(np.random.zipf(1.2, data_size), 'tab:green', 'np1.2')
plot_zipf(np.random.zipf(1.3, data_size), 'tab:red', 'np1.3')
# plot_zipf(np.random.zipf(1.4, data_size), 'tab:purple', 'np1.4')
# plot_zipf(np.random.zipf(1.5, data_size), 'tab:brown', 'np1.5')
# plot_zipf(np.random.zipf(1.7, data_size), 'tab:pink', 'np1.7')
# plot_zipf(np.random.zipf(1.9, data_size), 'tab:gray', 'np1.9')
plt.xlabel("lg(rank)")
plt.ylabel("lg(frequency)")
plt.legend()
plt.show()
