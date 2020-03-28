# check that we are in the correct folder
unset found
if [ -f .git/config ]; then
    if grep highland .git/config  | grep -q 'highland'; then
        found=1
    fi
fi

if [ -z "${found}" ]; then
   echo "you are not in the correct folder !!! "
   return
fi

# create a working directory
if [ -d "build" ]; then
    echo "build folder already exists. You should delete it manually or source cleanup.sh "
    return
fi

# set environment variables
source scripts/setup.sh

# creates the build directory
mkdir build
cd build

# create the Makefile inside the build folder using the CMakeList.txt in the parent folder
cmake ..

# go back to the source folder
cd ..

# compile  (equivalent to "make" in most systems)
cmake --build build

#install libraries and binaries in the appropriate folders  (equivalent to "make install" in most systems)
cmake -P build/cmake_install.cmake 
