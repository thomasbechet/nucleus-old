import subprocess
import os
import time

subprocess.Popen('mingw32-make install', cwd='../build/').wait()
time.sleep(0.5)
subprocess.run(['../bin/nucleus_test.exe'])