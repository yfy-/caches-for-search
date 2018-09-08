from matplotlib import pyplot as plt
import numpy as np
import argparse
import pdb

parser = argparse.ArgumentParser()
parser.add_argument("file")
parser.add_argument("-s", "--select", nargs="*")
parser.add_argument("-d", "--display", nargs="*")

args = parser.parse_args()

data = np.genfromtxt(args.file, dtype=float, delimiter=',', names=True)

all_names = data.dtype.names
cache_column = [all_names[0]]
names = cache_column + args.select if args.select else all_names
display = cache_column + args.display if args.display else names
x_vals = data[names[0]]


markers = ['o', 'v', '^', '<', '>', '1', '2', '3', '4', 's', 'p']

for i in range(1, len(names)):
    plt.plot(x_vals, data[names[i]], '-' + markers[i - 1],
             label=display[i])

plt.xlabel(names[0].replace('_', ' '))
plt.ylabel("Precision")
plt.legend()
plt.show()
