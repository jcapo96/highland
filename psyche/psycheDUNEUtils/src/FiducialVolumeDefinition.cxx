#include <FiducialVolumeDefinition.hxx>
#include <DetectorDefinition.hxx>
#include "Units.hxx"
#include <iomanip>

//dumping of array
#define dump_array3(par) std::cout << std::fixed    << \
  std::setw(10) << std::left   << #par   << ": \t"  << \
  std::setw(10) << std::right  << par[0] << " "     << \
  std::setw(10) << std::right  << par[1] << " "     << \
  std::setw(10) << std::right  << par[2] << std::endl;

namespace FVDef {

  // FV definitions: edges to be removed from the detector definitions in DetectorDefinition.cxx
  // Can be overridden in yourAnalysis::Initialize()
  // with e.g. FVDef::FVdefminSubdet1_1[0] = x;
  // (note that FVDef::FVdefminSubdet1_1 = {x,y,z} works only with old compilers)

  Float_t FVdefminSubdet1_1[3] = {20, 20, 20};
  Float_t FVdefmaxSubdet1_1[3] = {20, 20, 20};

  Float_t FVdefminSubdet1_2[3] = {2*(Float_t)units::cm,2*(Float_t)units::cm,2*(Float_t)units::cm};
  Float_t FVdefmaxSubdet1_2[3] = {2*(Float_t)units::cm,2*(Float_t)units::cm,2*(Float_t)units::cm};


  //**********************************
  void DumpFV(){
  //**********************************
    std::streamsize ss = std::cout.precision();
    std::cout.precision(3);

    std::cout << "\n***** FiducialVolume: available volumes list: ***** \n" << std::endl;

    dump_array3(FVdefminSubdet1_1);
    dump_array3(FVdefmaxSubdet1_1);

    dump_array3(FVdefminSubdet1_2);
    dump_array3(FVdefmaxSubdet1_2);

    std::cout << "\n******************************************************* \n" << std::endl;

    std::cout.precision(ss);
  }

}
