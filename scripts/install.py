from distutils.dir_util import copy_tree

# Copy content
copy_tree('../package/', '../build/bin/')

# subprocess.Popen(['cp', '-r', '../package/.', 'bin/'], cwd='../build/').wait()
print('Package installed.')