#include "HighlandInputManager.hxx"
//#include "BasicUtils.hxx"

//********************************************************************
HighlandInputManager::HighlandInputManager():InputManager(){
//********************************************************************

  _RawSpill       = NULL;

}

//********************************************************************
void HighlandInputManager::Reset(){
//********************************************************************

  InputManager::Reset();
  _RawSpill       = NULL;
}

//*****************************************************************************
void HighlandInputManager::DeleteSpill(){
//*****************************************************************************

  InputManager::DeleteSpill();

  if (_RawSpill)
    delete _RawSpill;

  _RawSpill=NULL;
}

//*****************************************************************************
bool HighlandInputManager::LoadSpill(Long64_t& entry){
//*****************************************************************************

  // Read the RawSpill 
  if (!InputManager::LoadSpill(entry)) return false;
  
  // We can now associate the spill just read with the _RawSpill
  _RawSpill = _CorrectedSpill;

  // Make a copy of the spill
  _CorrectedSpill = _RawSpill->Clone();

  return true;
}

//*****************************************************************************
void HighlandInputManager::ResetSpillToRaw() {
//*****************************************************************************
  if (_Spill) {
    delete _Spill;
    _Spill = _RawSpill->Clone();
  }
}

//*****************************************************************************
//AnaEventC*  HighlandInputManager::MakeEvent() {
//*****************************************************************************

//  return new AnaEventC(GetRawSpill(),GetRawBunch());
//}



