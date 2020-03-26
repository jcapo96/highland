#include "baseSelection.hxx"
//#include "ND280AnalysisUtils.hxx"
#include "BasicUtils.hxx"
//#include "CutUtils.hxx"

//********************************************************************
baseSelection::baseSelection(): SelectionBase() {
//********************************************************************  
}

//********************************************************************
void baseSelection::DefineSteps(){
//********************************************************************

  AddStep(StepBase::kCut, "event quality",      new EventQualityCut(),           true);
}

//**************************************************
bool EventQualityCut::Apply(AnaEventC& eventBB, ToyBoxB& box) const{
//**************************************************

  (void)box;

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  if(event.GetIsMC())  return true;              // This is MC, ignore DQ. 

  if (enableDQCut) {
    if(!event.DataQuality->GoodDaq ) return false;  // Bad ND280 DQ
  }

  if (enableBeamQualityCut) {
    if(!event.Beam->GoodSpill ) return false;      // Bad Spill
  }

  return true;
}

//********************************************************************
bool baseSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************
  
  (void)event;
  (void)allCutsPassed;

  return 0;
}
