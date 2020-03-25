export MAKEFLAGS=-j8 QUICK=1

if [[ ${HOSTNAME} =~ "neutrinos" ]] ; then
   export ROOTSYS=/data4/NEXT/software/scisoft/root/v6_18_04/Linux64bit+2.6-2.12-e17-prof
   export PATH=/data4/NEXT/software/scisoft/cmake/v3_14_3/Linux64bit+2.6-2.12/bin:/data4/NEXT/software/scisoft/gcc/v7_3_0/Linux64bit+2.6-2.12/bin:$PATH
   export LD_LIBRARY_PATH=/data4/NEXT/software/scisoft/tbb/v2019_3/Linux64bit+2.6-2.12-e17-prof/lib:/data4/NEXT/software/scisoft/gcc/v7_3_0/Linux64bit+2.6-2.12/lib64:$LD_LIBRARY_PATH
else
   export ROOTSYS=/hep/sw/root-6.12.06/myroot
fi


export CMAKE_MODULE_PATH=$ROOTSYS
export CMAKE_PREFIX_PATH=$ROOTSYS
export HIGHLAND_PACKAGE_HIERARCHY=pionAnalysis:baseAnalysis:highlandTools:psycheSelections:highlandIO:highlandCorrections:highlandUtils:LArSoftReader:highlandEventModel:highlandCore:psycheIO:psycheDUNEUtils:psycheUtils:psycheEventModel:psycheCore
