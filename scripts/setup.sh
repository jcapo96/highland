export MAKEFLAGS=-j8 QUICK=1

# automatically find the highland source directory
export HIGHLANDPATH="$(cd -P -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd -P)"
echo "HIGHLANDPATH=${HIGHLANDPATH}"

if [[ ${HOSTNAME} =~ "neutrinos" ]] ; then
    # environment for IFIC's machines
    products_dir=/data4/NEXT/software/scisoft

    gcc_version=v7_3_0         
    root_version=v6_18_04         
    tbb_version=v2019_3
    cmake_version=v3_14_3
    
    gcc_compiler=Linux64bit+2.6-2.12
    root_compiler=Linux64bit+2.6-2.12-e17-prof                
    tbb_compiler=Linux64bit+2.6-2.12-e17-prof
    cmake_compiler=Linux64bit+2.6-2.12

elif [[ -e /cvmfs/larsoft.opensciencegrid.org/products ]] ; then
    # environment for CERN and FERMILAB's machines
    products_dir=/cvmfs/larsoft.opensciencegrid.org/products

    gcc_version=v9_3_0         
    root_version=v6_22_08d         
    tbb_version=v2021_1_1
    cmake_version=v3_22_2
    
    gcc_compiler=Linux64bit+3.10-2.17
    root_compiler=Linux64bit+3.10-2.17-e20-p392-prof
    tbb_compiler=Linux64bit+3.10-2.17-e20
    cmake_compiler=Linux64bit+3.10-2.17
elif [[ ${HOSTNAME} =~ "portcervera.ific.uv.es" ||  ${HOSTNAME} =~ "Anselmo-Cerveras-MacBook-Pro-II.local" ]]; then
    # environment for Anselmo's laptop
    root_dir=/hep/sw/root-6.12.06/myroot
else
    echo "ENVIRONMENT NOT AVAILABLE FOR THIS MACHINE !!! ADD APPROPRIATE ENVIRONMENT TO scripts/setup.sh" 
    return
fi

if [[ x$products_dir != x ]] && [[ -z "$TBBROOT" ]] ; then
    export gcc_dir=$products_dir/gcc/$gcc_version/$gcc_compiler
    export tbb_dir=$products_dir/tbb/$tbb_version/$tbb_compiler
    export root_dir=$products_dir/root/$root_version/$root_compiler
    export cmake_dir=$products_dir/cmake/$cmake_version/$cmake_compiler

    export LD_LIBRARY_PATH=$gcc_dir/lib64:$gcc_dir/lib:$tbb_dir/lib:$LD_LIBRARY_PATH         
    export PATH=$cmake_dir/bin/:$gcc_dir/bin/:$PATH
fi

if [[ -z "$ROOTSYS" ]]; then
    # tell the system where ROOT is
    export ROOTSYS=$root_dir
    export PATH=$ROOTSYS/bin:$PATH    
    export DYLD_LIBRARY_PATH=$ROOTSYS/lib:$DYLD_LIBRARY_PATH
    export   LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH

    
    # necessary for root to find headers in dictionaries 
    # (see https://root.cern/doc/v618/release-notes.html#header-location-and-root_generate_dictionary-root_standard_library_package)
    export ROOT_INCLUDE_PATH=$HIGHLANDPATH/include
fi


export PATH=$HIGHLANDPATH/bin:$PATH    
export DYLD_LIBRARY_PATH=$HIGHLANDPATH/lib:$DYLD_LIBRARY_PATH
export   LD_LIBRARY_PATH=$HIGHLANDPATH/lib:$LD_LIBRARY_PATH


export CMAKE_PREFIX_PATH=$HIGHLANDPATH:$ROOTSYS

#export HIGHLAND_PACKAGE_HIERARCHY=baseAnalysis:highlandTools:psycheSelections:highlandIO:highlandCorrections:highlandUtils:highlandEventModel:highlandCore:psycheIO:psycheDUNEUtils:psycheUtils:psycheEventModel:psycheCore
export HIGHLAND_PACKAGE_HIERARCHY=psycheCore


#------ set the ROOT folder for every package in highland  ------
D0=$PWD
cd $HIGHLANDPATH/src
for D1 in {psyche,highland2}
do
  if [ ! -d $D1 ]; then
    continue
  fi
  cd $D1
  for D2 in *
  do
    if [ ! -d $D2 ]; then
      continue
    fi
    D3=$(echo $D2 | tr "[a-z]" "[A-Z]")
    D4=${D3}ROOT
    export ${D4}=$HIGHLANDPATH/src/$D1/$D2
  done
  cd ..
done
cd $D0

