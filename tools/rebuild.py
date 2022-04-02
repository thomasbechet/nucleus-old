import subprocess
import os
import shutil

# Create build folder
if os.path.exists('../build/'):
    shutil.rmtree('../build')
os.makedirs('../build/')

# Call cmake
subprocess.call('cmake .. -G"MinGW Makefiles"', cwd='../build/', shell=True)
subprocess.call('cmake .. -G"MinGW Makefiles"', cwd='../build/', shell=True)

# Build
subprocess.call('python build.py', cwd='.', shell=True)