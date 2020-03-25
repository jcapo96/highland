#ifndef SampleId_hxx
#define SampleId_hxx

#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>


/// Event sample - whether it passes the NuMu-CCQE selection, or CC1Pi selection for example
namespace SampleId{

  
  enum SampleEnum {
    kUnassigned,
    kDUNE_NuMuCC,
    kProtoDUNE_SP_Elec,
    kProto35t_Muon,    
    kNSamples
  };

  std::string ConvertSample(SampleEnum sample);
  std::string ConvertSampleToSelection(SampleEnum sample);
  
}

#endif
