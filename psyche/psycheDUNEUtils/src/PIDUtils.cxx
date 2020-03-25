#include "PIDUtils.hxx"
#include "CutUtils.hxx"
#include "VersioningUtils.hxx"

const unsigned int NMAXPARTICLESWITHSUBDET2 = NMAXPARTICLES;
  
//********************************************************************
void anaUtils::RecomputeSubdet2Pulls(AnaSubdet2ParticleB &track) {
//********************************************************************

    // TODO
    (void)track;

    /*
       track.Pullmu  = ComputeSubdet2Pull(track,"muon");
       track.Pullp   = ComputeSubdet2Pull(track,"proton");
       track.Pullele = ComputeSubdet2Pull(track,"electron");
       track.Pullpi  = ComputeSubdet2Pull(track,"pion");
       track.Pullk   = ComputeSubdet2Pull(track,"kaon");
       */
}

//********************************************************************
void anaUtils::ComputeSubdet2Pull(const AnaSubdet2ParticleB &track, Float_t* pulls) {
//********************************************************************

    pulls[0] = ((track.dEdxMeas - track.dEdxexpMuon) / track.dEdxSigmaMuon);
    pulls[1] = ((track.dEdxMeas - track.dEdxexpEle) / track.dEdxSigmaEle);
    pulls[2] = ((track.dEdxMeas - track.dEdxexpProton) / track.dEdxSigmaProton);
    pulls[3] = ((track.dEdxMeas - track.dEdxexpPion) / track.dEdxSigmaPion);
    
	
}

//********************************************************************
Float_t anaUtils::ExpecteddEdx(const AnaParticleMomB& part, ParticleId::ParticleEnum partID) {
//********************************************************************

  return ExpecteddEdx(part.Momentum,partID);
}

//********************************************************************
Float_t anaUtils::ExpecteddEdx(Float_t mom, ParticleId::ParticleEnum partID) {
//********************************************************************

    // for production 5
    Float_t ExpecteddEP0 = 149.4;
    Float_t ExpecteddEP1 = 2.765;
    Float_t ExpecteddEP2 = 0.103;
    Float_t ExpecteddEP3 = 2.052;
    Float_t ExpecteddEP4 = 0.5104;

    if (versionUtils::prod6_corrections){
      // for production 6
      ExpecteddEP0 = 53.87;
      ExpecteddEP1 = 5.551;
      ExpecteddEP2 = 0.001913;
      ExpecteddEP3 = 2.283;
      ExpecteddEP4 = 1.249;
    }

    Float_t mass = GetParticleMass(partID);
    if (mass<0){
      std::cout << "Tried to compute dEdx for invalid particle hypothesis" << std::endl;
      return -10000;
    }

    Float_t bg = mom / mass;
    Float_t beta = bg / sqrt(1. + bg * bg);
    Float_t func = ExpecteddEP1 - pow(beta, ExpecteddEP3) - log(ExpecteddEP2 + 1. / pow(bg, ExpecteddEP4));
    func = func * ExpecteddEP0 / pow(beta, ExpecteddEP3);

    return func;
}

