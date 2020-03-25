#include "protoDuneSelectionUtils.hxx"
#include "EventBoxDUNE.hxx"
#include "TSpline.h"


//data for range-momentum conversion, muons
//http://pdg.lbl.gov/2012/AtomicNuclearProperties/MUON_ELOSS_TABLES/muonloss_289.pdf divided by LAr density for cm

float Range_grampercm[29] = {
  9.833E-1/1.396, 1.786E0/1.396, 3.321E0/1.396, 6.598E0/1.396, 1.058E1/1.396, 3.084E1/1.396, 4.250E1/1.396, 6.732E1/1.396,
  1.063E2/1.396,  1.725E2/1.396, 2.385E2/1.396, 4.934E2/1.396, 6.163E2/1.396, 8.552E2/1.396, 1.202E3/1.396, 1.758E3/1.396,
  2.297E3/1.396,  4.359E3/1.396, 5.354E3/1.396, 7.298E3/1.396, 1.013E4/1.396, 1.469E4/1.396, 1.910E4/1.396, 3.558E4/1.396,
  4.326E4/1.396,  5.768E4/1.396, 7.734E4/1.396, 1.060E5/1.396, 1.307E5/1.396};

float KE_MeV[29]= {
  10,    14,    20,    30,    40,     80,     100,    140,    200,   300,
  400,   800,   1000,  1400,  2000,   3000,   4000,   8000,   10000, 14000,
  20000, 30000, 40000, 80000, 100000, 140000, 200000, 300000, 400000};

//data for range-momentum conversion, protons

double Range_gpercm_P_Nist[31]={
  1.887E-1/1.396, 3.823E-1/1.396, 6.335E-1/1.396, 1.296/1.396,   2.159/1.396,   7.375/1.396,   1.092E1/1.396, 2.215E1/1.396,
  3.627E1/1.396,  5.282E1/1.396,  7.144E1/1.396,  9.184E1/1.396, 1.138E2/1.396, 1.370E2/1.396, 1.614E2/1.396, 1.869E2/1.396, 
  2.132E2/1.396,  2.403E2/1.396,  2.681E2/1.396,  2.965E2/1.396, 3.254E2/1.396, 3.548E2/1.396, 3.846E2/1.396, 4.148E2/1.396, 
  4.454E2/1.396,  7.626E2/1.396,  1.090E3/1.396,  1.418E3/1.396, 1.745E3/1.396, 2.391E3/1.396, 3.022E3/1.396};

double KE_MeV_P_Nist[31]={
  10,   15,   20,   30,   40,   80,   100, 150,
  200,  250,  300,  350,  400,  450,  500, 550, 
  600,  650,  700,  750,  800,  850,  900, 950, 
  1000, 1500, 2000, 2500, 3000, 4000, 5000};

TGraph const KEvsR(29, Range_grampercm, KE_MeV);
TSpline3 const KEvsR_spline3("KEvsRS", &KEvsR);

TGraph const RvsKE(29, KE_MeV, Range_grampercm);
TSpline3 const RvsKE_spline3("RvsKES", &RvsKE);

TGraph const RvsKE_P(31, KE_MeV_P_Nist, Range_gpercm_P_Nist);
TSpline3 const RvsKE_P_spline3("RvsKE_P_S", &RvsKE_P);


