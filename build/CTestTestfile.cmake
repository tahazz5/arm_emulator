# CMake generated Testfile for 
# Source directory: /workspaces/arm_emulator
# Build directory: /workspaces/arm_emulator/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(EmulatorTests "/workspaces/arm_emulator/build/emulator_tests")
set_tests_properties(EmulatorTests PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/arm_emulator/CMakeLists.txt;34;add_test;/workspaces/arm_emulator/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
