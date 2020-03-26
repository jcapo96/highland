#include "MomRangeCorrection.hxx"
#include "CutUtils.hxx"


//#define UseRecPack
#ifdef  UseRecPack
#include "AnaRecPackManager.hxx"
#endif


//********************************************************************
MomRangeCorrection::MomRangeCorrection(SubDetId::SubDetEnum det) :  CorrectionBase(){
  //********************************************************************
  _det = det;
}

//********************************************************************
void MomRangeCorrection::Apply(AnaSpillC& spillBB) {
  //********************************************************************

  AnaSpill& spill = *static_cast<AnaSpill*>(&spillBB);

  for (UInt_t i = 0; i < spill.Bunches.size(); i++) {
    AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
    for (UInt_t j = 0; j < bunch->Particles.size(); j++) {

      AnaTrackB* track = static_cast<AnaTrackB*>(bunch->Particles[j]);
      if (!track) continue;
      
      // Check whether a track is relevant to apply the correction
      if (!IsRelevantTrack(*track)) continue;

#ifdef  UseRecPack
      double mom_range;
      track->RangeMomentumMuon = -999.; 
      if (ND::tman().GetMomentumFromRangeLinear(*track, mom_range, ParticleId::kMuon))
        track->RangeMomentumMuon = (Float_t)mom_range;
#endif

    }
  }
}

//********************************************************************
bool MomRangeCorrection::IsRelevantTrack(const AnaTrackB& track) const{
  //********************************************************************

  if (!SubDetId::GetDetectorUsed(track.Detector, _det)) return false;

  // Should fail TPC track quality cut
  if (cutUtils::TrackQualityCut(track)) return false;

  // Should start or stop in the volume of interest
  if (!anaUtils::InDetVolume(_det, track.PositionStart) && 
      !anaUtils::InDetVolume(_det, track.PositionEnd)) return false;  

  return true;
}

