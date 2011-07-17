
import os
import sys

#***************************************************************************************#
# The directories to work with								#
#***************************************************************************************#
build_root = 'build'
object_root = os.path.join(build_root, 'object')
main_src_root = 'source'
mixer_src_root = 'AudioMixer'
client_src_root = 'AudioClient'
common_src_root = 'Common'


inc_roots = [
	'include',
	main_src_root,
	mixer_src_root,
	client_src_root,
	'/usr/include',
	'/usr/include/glib-2.0',
	'/usr/lib/glib-2.0/include',
	'/usr/local/include',
	]

lib_roots = [
	'lib',
	'/lib',
	'/usr/lib',
]

libs = [
	'asound',
	'pthread',
	'glib-2.0',
	'rt'
]

main_program_name = 'ece554_project'
mixer_program_name = 'audio_mixing_server'

#***************************************************************************************#
# The source files that we want to use for the build					#
#***************************************************************************************#
common_src = [
	'Mutex.cpp'
]

audio_mixer_src = [
	'AlsaAudioPlayback.cpp',
	'audio_mixer.cpp',
	'PlaybackThread.cpp'
]

#***************************************************************************************#
# The help string that is outputted via scons -h					#
#***************************************************************************************#
Help("""
This SConstruct file will build the ece554_project application

The following variables can be passed to scons to modify the build:

(variable name)		(parameters)
===============		============
debug			1
			0 (default)
output			verbose
			quiet (default)
	""")

#***************************************************************************************#
# defines the set of options that are possible						#
#***************************************************************************************#
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
	'debug' : {
		'1' : ['-ggdb3'],
		'0' : ['']
		}
	}

#***************************************************************************************#
# contains a set of default options to use for a build					#
#***************************************************************************************#
default_options_dict = {
		'output' : options_dict['output']['quiet'],
		'debug' : options_dict['debug']['0']
	}

#****************************************************************************************#
# set the build options to the default dictionary					 #
#****************************************************************************************#
build_options_dict = default_options_dict

#****************************************************************************************#
# Build the apps									 #
#****************************************************************************************#
if '-h' not in sys.argv:
	if 'verbose' == ARGUMENTS.get('output', 'quiet'):
		print '+Configuring verbose output...'
		build_options_dict['output'] = options_dict['output']['verbose']

	if '1' == ARGUMENTS.get('debug', '0'):
		print '+Configuring debug symbols...'
		build_options_dict['debug'] = options_dict['debug']['1']

	common_object_root_src = []
	for f in common_src:
		common_object_root_src.append(os.path.join(object_root, common_src_root, f))

	mixer_object_root_src = common_object_root_src
	for f in audio_mixer_src:
		mixer_object_root_src.append(os.path.join(object_root, mixer_src_root, f))

	env = Environment();

	env.Replace(
		LIBPATH = lib_roots,
		LIBS = libs,
		LINKCOMSTR= build_options_dict['output']['linker'],
		CCCOMSTR =  build_options_dict['output']['compiler'],
		CXXCOMSTR = build_options_dict['output']['compiler'],
		CCFLAGS = build_options_dict['debug'],
		CXXFLAGS = build_options_dict['debug'],
		CPPPATH = inc_roots,
		ENV = os.environ)

	env.VariantDir(object_root, main_src_root, duplicate=0)
	env.Program(target = os.path.join(build_root, mixer_program_name), source = mixer_object_root_src)
