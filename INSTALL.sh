# set environment variables 
source scripts/setup.sh

# create a working directory
mkdir build
cd build

# create the Makefile
cmake ..

# compile
make

#install libraries and binaries in the appropriate folder
make install

# go to the initial folder
cd ..

# set environment variables that need to run cmake first
source scripts/setup_cmake.sh
