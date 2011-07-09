
import os
import sys

  #**************************************************************************************#
 #										        #
#***************************************************************************************#
## The directories to work with								 #
###***************************************************************************************#
build_root = 'build'
object_root = os.path.join(build_root, 'object')
src_root = 'source'
inc_root = 'include'
program_name = 'ece554_project'

  #**************************************************************************************#
 #										        #
#***************************************************************************************#
## The source files that we want to use for the build					 #
###***************************************************************************************#
ece554_src = [
	'ece554_project.cpp'
]

  #**************************************************************************************#
 #										        #
#***************************************************************************************#
## The help string that is outputted via scons -h					 #
###***************************************************************************************#
Help("""
This SConstruct file will build the ece554_project application

The following variables can be passed to scons to modify the build:

(variable name)		(parameters)
===============		============
output			verbose
			quiet (default)
	""")

  #**************************************************************************************#
 #										        #
#***************************************************************************************#
## defines the set of options that are possible						 #
###***************************************************************************************#
#
options_dict = {
	'output' : {
		'verbose' : {
			'compiler' : [''],
			'linker' : ['']
			},
		'quiet' : {
			'compiler' : ["Compiling $TARGET                       "],
			'linker' : ["Linking $TARGET                       "]
			}
		},
	}

  #**************************************************************************************#
 #										        #
#***************************************************************************************#
## contains a set of default options to use for a build					 #
###***************************************************************************************#
#
default_options_dict = {
		'output' : options_dict['output']['quiet']
	}

###**************************************************************************************#
##										        #
#***************************************************************************************#
## set the build options to the default dictionary					 #
##***************************************************************************************#
build_options_dict = default_options_dict

  #**************************************************************************************#
 #										        #
#***************************************************************************************#
## Build the apps									 #
###***************************************************************************************#
if '-h' not in sys.argv:
	if 'verbose' == ARGUMENTS.get('output', 'quiet'):
		print '+Configuring verbose output...'
		build_options_dict['output'] = options_dict['output']['verbose']

	object_root_src = []
	for f in ece554_src:
		object_root_src.append(os.path.join(object_root, f))

	env = Environment();

	env.Replace(
		LINKCOMSTR= build_options_dict['output']['linker'],
		CCCOMSTR =  build_options_dict['output']['compiler'],
		CXXCOMSTR = build_options_dict['output']['compiler'],
		CPPPATH = [inc_root],
		ENV = os.environ)

	env.VariantDir(object_root, src_root, duplicate=0)
	env.Program(target = os.path.join(build_root, program_name), source = object_root_src)
