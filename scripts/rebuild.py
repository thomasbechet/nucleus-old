import subprocess

subprocess.Popen('rm -rf *', cwd='../build/').wait()
subprocess.Popen('cmake .. -G"MinGW Makefiles"', cwd='../build/').wait()
subprocess.Popen('cmake .. -G"MinGW Makefiles"', cwd='../build/').wait()
subprocess.Popen('python build.py', cwd='.').wait()