//********************************************************************
void anaUtils::GetPIDLikelihood(const AnaTrackB& track, Float_t* hypo){
//********************************************************************

    UInt_t itrk = track.Index;

    if( itrk >= NMAXPARTICLESWITHSUBDET2 ) return; // Protection against values out of the vector. 

    Double_t prob[4]={1,1,1,1};
    Double_t tmp_prob[3][4];
    Double_t total_prob=0;
    bool found=false;

    AnaSubdet2ParticleB* segmentsInSubdet2[3];
    for(int i = 0; i < 3; ++i){
        segmentsInSubdet2[i] = NULL;
        for (Int_t j=0;j<4;j++){
            hypo[j]=-1;
            tmp_prob[i][j] = 1;
        }
    }
    // Get the closest Subdet2. We should make sure that at least the segment in that Subdet2 has the proper PID info
    SubDetId::SubDetEnum closesttpc = anaUtils::GetClosestSubdet2(track);

    // Loop over Subdet2 segments
    for (int j = 0; j < track.nSubdet2Segments; ++j){      
        AnaSubdet2ParticleB* Subdet2Segment = track.Subdet2Segments[j];
        if (!Subdet2Segment) continue;

        // Only segments passing the Subdet2 track quality cut will contribute to the likelihood
        // Was not applied for production 5 BANFF analysis
        //        if(!prod5Cut) if (!cutUtils::Subdet2TrackQualityCut(*Subdet2Segment)) continue;

        // Require valid values for all quantities involved
        if( Subdet2Segment->dEdxexpMuon==-0xABCDEF || Subdet2Segment->dEdxexpEle==-0xABCDEF || Subdet2Segment->dEdxexpPion==-0xABCDEF || Subdet2Segment->dEdxexpProton==-0xABCDEF) continue;
        if( Subdet2Segment->dEdxMeas ==-0xABCDEF ) continue; 
        if( Subdet2Segment->dEdxexpMuon==-99999 || Subdet2Segment->dEdxexpEle==-99999 || Subdet2Segment->dEdxexpPion==-99999 || Subdet2Segment->dEdxexpProton==-99999) continue;
        if( Subdet2Segment->dEdxMeas ==-99999 ) continue; 

        Float_t pulls[4];
        // Pulls in order: Muon, Electron, Proton, Pion
        anaUtils::ComputeSubdet2Pull(*Subdet2Segment,pulls);
        Float_t pullmu  = pulls[0];
        Float_t pullp   = pulls[2];
        Float_t pullele = pulls[1];
        Float_t pullpi  = pulls[3];
	
        if (!TMath::Finite(pullmu) || !TMath::Finite(pullele) || !TMath::Finite(pullp) || !TMath::Finite(pullpi)) continue;
        if (pullmu  != pullmu || pullele != pullele || pullp   != pullp || pullpi  != pullpi) continue;

        SubDetId::SubDetEnum det = SubDetId::GetSubdetectorEnum(Subdet2Segment->Detector);
        
        // To avoid mismatching between FlatTree and oaAnalysis we allow only one segment per Subdet2 to be included in the likelihood, the one with more nodes
        if (segmentsInSubdet2[det-2]){
            if (Subdet2Segment->NHits > segmentsInSubdet2[det-2]->NHits){
                segmentsInSubdet2[det-2] = Subdet2Segment;
                tmp_prob[det-2][0] = exp(-(pullmu*pullmu)/2);
                tmp_prob[det-2][1] = exp(-(pullele*pullele)/2);
                tmp_prob[det-2][2] = exp(-(pullp*pullp)/2);
                tmp_prob[det-2][3] = exp(-(pullpi*pullpi)/2);
            }            
        }
        else{
            segmentsInSubdet2[det-2] = Subdet2Segment;      
            tmp_prob[det-2][0] = exp(-(pullmu*pullmu)/2);  
            tmp_prob[det-2][1] = exp(-(pullele*pullele)/2);
            tmp_prob[det-2][2] = exp(-(pullp*pullp)/2);    
            tmp_prob[det-2][3] = exp(-(pullpi*pullpi)/2);  
        }
    }

    // Loop over all segments contributing to the likelihood and compute it
    for (int tpc=0;tpc<3;tpc++){
        if (segmentsInSubdet2[tpc]){
            // The pull should be already corrected by all corrections (CT and CT expected)
            prob[0] *= tmp_prob[tpc][0]; 
            prob[1] *= tmp_prob[tpc][1];
            prob[2] *= tmp_prob[tpc][2];
            prob[3] *= tmp_prob[tpc][3];

            if (SubDetId::GetDetectorUsed(segmentsInSubdet2[tpc]->Detector, closesttpc)) found = true;
        }
    }

    // If at least the segment in the closest Subdet2 has a  valid PID info
    if (found){
        for (int h=0;h<4;h++){
            total_prob += prob[h] ;
        }

        if (total_prob>0){
            for (int h=0;h<4;h++){
                hypo[h] = prob[h]/total_prob ;
            }
        }
    }
    return;
}

//********************************************************************
Float_t anaUtils::GetPIDLikelihood(const AnaTrackB& track, Int_t hypo){
//********************************************************************

    if( hypo >= 4 ) return -1.e+6; 

    Float_t Likelihood[4];
    GetPIDLikelihood(track,Likelihood);
    return Likelihood[hypo];
}

//********************************************************************
Float_t anaUtils::GetPIDLikelihoodMIP(const AnaTrackB &track) {
//********************************************************************

    Float_t Likelihood[4];
    GetPIDLikelihood(track,Likelihood);

    Float_t likemu = Likelihood[0];
    Float_t likepi = Likelihood[3];
    Float_t likep  = Likelihood[2];

    return (likemu+likepi)/(1-likep); 
}

//********************************************************************
Float_t anaUtils::GetPIDPrior(const AnaTrackB& track, Int_t hypo){
//********************************************************************

    // This function is not used yet

    Float_t xbins[18] = {0., 100., 200., 300., 400., 500., 600., 700, 800, 1000., 1200., 1400., 1700, 2000., 2500., 3000., 4000., 5000.};

    Float_t eprior[17] = {800., 250., 100,  40,    30,  25,   10,    5,    0,   0,     0,     0,     0,     0,     0,     0,     0}; 

    for (Int_t i=0;i<17;i++){
        eprior[i] /=400.;
    }

    Int_t pbin = 16;
    for (Int_t i=0;i<17;i++){
        pbin = i-1;
        if (track.Momentum>0 && track.Momentum < xbins[i]) break;
    }


    if (hypo==1)  return eprior[pbin];
    else return 1.;

}
