#!/bin/bash

# Initialize and update git submodules
git submodule init
git submodule update

# Build the project
export CXXFLAGS="$CXXFLAGS -I$CONDA_PREFIX/include"
export LDFLAGS="$LDFLAGS -L$CONDA_PREFIX/lib"
$PYTHON -m pip install . --no-deps -vv
