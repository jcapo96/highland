#include "StepBase.hxx"

//****************************************************
StepBase::StepBase(){
//****************************************************

  _title="";
  _type=kUnknown;
  _break=false;
  _nextSteps.clear();
  _branchUniqueIDs.clear();
  _disabledInBranch.clear();
  _index=-1;
}  

//****************************************************
void StepBase::CopyStep(const StepBase& step, Int_t branchID){
//****************************************************

  _title = step.Title();
  _type  = step.Type();
  _break = step.Break();
  _index = step.Index();
  _nextSteps.clear();
  _branchUniqueIDs.clear();
  _disabledInBranch.clear(); 

  if (branchID==-1){
    // Copy everything as it is
    
    _nextSteps       = step.GetNextSteps();

    for (UInt_t i=0;i<step.GetBranchUniqueIDs().size();i++)
      AddBranchUniqueID(step.GetBranchUniqueIDs()[i]);
    
    const std::vector<UInt_t>& IDs = step.GetBranchUniqueIDs();
    for (UInt_t j=0;j<IDs.size();j++){
      if (step.IsDisabledInBranch(IDs[j]))
        DisableInBranch(IDs[j]);
    }
  }
  else{
    // Copy only the specified branch from the input step
    
    Int_t branchIndex = step.BranchIndexForBranchUniqueID(branchID);     
    if (branchIndex>-1) _nextSteps.push_back(step.GetNextSteps()[branchIndex]);
    
    AddBranchUniqueID(branchID);
    if (step.IsDisabledInBranch(branchID))
      DisableInBranch(branchID);
  }
    
}

//****************************************************
StepBase* StepBase::MakeCloneRecursive(Int_t branchID){
//****************************************************

  StepBase* step2 = MakeClone();
  step2->CopyStep(*this, branchID);
  step2->ClearNextSteps();
  if (GetNextSteps().size()==0) return step2;
  if (branchID==-1){
    for (UInt_t i = 0; i < GetNextSteps().size(); ++i){
      step2->AddNextStep(GetNextSteps()[i]->MakeCloneRecursive(branchID),i);
    }
  }
  else{    
    Int_t branchIndex = BranchIndexForBranchUniqueID(branchID); 
    if (branchIndex>-1){
      step2->AddNextStep(GetNextSteps()[branchIndex]->MakeCloneRecursive(branchID),branchIndex);
    }
  }
  return step2;
}


//****************************************************
void StepBase::Configure(StepBase::TypeEnum type, const std::string& title, bool cut_break){
//****************************************************

  // Set the type (cut or action)
  SetType(type);
  
  // Set the title (a nice name to be displayed in plots)
  SetTitle(title);
  
  // Set the cut break (only for cuts)
  if (Type() == StepBase::kCut)
    SetBreak(cut_break);
}

//****************************************************
void StepBase::ValidateStepRecursive() const{
//****************************************************

  if (GetBranchUniqueIDs().size()==0){
    std::cout << "\nERROR. StepBase::ValidateStepRecursive(). Step '" << _title 
              << "' does not belong to any branch. You must use SetBranchAlias after adding the steps in your selection" << std::endl;
    exit(1);
  }  

  for (UInt_t i = 0; i < GetNextSteps().size(); ++i){
    StepBase* step2=GetNextSteps()[i];
    if (step2) step2->ValidateStepRecursive();
  }

}

//****************************************************
bool StepBase::HasBranchUniqueID(UInt_t branchID) const{
//****************************************************

  for (UInt_t j=0;j<_branchUniqueIDs.size();j++){
    if (_branchUniqueIDs[j]==branchID) return true;
  }
  return false;
}

//****************************************************
Int_t StepBase::BranchIndexForBranchUniqueID(UInt_t branchID) const {
//****************************************************

  for (UInt_t i=0;i<_nextSteps.size();i++){
    if (!_nextSteps[i]) continue;
    if (_nextSteps[i]->HasBranchUniqueID(branchID)) return i;
  }
  return -1;
  
}

//****************************************************
void StepBase::AddBranchUniqueID(UInt_t branchID){
//****************************************************

  for (UInt_t i=0;i<_branchUniqueIDs.size();i++){
    if (_branchUniqueIDs[i]==branchID){
      std::cout << "StepBase::AddBranchUniqueID(). branch " << branchID << " added previously !!!!" << std::endl;
      return;
    }
  }
  
  _branchUniqueIDs.push_back(branchID);
  if (_disabledInBranch.size() < branchID+1){ 
    _disabledInBranch.resize(branchID+1);
    for (UInt_t i=0;i<_disabledInBranch.size();i++)
      _disabledInBranch[i]=false;
  }
}

//****************************************************
void StepBase::RemoveBranchUniqueID(UInt_t branchID){
//****************************************************

  for (std::vector<UInt_t>::iterator it =_branchUniqueIDs.begin(); it!=_branchUniqueIDs.end();it++){
    if (*it == branchID){
      _branchUniqueIDs.erase(it);
      break;
    }
  }
}

//***********************************************************
std::string StepBase::ConvertType() const{
//***********************************************************

  if (_type == kCut)
    return "cut";
  else if (_type == kAction)
    return "action";
  else
    return "unknown";
}

