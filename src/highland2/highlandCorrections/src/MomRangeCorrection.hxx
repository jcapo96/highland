#ifndef MomRangeCorrection_h
#define MomRangeCorrection_h

#include "CorrectionBase.hxx"
#include "DataClasses.hxx"
#include "SubDetId.hxx"


class MomRangeCorrection: public CorrectionBase {
	
	public:

		MomRangeCorrection(SubDetId::SubDetEnum det = SubDetId::kSubdet1_1);
		virtual ~MomRangeCorrection(){}

		void Apply(AnaSpillC& spill);
    
  protected:
    bool IsRelevantTrack(const AnaTrackB& track) const; 
    SubDetId::SubDetEnum _det;

};

#endif
