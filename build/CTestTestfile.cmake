# CMake generated Testfile for 
# Source directory: /home/chiara/programmazione/rete
# Build directory: /home/chiara/programmazione/rete/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[neuralnet.t]=] "/home/chiara/programmazione/rete/build/Debug/neuralnet.t")
  set_tests_properties([=[neuralnet.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/chiara/programmazione/rete/CMakeLists.txt;53;add_test;/home/chiara/programmazione/rete/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[neuralnet.t]=] "/home/chiara/programmazione/rete/build/Release/neuralnet.t")
  set_tests_properties([=[neuralnet.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/chiara/programmazione/rete/CMakeLists.txt;53;add_test;/home/chiara/programmazione/rete/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[neuralnet.t]=] "/home/chiara/programmazione/rete/build/RelWithDebInfo/neuralnet.t")
  set_tests_properties([=[neuralnet.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/chiara/programmazione/rete/CMakeLists.txt;53;add_test;/home/chiara/programmazione/rete/CMakeLists.txt;0;")
else()
  add_test([=[neuralnet.t]=] NOT_AVAILABLE)
endif()
