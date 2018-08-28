#!/bin/bash

printf "Unit tests\n"
./make/tests/core/tubelib_tests_core;

printf "Basics\n"
./make/examples/basics/01_arithmetic/basics_01_arithmetic > /dev/null ; echo $?;
./make/examples/basics/02_simple_ctc/basics_02_simple_ctc > /dev/null ; echo $?;
./make/examples/basics/03_ivp_msinx/basics_03_ivp_msinx > /dev/null ; echo $?;
./make/examples/basics/04_lissajous/basics_04_lissajous > /dev/null ; echo $?;

printf "\nSolver\n"
./make/examples/solver/01_picard/solver_01_picard > /dev/null ; echo $?;
./make/examples/solver/04_bvp/solver_04_bvp > /dev/null ; echo $?;
./make/examples/solver/06_bvp_delay/solver_06_bvp_delay > /dev/null ; echo $?;

printf "\nRobotics\n"
./make/examples/robotics/01_causal_chain/rob_01_causal_chain > /dev/null ; echo $?;
./make/examples/robotics/02_lowcost_beacons/rob_02_lowcost_beacons > /dev/null ; echo $?;
./make/examples/robotics/03_drifting_clock/rob_03_drifting_clock > /dev/null ; echo $?;
./make/examples/robotics/05_data_association/rob_05_data_association > /dev/null ; echo $?;