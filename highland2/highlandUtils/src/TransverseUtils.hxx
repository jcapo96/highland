#ifndef TransverseUtils_h
#define TransverseUtils_h

#include "DetectorDefinition.hxx"
#include <TMath.h>
#include <TVector3.h>


namespace DetDef{

  //in m
  extern Float_t nd280BeamPipe[3];
  extern Float_t nd280NeutrinoParentDecay[3];

}


namespace anaUtils{

  // ===>
  // code inside this region is from xianguo. method for calculating the reconstructed neutrino direction
  //  void SetNeutrinoParentDecPointRec(TVector3 &vec);
  //  TVector3 CalcNuDir(TVector3 &nup0Global, TVector3 &nup1Local);
  //  TVector3 GetNuDirRec(const Float_t vertex_pos[]);
  //<============

}

#endif
