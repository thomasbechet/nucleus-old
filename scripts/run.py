import subprocess
import platform
from platform import uname

print(uname())
if platform.system() == 'Windows' or ('Microsoft' in uname()[2]):
    subprocess.call(['nucleus_test.exe'], cwd='../build/bin/', shell=True)
else:
    subprocess.call(['./nucleus_test'], cwd='../build/bin/')