//*****************************************************************************
Float_t protoDuneSelUtils::ComputeRangeMomentum(double trkrange, int pdg){
//*****************************************************************************
  
  /* Muon range-momentum tables from CSDA (Argon density = 1.4 g/cm^3)
     website:
     http://pdg.lbl.gov/2012/AtomicNuclearProperties/MUON_ELOSS_TABLES/muonloss_289.pdf
     
     CSDA table values:
     float Range_grampercm[30] = {9.833E-1, 1.786E0, 3.321E0,
     6.598E0, 1.058E1, 3.084E1, 4.250E1, 6.732E1, 1.063E2, 1.725E2,
     2.385E2, 4.934E2, 6.163E2, 8.552E2, 1.202E3, 1.758E3, 2.297E3,
     4.359E3, 5.354E3, 7.298E3, 1.013E4, 1.469E4, 1.910E4, 3.558E4,
     4.326E4, 5.768E4, 7.734E4, 1.060E5, 1.307E5}; float KE_MeV[30] = {10, 14,
     20, 30, 40, 80, 100, 140, 200, 300, 400, 800, 1000, 1400, 2000, 3000,
     4000, 8000, 10000, 14000, 20000, 30000, 40000, 80000, 100000, 140000,
     200000, 300000, 400000};
     
     Functions below are obtained by fitting polynomial fits to KE_MeV vs
     Range (cm) graph. A better fit was obtained by splitting the graph into
     two: Below range<=200cm,a polynomial of power 4 was a good fit; above
     200cm, a polynomial of power 6 was a good fit
     
     Fit errors for future purposes:
     Below 200cm, Forpoly4 fit: p0 err=1.38533;p1 err=0.209626; p2
     err=0.00650077; p3 err=6.42207E-5; p4 err=1.94893E-7; Above 200cm,
     Forpoly6 fit: p0 err=5.24743;p1 err=0.0176229; p2 err=1.6263E-5; p3
     err=5.9155E-9; p4 err=9.71709E-13; p5 err=7.22381E-17;p6
     err=1.9709E-21;*/
  
  //*********For muon, the calculations are valid up to 1.91E4 cm range
  //corresponding to a Muon KE of 40 GeV**********//
  
  /*Proton range-momentum tables from CSDA (Argon density = 1.4 g/cm^3):
    website: https://physics.nist.gov/PhysRefData/Star/Text/PSTAR.html
    
    CSDA values:
    double KE_MeV_P_Nist[31]={10, 15, 20, 30, 40, 80, 100, 150, 200, 250, 300,
    350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000,
    1500, 2000, 2500, 3000, 4000, 5000};
    
    double Range_gpercm_P_Nist[31]={1.887E-1,3.823E-1, 6.335E-1, 1.296,
    2.159, 7.375, 1.092E1, 2.215E1, 3.627E1, 5.282E1, 7.144E1,
    9.184E1, 1.138E2, 1.370E2, 1.614E2, 1.869E2, 2.132E2, 2.403E2,
    2.681E2, 2.965E2, 3.254E2, 3.548E2, 3.846E2, 4.148E2, 4.454E2,
    7.626E2, 1.090E3, 1.418E3, 1.745E3, 2.391E3, 3.022E3};
    
    Functions below are obtained by fitting power and polynomial fits to
    KE_MeV vs Range (cm) graph. A better fit was obtained by splitting the
    graph into two: Below range<=80cm,a a*(x^b) was a good fit; above 80cm, a
    polynomial of power 6 was a good fit
    
    Fit errors for future purposes:
    For power function fit: a=0.388873; and b=0.00347075
    Forpoly6 fit: p0 err=3.49729;p1 err=0.0487859; p2 err=0.000225834; p3
    err=4.45542E-7; p4 err=4.16428E-10; p5 err=1.81679E-13;p6
    err=2.96958E-17;*/
  
  //*********For proton, the calculations are valid up to 3.022E3 cm range
  //corresponding to a Muon KE of 5 GeV**********//
  
  if (trkrange < 0 || std::isnan(trkrange)) {
    std::cout << "TrackMomentumCalculator   " 
              << "Invalid track range " << trkrange << " return -1" << std::endl;
    return -1.;
  }

    
  double KE, Momentum, M;
  constexpr double Muon_M = 105.7, Proton_M = 938.272;
  
  if (abs(pdg) == 13) {
    M = Muon_M;
    KE = KEvsR_spline3.Eval(trkrange);
  } else if (abs(pdg) == 2212) {
    M = Proton_M;
    if (trkrange > 0 && trkrange <= 80)
      KE = 29.9317 * std::pow(trkrange, 0.586304);
    else if (trkrange > 80 && trkrange <= 3.022E3)
      KE =
        149.904 + (3.34146 * trkrange) + (-0.00318856 * trkrange * trkrange) +
        (4.34587E-6 * trkrange * trkrange * trkrange) +
        (-3.18146E-9 * trkrange * trkrange * trkrange * trkrange) +
        (1.17854E-12 * trkrange * trkrange * trkrange * trkrange * trkrange) +
        (-1.71763E-16 * trkrange * trkrange * trkrange * trkrange * trkrange *
         trkrange);
    else
      KE = -999;
  } else
    KE = -999;
  
  if (KE < 0)
    Momentum = -999;
  else
    Momentum = std::sqrt((KE * KE) + (2 * M * KE));
  
  Momentum = Momentum / 1000;
  
  return Momentum;
}

