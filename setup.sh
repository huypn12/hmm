#!/bin/bash

git clone https://github.com/huypn12/hmm.git
cd hmm
cmake -H. -DCMAKE_BUILD_TYPE=Release -Bcmake-build-release -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
cd cmake-build-release
make
