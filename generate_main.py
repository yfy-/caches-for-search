import math
import subprocess

filepath = '../src/main.cpp'
query_size = 200000
unique_size = 135009

for k in range(10, 2, -1):
    cache_size = unique_size / (2 ** k)
    for l in range(10, 0, -1):
        window_size = query_size / (2 ** l)
        wc_ratio = math.ceil(window_size / (1.0 * cache_size))

        if wc_ratio < 2 or (query_size / (1.0 * window_size)) < 2:
            continue

        for m in range(9, -1, -1):
            warm_up = (2 ** m) * window_size

            if warm_up > query_size / 2.0 or warm_up < cache_size:
                continue
                
            with open(filepath, 'r') as file:
                data = file.readlines()

            data[52] = "\tconstexpr std::uint64_t cache_size = " + str(cache_size) + ";\n"
            data[53] = "\tconstexpr std::uint64_t window_size = " + str(window_size) + ";\n"
            data[54] = "\tconstexpr std::uint64_t warm_up = " + str(warm_up) + ";\n"

            with open(filepath, 'w') as file:
                file.writelines(data)

            print subprocess.check_output(['make'])
            process = subprocess.Popen(['./ceng778_project'], stdout=subprocess.PIPE)
            process.wait()
            for line in process.stdout:
                print(line)
