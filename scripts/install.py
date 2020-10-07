import subprocess

subprocess.Popen('mkdir -p bin/', cwd='../build/').wait()
subprocess.Popen('cp -r ../package/* bin/', cwd='../build/').wait()
print('Package installed.')