//*****************************************************************************
Float_t protoDuneSelUtils::ComputeCSDARange(double beammom, int pdg){
//*****************************************************************************
    
  if (beammom < 0 || std::isnan(beammom)) {
    //    std::cout << "CSDARangeCalculator   " 
    //              << "Invalid beam mom " << beammom << " return -1" << std::endl;
    return -1.;
  }

  double KE, M, CSDARange;
  constexpr double Muon_M = 105.7, Proton_M = 938.272;
  
  if (abs(pdg) == 13) {
    M = Muon_M;
    KE = sqrt(beammom * beammom + M * M) - M;
    CSDARange = RvsKE_spline3.Eval(KE);
  } else if (abs(pdg) == 2212) {
    M = Proton_M;
    KE = sqrt(beammom * beammom + M * M) - M;
    CSDARange = RvsKE_P_spline3.Eval(KE);
  }
  else 
    CSDARange=-1;
  
  return CSDARange;
}


//********************************************************************
Float_t protoDuneSelUtils::ComputePIDA(const AnaParticle &track) {
//********************************************************************

  Float_t cut=30;

  Float_t PIDA=0;
  Int_t ncontrib=0;
  for (Int_t i=0;i<3;i++){
    for (Int_t j=0;j<track.NHitsPerPlane[i];j++){
      if (track.ResidualRange[i][j]<cut && track.ResidualRange[i][j]>0){
        ncontrib++;
        PIDA += track.dEdx[i][j]*pow(track.ResidualRange[i][j],0.42);
      }
    }
  }
  if (ncontrib>0) PIDA /= ncontrib*1.;

  return PIDA;
}

//********************************************************************
Float_t protoDuneSelUtils::ComputeKineticEnergy(const AnaParticle &part) {
//********************************************************************


  int nhits=part.NHitsPerPlane[0];
  double kinetic=0;
  double res=0;
  for (int i=0;i<nhits;i++){
    double dedxi = part.dEdx_corr[0][i];
//    double dedxi = part.dEdx[0][i];
    double Residualrangei = part.ResidualRange[0][i];
    kinetic = kinetic + dedxi * (Residualrangei - res);
    res = Residualrangei;
  }

  // convert to GeV
  return kinetic/units::GeV;
}

//********************************************************************
Float_t protoDuneSelUtils::ComputeDeDxFromDqDx(Float_t dqdx_adc) {
//********************************************************************
  
  /***************modified box model parameters and function*****************/
  double Rho = 1.383;//g/cm^3 (liquid argon density at a pressure 18.0 psia) 
  double betap = 0.212;//(kV/cm)(g/cm^2)/MeV
  double alpha = 0.93;//parameter from ArgoNeuT experiment at 0.481kV/cm 
  double Wion = 23.6e-6;//parameter from ArgoNeuT experiment at 0.481kV/cm. In MeV/e
  double Efield1=0.50;//kV/cm protoDUNE electric filed
  double beta = betap/(Rho*Efield1); // cm/MeV

  double calib_factor =6.155e-3; //right cali constant for the run 5387. This converts from ADC to e
  double dqdx = dqdx_adc/calib_factor;
  
  // dq/dx should be in e/cm
  // dE/dx is returned in MeV/cm
  
  return (exp(dqdx*beta*Wion)-alpha)/beta;
}

//********************************************************************
Float_t protoDuneSelUtils::ComputeDqDxFromDeDx(Float_t dedx) {
//********************************************************************
  
  /***************modified box model parameters and function*****************/
  double Rho = 1.383;//g/cm^3 (liquid argon density at a pressure 18.0 psia) 
  double betap = 0.212;//(kV/cm)(g/cm^2)/MeV
  double alpha = 0.93;//parameter from ArgoNeuT experiment at 0.481kV/cm 
  double Wion = 23.6e-6;//parameter from ArgoNeuT experiment at 0.481kV/cm
  double Efield1=0.50;//kV/cm protoDUNE electric filed
  double beta = betap/(Rho*Efield1);

  double calib_factor =6.155e-3; //right cali constant for the run 5387. This converts from ADC to e
  
  // dq/dx is returned in ADC
  // dE/dx should be in MeV/cm
  
  return  log(dedx*beta + alpha)/(beta*Wion)*calib_factor;
}



