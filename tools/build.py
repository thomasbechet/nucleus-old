import subprocess

# Call make command
subprocess.call('mingw32-make.exe install', cwd='../build/', shell=True)