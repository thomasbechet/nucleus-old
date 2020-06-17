import subprocess
import time

subprocess.Popen('glslc shader.vert -o ../../../bin/engine/shaders/shader.vert.spv', cwd='../nucleus/vulkan/shaders/').wait()
subprocess.Popen('glslc shader.frag -o ../../../bin/engine/shaders/shader.frag.spv', cwd='../nucleus/vulkan/shaders/').wait()
print("Shaders compiled.")