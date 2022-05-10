#!/bin/bash
# Build datatypes.

# Set up destinations dirs for install
mkdir -p $PREFIX/lib $PREFIX/tests/swift

# test runner script
cp ${RECIPE_DIR}/datatypes_run_tests.sh $PREFIX/tests/swift/

# build everything
mkdir -v -p build
cd build

# needed by xunit scons....
export TERM=
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PREFIX/lib

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install ../datatypes
make -j 2 install
cp -v -r install/lib/* $PREFIX/lib/
cp -v -r install/include/* $PREFIX/include/
rm -rf *

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install ../tests
make -j 2 install
mv -v install/bin/datatypes_tests $PREFIX/tests/swift/
