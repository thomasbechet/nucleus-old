import subprocess

subprocess.Popen('mkdir -p build', cwd='..').wait()
subprocess.Popen('rm -rf *', cwd='../build/').wait()
subprocess.Popen('cmake .. -G"MinGW Makefiles"', cwd='../build/').wait()
subprocess.Popen('cmake .. -G"MinGW Makefiles"', cwd='../build/').wait()
subprocess.Popen('python build.py', cwd='.').wait()