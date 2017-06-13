import subprocess

filepath = '../src/main.cpp'

for i in range(1, 6):
    with open(filepath, 'r') as file:
        data = file.readlines()

    data[60] = "\tconstexpr std::uint64_t n_segments = " + str(i*10) + ";\n"
    
    with open(filepath, 'w') as file:
        file.writelines(data)

    print subprocess.check_output(['make'])

    process = subprocess.Popen(['./ceng778_project'], stdout=subprocess.PIPE)
    process.wait()
    for line in process.stdout:
        print(line)
