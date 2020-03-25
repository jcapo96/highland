#include "DetectorDefinition.hxx"
#include "Units.hxx"
#include <iomanip>

//dumping of array
#define dump_array3(par) std::cout << std::fixed    << \
  std::setw(10) << std::left   << #par   << ": \t"  << \
  std::setw(10) << std::right  << par[0] << " "     << \
  std::setw(10) << std::right  << par[1] << " "     << \
  std::setw(10) << std::right  << par[2] << std::endl;

#define dump_var(par)  std::cout << std::fixed << std::setw(20) << std::left << #par << ":\t" << \
  std::setw(10) << std::right << par << std::endl;

namespace DetDef {

  // Detector definitions
  // Can be overridden in yourAnalysis::Initialize()
  // with e.g. DetDef::Subdet1_1min[0] = x;
  // (note that DetDef::Subdet1_1min = {x,y,z} works only with old compilers)
  // Fiducial volume is defined by removing the edge volumes in FiducialVolumeDefinition.cxx

  Float_t Subdet2_1min[3] = {-1150.00, -1170.0, -885.0};
  Float_t Subdet2_1max[3] = { 1150.00,  1230.0,   89.0};
  Float_t Subdet2_2min[3] = {-1150.00, -1170.0,  474.0};
  Float_t Subdet2_2max[3] = { 1150.00,  1230.0, 1448.0};
  Float_t Subdet2_3min[3] = {-1150.00, -1170.0, 1833.0};
  Float_t Subdet2_3max[3] = { 1150.00,  1230.0, 2807.0};


  // We have to include also skin and glue since mass studies are done on the whole modules (TN 91, 122, 198)
  Float_t Subdet1_1min[3] = {-50, -100, -20};
  Float_t Subdet1_1max[3] = {250,  120, 180};

  Float_t Subdet1_2min[3] = {-365*(Float_t)units::cm, -603*(Float_t)units::cm,  -10*(Float_t)units::cm};
  Float_t Subdet1_2max[3] = { 365*(Float_t)units::cm,  603*(Float_t)units::cm,  470*(Float_t)units::cm};



  //**********************************
  void DumpVolumes(){
  //**********************************
    std::streamsize ss = std::cout.precision();
    std::cout.precision(3);

    std::cout << "\n***** DetectorDefinition: available volumes list: ***** \n" << std::endl;

    dump_array3(Subdet1_1min);
    dump_array3(Subdet1_1max);

    dump_array3(Subdet1_2min);
    dump_array3(Subdet1_2max);

    dump_array3(Subdet2_1min);
    dump_array3(Subdet2_1max);

    dump_array3(Subdet2_2min);
    dump_array3(Subdet2_2max);

    dump_array3(Subdet2_3min);
    dump_array3(Subdet2_3max);


    std::cout << "\n******************************************************* \n" << std::endl;

    std::cout.precision(ss);
  }

} //namespace

