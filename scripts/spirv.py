import subprocess

subprocess.Popen('glslc shader.vert -o ../../../../package/engine/shader/spirv/shader.vert.spv', cwd='../nucleus/system/vulkan/shader/').wait()
subprocess.Popen('glslc shader.frag -o ../../../../package/engine/shader/spirv/shader.frag.spv', cwd='../nucleus/system/vulkan/shader/').wait()
print("Shaders compiled.")