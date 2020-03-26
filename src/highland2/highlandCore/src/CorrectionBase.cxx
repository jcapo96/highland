#include "CorrectionBase.hxx"

//***********************************************************
CorrectionBase::CorrectionBase() {
//***********************************************************
  _name = "";
  _enabled = true;
  _appliedInInput = false;
  _index = -1;
  
}

//***********************************************************
CorrectionBase::CorrectionBase(const CorrectionBase& corr){
//***********************************************************
  _name           = corr.Name();
  _enabled        = corr.IsEnabled();
  _appliedInInput = corr.IsAppliedInInput();
  _index          = corr.GetIndex();
}
