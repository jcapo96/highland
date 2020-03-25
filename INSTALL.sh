# configure the environment variables (including ROOTSYS and CMTPATH)
#source setup.sh


#The latest instruction tells the system where CMT has been installed.
#The CMT version can be changed: i.e. The CMT version can be changed. Have a look inside the CMT folder,
#where several versions are available. You can also download a new one if you wish 

#Go back to the top level folder (the one we created previosuly, the HIGHLAND_PATH)

#and get the HighLAND framework packages from git:

if [ ! -d psyche ]; then    
git clone https://next.ific.uv.es:8888/HighLAND/psychePolicy.git                 psyche/psychePolicy/v0r0
git clone https://next.ific.uv.es:8888/HighLAND/psycheCore.git                   psyche/psycheCore/v0r0
git clone https://next.ific.uv.es:8888/HighLAND/psycheUtils.git                  psyche/psycheUtils/v0r0

git clone https://next.ific.uv.es:8888/duneHighLAND/psycheDUNEUtils.git          psyche/psycheDUNEUtils/v0r0
git clone https://next.ific.uv.es:8888/duneHighLAND/psycheEventModel.git         psyche/psycheEventModel/v0r0
git clone https://next.ific.uv.es:8888/duneHighLAND/psycheIO.git                 psyche/psycheIO/v0r0
git clone https://next.ific.uv.es:8888/duneHighLAND/psycheSelections.git         psyche/psycheSelections/v0r0

git clone https://next.ific.uv.es:8888/HighLAND/highlandCore.git                 highland2/highlandCore/v0r0
git clone https://next.ific.uv.es:8888/HighLAND/highlandTools.git                highland2/highlandTools/v0r0
git clone https://next.ific.uv.es:8888/HighLAND/highlandDoc.git                  highland2/highlandDoc/v0r0

git clone https://next.ific.uv.es:8888/duneHighLAND/highlandEventModel.git       highland2/highlandEventModel/v0r0
git clone https://next.ific.uv.es:8888/duneHighLAND/highlandUtils.git            highland2/highlandUtils/v0r0 
git clone https://next.ific.uv.es:8888/duneHighLAND/highlandCorrections.git      highland2/highlandCorrections/v0r0
git clone https://next.ific.uv.es:8888/duneHighLAND/LArSoftReader.git            highland2/LArSoftReader/v0r0
git clone https://next.ific.uv.es:8888/duneHighLAND/highlandIO.git               highland2/highlandIO/v0r0
git clone https://next.ific.uv.es:8888/duneHighLAND/baseAnalysis.git             highland2/baseAnalysis/v0r0 
git clone https://next.ific.uv.es:8888/duneHighLAND/duneExampleAnalysis.git      highland2/duneExampleAnalysis/v0r0
git clone https://next.ific.uv.es:8888/duneHighLAND/protoDuneExampleAnalysis.git highland2/protoDuneExampleAnalysis/v0r0
git clone https://next.ific.uv.es:8888/duneHighLAND/pionAnalysis.git             highland2/pionAnalysis/v0r0
fi

#As you can see there are two groups of packages, which go either to the psyche or highland2 directories. This distiction is explained in the Wiki page.

#Before compiling we must indicate which are the folders in which CMT should find the code. This is done setting the CMTPATH environment variable.
#Since HighLAND depends on ROOT we should indicate both the path for HighLAND and the path for ROOT. The ROOTSYS environment variable is needed by ROOT, so if it has not been set yet do it by hand

#export ROOTSYS=FOLDER WHERE ROOT include, bin and lib directories are

#As explained in root_path page there are several ways of telling CMT where ROOT is, but the simplest one is to download from git a CMT package than bundles ROOT:

#git clone https://next.ific.uv.es:8888/sw/ROOT_CMT.git ROOT/v0r0

#and then specify a single path for CMT, which corresponds to HIGHLAND_PATH, or since we are already there, just type

#source setup.sh
#export CMTPATH=$PWD

#Then go inside the cmt folder of the top level package
# (substitute protoDuneExampleAnalysis by duneExampleAnalysis if you one to do analysis in the 10 kton detector)

source scripts/setup.sh
mkdir build
cd build
cmake ..
make
make install
cd ..

source scripts/setup_cmake.sh
