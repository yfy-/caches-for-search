import math
import subprocess

filepath = '../src/main.cpp'
query_size = 200000
unique_size = 135009

for k in range(10, 1, -1):
    cache_size = unique_size / (2 ** k)
    for l in range(1, 6):
        window_size = cache_size * (2 ** l)

        if window_size >= query_size:
            continue

                
        with open(filepath, 'r') as file:
            data = file.readlines()

        print "Cache size = " + str(cache_size) + "\n"
        print "Window size = " + str(window_size) + "\n"
            
        data[55] = "\tconstexpr std::uint64_t cache_size = " + str(cache_size) + ";\n"
        data[56] = "\tconstexpr std::uint64_t window_size = " + str(window_size) + ";\n"

        with open(filepath, 'w') as file:
            file.writelines(data)

        print subprocess.check_output(['make'])
        process = subprocess.Popen(['./ceng778_project'], stdout=subprocess.PIPE)
        process.wait()
        for line in process.stdout:
            print(line)
