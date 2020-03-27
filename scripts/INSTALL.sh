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

# set environment variables needed by cmake
source scripts/setup.sh

# create a working directory
if [ -d "build" ]; then
    echo "build folder already exists. You should delete it manually or source cleanup.sh "
    return
fi

mkdir build
cd build

# create the Makefile
cmake ..

# compile  (equivalent to "make" in most systems)
cmake --build .

#install libraries and binaries in the appropriate folder  (equivalent to "make install" in most systems)
cmake --install .

# go to the initial folder
cd ..

# set environment variables that need to run cmake first
source scripts/setup_cmake.sh

