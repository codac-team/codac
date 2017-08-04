#! /usr/bin/env python
# encoding: utf-8

######################
###### options #######
######################
def options (opt):
	opt.add_option ("--with-tubex", action="store_true",
			dest="WITH_TUBEX", help = "Use Tubex plugin")

######################
##### configure ######
######################
def configure (conf):
	conf.env.WITH_TUBEX = conf.options.WITH_TUBEX
		
	conf.start_msg ("Tubex plugin")
	if not conf.env.WITH_TUBEX: 
		conf.end_msg ("not used")
		return
	
	conf.end_msg ("enabled")
	conf.env.append_unique ("IBEX_PLUGIN_USE_LIST", "TUBEX")
	
	# Add information in ibex_Setting
	conf.setting_define ("WITH_TUBEX", 1)

	# add TUBEX plugin include directory
	for f in conf.path.ant_glob ("src/** src", dir = True, src = False):
		conf.env.append_unique("INCLUDES_TUBEX", f.abspath())

	# The build and install steps will be run from the main src/wscript script so
	# we need to give path relative to the main src directory
	mainsrc = conf.srcnode.make_node ("src")

	# add TUBEX headers
	for f in conf.path.ant_glob ("src/**/*.h"):
		conf.env.append_unique ("IBEX_HDR", f.path_from (mainsrc))

	# add TUBEX source files
	for f in conf.path.ant_glob ("src/**/*.cpp"):
		conf.env.append_unique ("IBEX_SRC", f.path_from (mainsrc))

	# The utest step will be run from the main tests/wscript script so we need to
	# give path relative to the main tests directory
	maintests = conf.srcnode.make_node ("tests")

	# add TUBEX test files
	for f in conf.path.ant_glob ("tests/**/*.cpp"):
		conf.env.append_unique ('TEST_SRC', f.path_from (maintests))

	# Add tubex/tests directory to list of INCLUDES for TESTS
	testsnode = conf.path.make_node ("tests")
	conf.env.append_unique ("INCLUDES_TESTS", testsnode.abspath ())

######################
####### build ########
######################
def build (bld):
	pass # nothing to do, everything is done in the main src/wscript script