//********************************************************************
void StepBase::AddBranches(UInt_t nbranches){
//********************************************************************

  for (UInt_t i=0;i<nbranches;i++)
    _nextSteps.push_back(NULL);         
}

//********************************************************************
void StepBase::AddNextStep(StepBase* step, Int_t branchIndex){
//********************************************************************

  // If there are no steps yet, just add it 
  if (_nextSteps.size()==0)    
    _nextSteps.push_back(step);
  else{
    // by default it is added to all branches 
    if (branchIndex==-1){
      for (UInt_t i=0;i<_nextSteps.size();i++)
        _nextSteps[i] = step;
    }
    else{
      // if the branch is specified add the step to the specific branch
      if (branchIndex>=0 && branchIndex < (Int_t)_nextSteps.size()){
        _nextSteps[branchIndex] = step;
      }
      else
        std::cout << "StepBase::AddNextStep(). branchIndex " << branchIndex << " not defined for step '" << _title << "'" << std::endl;
    }
  }
}

//********************************************************************
void StepBase::RemoveNextStep(Int_t branchIndex){
//********************************************************************

  // Remove steps in all branches
  if (branchIndex==-1){
    for (UInt_t i=0;i<_nextSteps.size();i++)
      RemoveNextStep(i);
  }
  else if (branchIndex<0 || branchIndex >= (Int_t)_nextSteps.size()){
    std::cout << "ERROR. StepBase::RemoveNextStep(). branchIndex " << branchIndex << " not defined for step '" << _title << "'" << std::endl;
    exit(1);
  }

  // if there is only one next step, the vector is emptied, otherwise the corresponding entry in the vector is set to NULL
  // in order to keep the number of branches in the split
  if (_nextSteps.size()==1)
    _nextSteps.clear();
  else
    _nextSteps[branchIndex]=NULL;
}

//********************************************************************
void StepBase::ClearNextSteps(){
//********************************************************************

  _nextSteps.clear();
}

//********************************************************************
void StepBase::RemoveStepsInBranchIDRecursive(Int_t branchID){
//********************************************************************
  
  for (std::vector<StepBase*>::iterator it=_nextSteps.begin();it!=_nextSteps.end();it++){
    
    // Skip if the step is NULL at the split
    if (!*it) continue;

    // Check that the step is in a given branch ID
    if (!(*it)->HasBranchUniqueID(branchID)) continue;

    // Remove the step from the branch (remove the branch ID from the step)
    (*it)->RemoveBranchUniqueID(branchID);

    // If the step does not belong to any branch remove the step from the list of next steps
    if ((*it)->GetBranchUniqueIDs().size()==0){
      _nextSteps.erase(it);
      break;
    }

    // Do the same operation recursively
    (*it)->RemoveStepsInBranchIDRecursive(branchID);
  }
}

//********************************************************************
void StepBase::GetNextSteps(std::vector<StepBase*>& steps, bool withSplits, Int_t branchID) const{
//********************************************************************

  // Get all Steps with or without splits
  if (!withSplits && _nextSteps.size() != 1) return;

  for (std::vector<StepBase*>::const_iterator it=_nextSteps.begin();it!=_nextSteps.end();it++){

    // Skip if the step is NULL at the split
    if (!*it) continue;

    if (branchID!=-1 && !(*it)->HasBranchUniqueID(branchID)) continue;
    
    // Add The next step
    steps.push_back(*it);
    
    // Go recursively to add the childs of the next step
    (*it)->GetNextSteps(steps,withSplits);
  }
}

//********************************************************************
void StepBase::GetBranchSequence(Int_t branchID, std::vector<UInt_t>& branch_seq) const{
//********************************************************************


  if (_nextSteps.size()>1){
    Int_t branchIndex = BranchIndexForBranchUniqueID(branchID);
    if (branchIndex>=0)
      branch_seq.push_back(branchIndex);
  }
    
  for (std::vector<StepBase*>::const_iterator it=_nextSteps.begin();it!=_nextSteps.end();it++){

    // Skip if the step is NULL at the split
    if (!*it) continue;

    if (!(*it)->HasBranchUniqueID(branchID)) continue;

    // Go recursively to add the childs of the next step
    (*it)->GetBranchSequence(branchID, branch_seq);
  }
}



//********************************************************************
StepBase::BranchStatus StepBase::GetNextStepsInBranch(std::vector<StepBase*>& steps, int branchIndex) const{
//********************************************************************

  // This function adds all subsequent steps to the steps vector at a given split, for the specific branchIndex  at this split. 

  // Make sure the requested branchIndex is OK
  if ((branchIndex<0 || branchIndex>=(Int_t)_nextSteps.size()) && branchIndex!=0 && _nextSteps.size() !=0){
    std::cout << "StepBase::GetNextSteps(branch): Branch index " << branchIndex << " does not exist in Step '" << _title << "'" << std::endl;
    return BranchOutOfRange;
  }
  
  // No more steps to add when nextSteps has 0 elements
  if (_nextSteps.size()==0) return NoBranches;

  // TODO: not sure it is needed. Just a protection
  if (!_nextSteps[branchIndex]) return BranchEmpty;

  // Add The next step
  steps.push_back(_nextSteps[branchIndex]);

  // Go recursively to add the doughters of the next step
  _nextSteps[branchIndex]->GetNextSteps(steps);

  return BranchOK;
}

