#!/bin/bash

read -p "Please enter the codac package version (X.Y.Z[.D]) : " PACKAGE_VERSION

PY_V_MAJ=3

for PY_V_MIN in 8 9 10 11 12 13; do
	for CPCFG in "-macosx_10_16_x86_64" "-macosx_11_0_arm64" "-manylinux_2_17_x86_64.manylinux_2_28_x86_64" "-manylinux_2_17_aarch64.manylinux_2_28_aarch64" "-win32" "-win_amd64"; do
	 rm -Rf codac-$PACKAGE_VERSION-cp$PY_V_MAJ$PY_V_MIN-cp$PY_V_MAJ$PY_V_MIN$CPCFG.whl ; wget https://github.com/codac-team/codac/releases/download/v$PACKAGE_VERSION/codac-$PACKAGE_VERSION-cp$PY_V_MAJ$PY_V_MIN-cp$PY_V_MAJ$PY_V_MIN$CPCFG.whl --no-check-certificate -nv
	done
done
