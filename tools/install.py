from distutils.dir_util import copy_tree

# Copy content
copy_tree('../package/', '../build/bin/')

print('Package installed.')