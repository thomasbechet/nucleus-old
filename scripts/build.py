import subprocess

subprocess.Popen(['mingw32-make.exe', 'install'], cwd='../build/').wait()