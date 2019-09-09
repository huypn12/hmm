# Hidden Markov Model

## 1. Introduction
Implementation of Hidden Markov Model and inference algorithms.
This project uses C++17 with Eigen3 (linear algebra) and Catch2 (unit test).
All external libraries are header only, thus simplify the build process.

## 2. Build 
### 2.1 Prerequisites
Only basic build tools are needed.
  * CMake >= 3.3 
  * GCC >= 6.0 or Clang >= 3.6 (for C++17 support)
  * make

### 2.2 Build
```shell
sh -c "$(wget -O- https://raw.githubusercontent.com/huypn12/hmm/devel/setup.sh)"
```
Unit test
```shell
make test
```
Build result *libhmm.a* is used as a static library.
