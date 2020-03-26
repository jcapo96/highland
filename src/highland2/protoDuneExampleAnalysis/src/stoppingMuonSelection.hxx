#ifndef stoppingMuonSelection_h
#define stoppingMuonSelection_h

#include "duneExampleSelection.hxx"
#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxPD.hxx"
#include "EventBoxDUNE.hxx"
#include "EventBoxId.hxx"
#include "SystId.hxx"
#include "SubDetId.hxx"

class stoppingMuonSelection: public SelectionBase{
 public:
  stoppingMuonSelection(bool forceBreak=true);
  virtual ~stoppingMuonSelection(){}

  //---- These are mandatory functions
  void DefineSteps();
  void DefineDetectorFV(); //dummy (not needed for this particular selection)
  ToyBoxB* MakeToyBox(){return new ToyBoxPD();}
  void InitializeEvent(AnaEventC&);

  // These ones are also mandatory, although only used in some cases. A dummy implementation is enough if many cases  
  bool FillEventSummary(AnaEventC&, Int_t*){return false;}
  SampleId_h GetSampleId(){return UNASSIGNEDID;}
  Int_t GetRelevantRecObjectGroupsForSystematic(SystId_h, Int_t* IDs, Int_t) const{ IDs[0] = EventBoxDUNE::kLongTracks;return 1;}
  Int_t GetRelevantTrueObjectGroupsForSystematic(SystId_h, Int_t*, Int_t) const{return 0;}

  //------------------
    
protected:

  Int_t _MuonRangeCutIndex;
  Int_t _MuonRangeStepIndex;
  Int_t _FindMainTrackStepIndex;
  Int_t _TotalMultiplicityCutIndex;
};


/*
class FindBeamTrackAction: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindBeamTrackAction();}
};
*/

class BeamMuonCut: public StepBase{
 public:
  using StepBase::Apply;
  BeamMuonCut(){_electronVeto = (bool)ND::params().GetParameterI("protoDuneExampleAnalysis.ElectronVeto");}
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new BeamMuonCut();}

 protected:
  bool _electronVeto;  
};
/*
class CandidateExistsCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new CandidateExistsCut();}
};
*/

class BeamMuonAngleCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new BeamMuonAngleCut();}
};

class MuonCSDARangeCut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new MuonCSDARangeCut();}
};


class MuonPIDACut: public StepBase{
 public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new MuonPIDACut();}
};


#endif