//********************************************************************
Float_t* protoDuneSelUtils::ExtrapolateToZ(AnaParticle* part, Float_t z, Float_t* posz) {
//********************************************************************

  // Get track starting point
  double xi,yi,zi;
  xi = part->PositionStart[0];
  yi = part->PositionStart[1];
  zi = part->PositionStart[2];

  // Get track direction
  double ux, uy, uz;
  ux = part->DirectionStart[0];
  uy = part->DirectionStart[1];
  uz = part->DirectionStart[2];

  // zi = z + n * uz; get n
  double n = (zi-z)/uz;

  // Get position at z plane
  posz[0] = xi-n*ux;
  posz[1] = yi-n*uy;
  posz[2] = zi-n*uz;
  
  //std::cout << posz[0] << " " << posz[1] << " " << posz[2] << std::endl;

  return posz;
}



//********************************************************************
void protoDuneSelUtils::ComputeBinnedDeDx(const AnaParticle* part, Float_t max_resrange, Int_t nbins, Float_t** avg_dedx){  
//********************************************************************

  for (Int_t i=0;i<3;i++){
    for (Int_t j=0;j<nbins;j++){
      avg_dedx[i][j]=0;
    }
  }
    
  
  Float_t bin_width = max_resrange/nbins;
  
  for (Int_t i=0;i<3;i++){
    for (Int_t k=0;k<nbins;k++){
      Float_t cut_min = k*bin_width;
      Float_t cut_max = (k+1)*bin_width;      
      Float_t ncontrib=0;
      //      std::cout << "k = " << k << std::endl;
      for (Int_t j=0;j<std::min((Int_t)part->NHitsPerPlane[i],(Int_t)NMAXHITSPERPLANE);j++){
        // a protection against crazy values
        if (part->ResidualRange[i][j]<0.01 || part->dEdx[i][j]<0.01 || part->dEdx[i][j]>100) continue;
        if (part->ResidualRange[i][j]<cut_max && part->ResidualRange[i][j]>cut_min){
          ncontrib++;
          avg_dedx[i][k] +=part->dEdx[i][j];
        }
      }      
      if (ncontrib>0)
        avg_dedx[i][k] /=ncontrib;

    }
  }
  
}

//********************************************************************
AnaTrueParticle* protoDuneSelUtils::FindBeamTrueParticle(const AnaSpillB& spill){  
//********************************************************************


  AnaTrueParticle* beampart=NULL;
  
  AnaBeam* beam         = static_cast<AnaBeam*>(spill.Beam);
  AnaParticleMomB* beamPart = beam->BeamParticle;

  Float_t beammom=0;
  if (beamPart){
    if (beamPart->TrueObject){
      beammom = static_cast<AnaTrueParticleB*>(beamPart->TrueObject)->Momentum;
    }
  }
  
  if (spill.TrueParticles.size() > 0){
    for (UInt_t i =0; i< spill.TrueParticles.size();i++){
      if (beammom == spill.TrueParticles[i]->Momentum){
        beampart = static_cast<AnaTrueParticle*> (spill.TrueParticles[i]);
        break;
      }
    }
  }

  return beampart;

}


