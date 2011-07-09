
import os

build_root = 'build'
object_root = os.path.join(build_root, 'object')
src_root = 'source'
inc_root = 'include'
program_name = 'ece554_project'

env = Environment()
env.VariantDir(object_root, src_root, duplicate=0)
ece554_src = [
	'ece554_project.cpp'
]

env = Environment()
env.Append(CPPPATH=[inc_root])

object_root_src = []
for f in ece554_src:
	object_root_src.append(os.path.join(object_root, f))

env.Program(target = os.path.join(build_root, program_name), source = object_root_src)

