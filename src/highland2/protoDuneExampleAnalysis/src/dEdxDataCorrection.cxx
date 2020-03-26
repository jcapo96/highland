#include "dEdxDataCorrection.hxx"
#include "DUNEAnalysisUtils.hxx"
#include "DataClasses.hxx"
#include "EventBoxDUNE.hxx"
#include "protoDuneSelectionUtils.hxx"
#include "TFile.h"
#include <cassert>

//#define DEBUG

//********************************************************************
dEdxDataCorrection::dEdxDataCorrection(){
//********************************************************************

  std::string filename = std::string(getenv("PROTODUNEEXAMPLEANALYSISROOT"))+"/data/run_5387_Xcalibration.root";

  // read the file with the calibration histograms
  TFile cali_file(filename.c_str());

  // get the calibration histogram
  cali_factor=static_cast<TH1F*>(cali_file.Get("dqdx_X_correction_hist")->Clone());
  cali_factor->SetDirectory(NULL);
}

//********************************************************************
void dEdxDataCorrection::Apply(AnaSpillC& spillC){
//********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillC);

  // No correction for data
  if (spill.GetIsMC()) return;

  // Loop over all bunches
  for (unsigned int i = 0; i < spill.Bunches.size(); i++) {
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);

    // Loop over all relevant tracks for this variation
    for (UInt_t itrk = 0; itrk<bunch->Particles.size(); itrk++){
      
      AnaParticle* part = static_cast<AnaParticle*>(bunch->Particles[itrk]);
      
      // The un-corrected particle
      const AnaParticle* original = static_cast<const AnaParticle*>(part->Original);

      if (!original) continue; //?


      for (Int_t i=0;i<3;i++){
        for (Int_t j=0;j<std::min((Int_t)NMAXHITSPERPLANE,part->NHitsPerPlane[i]);j++){
          if (part->dQdx[i][j]<0.1) continue;  // Protection against crazy values
          part->dQdx[i][j] = ComputeCalibratedDqDx(original->dQdx[i][j],original->HitX[i][j]);
          part->dEdx[i][j] = protoDuneSelUtils::ComputeDeDxFromDqDx(part->dQdx[i][j]);
          //          std::cout << part->dQdx[i][j] << " " << original->dQdx[i][j] << " " << part->dEdx[i][j] << " " << original->dEdx[i][j] << " " << part->HitX[i][j] << std::endl; 
        }
      }
    }
  }
}

//********************************************************************
Float_t dEdxDataCorrection::ComputeCalibratedDqDx(Float_t prim_dqdx, Float_t prim_hitx) {
//********************************************************************
  
  double normalisation_factor=0.983;//for plane 2

  Int_t bin = cali_factor->FindBin(prim_hitx);
  if (bin<1) bin = 1;
  if (bin>148) bin = 148;
  double Cx=cali_factor->GetBinContent(bin);
  double cali_dqdx=Cx*prim_dqdx*normalisation_factor;

  //  std::cout << Cx << " " << prim_hitx << " " << prim_dqdx << " " << cali_dqdx*calib_factor << " " << prim_dedx << " " << cali_dedx  << std::endl; 
  
  return cali_dqdx;
}

