import subprocess
import time

subprocess.Popen('mingw32-make install', cwd='../build/').wait()
time.sleep(0.5)
subprocess.Popen('../bin/nucleus_test', cwd='../bin/').wait()