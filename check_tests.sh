#!/bin/bash

./make/tests/core/tubelib_tests_core;
./make/examples/cpp/08_solver_01_picard/solver_01_picard > /dev/null ; echo $?;
#./make/examples/cpp/08_solver_02_xmsin_fwd/solver_02_xmsin_fwd > /dev/null ; echo $?;
#./make/examples/cpp/08_solver_03_xmsin_bwd/solver_03_xmsin_bwd > /dev/null ; echo $?;
./make/examples/cpp/08_solver_04_bvp/solver_04_bvp > /dev/null ; echo $?;
#./make/examples/cpp/08_solver_05_delay/solver_05_delay > /dev/null ; echo $?;
./make/examples/cpp/08_solver_06_bvp_delay/solver_06_bvp_delay > /dev/null ; echo $?;
#./make/examples/cpp/08_solver_07_path_planning/solver_07_path_planning > /dev/null ; echo $?;