//********************************************************************
void protoDuneSelUtils::AddParticles(AnaParticle* part1, AnaParticle* part2){  
//********************************************************************

  part1->Length     += part2->Length;
  part1->NHits      += part2->NHits;
  
  anaUtils::CopyArray(part2->DirectionEnd,    part1->DirectionEnd,   3);
  anaUtils::CopyArray(part2->PositionEnd,     part1->PositionEnd,    4);
  
  AnaParticle* part1c = part1->Clone();
    
    
  for (Int_t i=0;i<3;i++){
    part1->NHitsPerPlane[i] += part2->NHitsPerPlane[i];

    Int_t last_hit=0;
    for (Int_t j=0;j<std::min((Int_t)NMAXHITSPERPLANE,part2->NHitsPerPlane[i]);j++){
      Int_t offset = 0;
      part1->dEdx[i]         [j+offset]=part2->dEdx[i][j];
      part1->dQdx[i]         [j+offset]=part2->dQdx[i][j];
      part1->dEdx_corr[i]    [j+offset]=part2->dEdx_corr[i][j];
      part1->dQdx_corr[i]    [j+offset]=part2->dQdx_corr[i][j];
      part1->HitX[i]         [j+offset]=part2->HitX[i][j];
      part1->ResidualRange[i][j+offset]=part2->ResidualRange[i][j];
      last_hit=j;
    }

    for (Int_t j=0;j<std::min((Int_t)part1->NHitsPerPlane[i],(Int_t)NMAXHITSPERPLANE-part2->NHitsPerPlane[i]);j++){
      Int_t offset = part1->NHitsPerPlane[i];
      part1->dEdx[i]         [j+offset]=part1c->dEdx[i][j];
      part1->dQdx[i]         [j+offset]=part1c->dQdx[i][j];
      part1->dEdx_corr[i]    [j+offset]=part1c->dEdx_corr[i][j];
      part1->dQdx_corr[i]    [j+offset]=part1c->dQdx_corr[i][j];
      part1->HitX[i]         [j+offset]=part1c->HitX[i][j];
      part1->ResidualRange[i][j+offset]=part1c->ResidualRange[i][j]+part2->ResidualRange[i][last_hit];
    }    
  }

  delete part1c;
  
  for (int i=0; i<3; i++) {
    part1->PIDA[i]     = part2->PIDA[i];
    part1->ReconPDG[i] = part2->ReconPDG[i];

    for (int j=0; j<10; j++) {
      part1->PID[i][j]   = part2->PID[i][j];
      part1->CALO[i][j] += part2->CALO[i][j];
    }
  }

  part1->RangeMomentum[0] = protoDuneSelUtils::ComputeRangeMomentum(part1->Length, 13);
  part1->RangeMomentum[1] = protoDuneSelUtils::ComputeRangeMomentum(part1->Length, 2212);

  // A pointer to the original particle
  //    Original = &part;
  
  
  
  //  part1->TrueEff       = part->TrueEff;
  //  part1->TruePur       = part->TruePur;
  
  
  //  AveragedQdx   = part->AveragedQdx;
  //  AveragedEdx   = part->AveragedEdx;
  //  MomentumError = part->MomentumError;

  // TODO: just to mark it as a broken track
  part1->NDOF       = 8888;
  //  Chi2          = part->Chi2;
  //  FitPDG        = part->FitPDG;
  //  Bunch         = part->Bunch;

  
  // TODO
  //  Daughters.clear();
  //  for (UInt_t i=0;i<part.Daughters.size();i++){
  //    Daughters.push_back(part.Daughters[i]->Clone());
  //  }

}

//********************************************************************
std::vector<double> protoDuneSelUtils::ComputeDistanceToVertex(AnaParticle* part){
//********************************************************************

  std::vector<double> distance;
  double dis1 = 0, dis2 = 0;
  if(part->Daughters.empty()) return distance;
  else {
    for(int i = 0; i < (int)part->Daughters.size(); i++){
      AnaParticle* dau = static_cast<AnaParticle*>(part->Daughters[i]);
      //compute distance between part and daughter. Assume dau can be reconstructed backwards
      dis1 = sqrt(pow(part->PositionEnd[0]-dau->PositionStart[0],2)+pow(part->PositionEnd[1]-dau->PositionStart[1],2)+pow(part->PositionEnd[2]-dau->PositionStart[2],2));
      dis2 = sqrt(pow(part->PositionEnd[0]-dau->PositionEnd[0],2)+pow(part->PositionEnd[1]-dau->PositionEnd[1],2)+pow(part->PositionEnd[2]-dau->PositionEnd[2],2));
      //return lower value
      if(dis1<dis2)distance.push_back(dis1);
      else distance.push_back(dis2);
    }

    return distance;
  }
}
