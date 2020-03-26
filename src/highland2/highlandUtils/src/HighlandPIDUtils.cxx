#include "HighlandAnalysisUtils.hxx"
#include <stdio.h>
#include <math.h>

const unsigned int NMAXPARTICLESWITHSUBDET2 = NMAXPARTICLES;

//********************************************************************
void anaUtils::ComputeSubdet2PullIncludingKaon(const AnaSubdet2ParticleB &trackB, Float_t* pulls) {
//********************************************************************

  // cast the track
  const AnaSubdet2Particle& track = *static_cast<const AnaSubdet2Particle*>(&trackB);

  // compute pulls for the standard hypotheses using the method in psyche utils
  anaUtils::ComputeSubdet2Pull(track, pulls);
  
  // compute pull for kaon hypothesis
  pulls[4] = ((track.dEdxMeas - track.dEdxexpKaon) / track.dEdxSigmaKaon);
}

//********************************************************************
void anaUtils::GetPIDLikelihoodIncludingKaon(const AnaTrackB& track, Float_t* hypo) {
//********************************************************************

    UInt_t itrk = track.Index;

    if( itrk >= NMAXPARTICLESWITHSUBDET2 ) return; // Protection against values out of the vector. 

    Double_t prob[5]={1,1,1,1,1};
    Double_t tmp_prob[3][5];
    Double_t total_prob=0;
    bool found=false;

    AnaSubdet2ParticleB* segmentsInSubdet2[3];
    for(int i = 0; i < 3; ++i){
        segmentsInSubdet2[i] = NULL;
        for (Int_t j=0;j<5;j++){
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
        
        // Require valid values for kaon quantities (these are not yet in psyche)
        AnaSubdet2Particle* Subdet2SegmentK = (AnaSubdet2Particle*)Subdet2Segment;
        if( Subdet2SegmentK->dEdxexpKaon==-0xABCDEF || Subdet2SegmentK->dEdxMeas ==-0xABCDEF ) continue;
        if( Subdet2SegmentK->dEdxexpKaon==-99999    || Subdet2SegmentK->dEdxMeas ==-99999    ) continue;
        
        Float_t pulls[5];
        // Pulls in order: Muon, Electron, Proton, Pion, Kaon
        anaUtils::ComputeSubdet2PullIncludingKaon(*Subdet2Segment,pulls);
        Float_t pullmu  = pulls[0];
        Float_t pullele = pulls[1];
        Float_t pullp   = pulls[2];
        Float_t pullpi  = pulls[3];
        Float_t pullka  = pulls[4];

        if (!TMath::Finite(pullmu) || !TMath::Finite(pullele) || !TMath::Finite(pullp) || !TMath::Finite(pullpi) || !TMath::Finite(pullka)) continue;
        if (pullmu != pullmu || pullele != pullele || pullp != pullp || pullpi != pullpi || pullka != pullka) continue;

        SubDetId::SubDetEnum det = SubDetId::GetSubdetectorEnum(Subdet2Segment->Detector);
        
        // To avoid mismatching between FlatTree and oaAnalysis we allow only one segment per Subdet2 to be included in the likelihood, the one with more nodes
        if (segmentsInSubdet2[det-2]){
            if (Subdet2Segment->NHits > segmentsInSubdet2[det-2]->NHits){
                segmentsInSubdet2[det-2] = Subdet2Segment;
                tmp_prob[det-2][0] = exp(-(pullmu*pullmu)/2);
                tmp_prob[det-2][1] = exp(-(pullele*pullele)/2);
                tmp_prob[det-2][2] = exp(-(pullp*pullp)/2);
                tmp_prob[det-2][3] = exp(-(pullpi*pullpi)/2);
                tmp_prob[det-2][4] = exp(-(pullka*pullka)/2);
            }            
        }
        else{
            segmentsInSubdet2[det-2] = Subdet2Segment;      
            tmp_prob[det-2][0] = exp(-(pullmu*pullmu)/2);
            tmp_prob[det-2][1] = exp(-(pullele*pullele)/2);
            tmp_prob[det-2][2] = exp(-(pullp*pullp)/2);
            tmp_prob[det-2][3] = exp(-(pullpi*pullpi)/2);
            tmp_prob[det-2][4] = exp(-(pullka*pullka)/2);
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
            prob[4] *= tmp_prob[tpc][4];

            if (SubDetId::GetDetectorUsed(segmentsInSubdet2[tpc]->Detector, closesttpc)) found = true;
        }
    }

    // If at least the segment in the closest Subdet2 has a  valid PID info
    if (found){
        for (int h=0;h<5;h++){
            total_prob += prob[h];
        }

        if (total_prob>0){
            for (int h=0;h<5;h++){
                hypo[h] = prob[h]/total_prob;
            }
        }
    }
    return;
}

//********************************************************************
Float_t anaUtils::GetPIDLikelihoodIncludingKaon(const AnaTrackB& track, Int_t hypo) {
//********************************************************************

    if( hypo >= 5 ) return -1.e+6; 

    Float_t Likelihood[5];
    GetPIDLikelihoodIncludingKaon(track,Likelihood);
    return Likelihood[hypo];
}

