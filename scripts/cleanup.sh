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

#remove everything that was created during compilation
rm -fr bin
rm -fr lib
rm -fr build
rm -f scripts/setup_cmake.sh
