#include "SelectionBase.hxx"
#include "CoreUtils.hxx"
#include "MultiThread.hxx"

//********************************************************************
SelectionBase::SelectionBase(bool forceBreak, EventBoxId_h eventBoxId){
//********************************************************************

    _initialized=false;
    _name="";
    _enabled=true;  
    
    _branchSequence.clear();
    _branchAlias.clear();
    _nEventsPassed.clear();

#ifndef MULTITHREAD
    for(unsigned int branch = 0; branch < NMAXBRANCHES; ++branch){
      for(unsigned int cut = 0; cut < NMAXSTEPS; ++cut){
        _cut_passed_stats[branch][cut] = 0;
      }
    }
#endif
        
    _nCutsInBranch.clear();
    
    //  AddBranch(0, ConvertBranch(),"trunk");
    
    // whether to break or not the cut sequence when a cut is not passed
    _forceBreak = forceBreak;
    
    /// By default EventSummary class is created and filled when the selection is passed. But this might be necessary also for events not passing the selection
    /// The user can force the EventSummary to be filled always. 
    _forceFillEventSummary = false;

    // Initialised all run periods to be true - the selection will be applied to all run periods by default
    // TODO: Really should use enumeration for run periods!!
    for(int i = 0; i < 10; ++i){
        _validRunPeriods[i] = true;
    }
    
    // Initialze default pre-selection accum cut level to zero
    _presel_accum_cut_level = 0;

    // The index of the selection in the selection manager (for enabled selections)
    _selEnabledIndex = 0;

      
    // The maximum accum level of any of the branches in all toys
    _maxAccumLevelForAllToys = 0;
     
    // detector in which the Fiducial Volume is defined, By default no detector is set
    for (UInt_t b=0;b<NMAXBRANCHES;b++)
      _detectorFV[b] = UNASSIGNEDID;

    // Set the EventBox to be used by this selection (kUnassigned  by default)
    _eventBoxId = eventBoxId;


    _firstSteps.clear();

    _rootStep = new StepBase();
    _rootStep->Configure(StepBase::kUnknown, "dummy");
    _rootStep->AddBranchUniqueID(0);
    _rootStep->SetIndex(-1);
}

//********************************************************************
bool SelectionBase::HasBranch(Int_t ibranch, const std::string& fromMethod) const{
//********************************************************************
 
  if (ibranch<0 || ibranch>=(Int_t)GetNBranches()){
    std::cout << "ERROR. SelectionBase::" <<  fromMethod << "() Unknown branch " << ibranch 
              << ". There are " << GetNBranches() << " branches in selection of type " << typeid(*this).name() << std::endl;
    return false;
  }
  return true;
}

//********************************************************************
void SelectionBase::SetDetectorFV(SubDetId_h det, Int_t ibranch) {
//********************************************************************
 
  if (ibranch==-1){
    for (UInt_t b = 0; b<GetNBranches();b++) 
      _detectorFV[b]=det;
  }
  else if (!HasBranch(ibranch,"SetDetectorFV"))
    exit(1);
  else
    _detectorFV[ibranch]=det;
}

//********************************************************************
SubDetId_h SelectionBase::GetDetectorFV(Int_t ibranch) const {
//********************************************************************

  if (!HasBranch(ibranch,"GetDetectorFV")) exit(1);
  return _detectorFV[ibranch];
}

//********************************************************************
void SelectionBase::InitializeRootStepFromFirstSteps(){
//********************************************************************

  // For backwards compatibility. It fills the _rootStep using the firstSteps vector in old micro-trees  
  // Don't do anything when rootStep is aready filled (from a new micro-tree)

  // TODO. Workarround to save the _rootStep in the root file
  _rootStep = new StepBase(_rootStep2);
  
  /*
  if (_rootStep->GetNextSteps().size() <_firstSteps.size()){
    _rootStep->AddBranches(_firstSteps.size());
    for (UInt_t i=0;i<_firstSteps.size();i++)
      _rootStep->AddNextStep(_firstSteps[i],i);
  }
  */
}

//********************************************************************
void SelectionBase::Initialize(){
//********************************************************************

  // Don't do anything when the selection is already initialized
  if (_initialized) return;

  // Define the selection steps
  DefineSteps();
  
  // Define the detector Fiducial Volume in which the selection is applied
  DefineDetectorFV();

  // validate the selection. The program will exit if the selection is not correct
  Validate();

  // TODO. Workarround to save the _rootStep in the root file
  _rootStep2 = *_rootStep;
  
  // Mark this selection as initialized
  _initialized=true;
}

//********************************************************************
void SelectionBase::Validate() const{
//********************************************************************
  
  std::cout << "SelectionBase::Validate(). Validating selection '" << _name << "' of type " << typeid(*this).name() << " .....";

  // Validate all steps recursively
  _rootStep->ValidateStepRecursive();

  if (_eventBoxId == UNASSIGNEDID){
    std::cout << "ERROR. SelectionBase::Validate(). _eventBoxId is not set in this selection. Please use the constructor to specify the EventBox to be used. " 
              << "The standard way is to write something like 'numuCCSelection::numuCCSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker)'. "
              << "_eventBoxId is a integer (not an enum) and therefor values in the EventBoxId.hxx file (in psycheEventModel) can be extended using a different enum" << std::endl;
    exit(1);
  }

  
  // There are two different ways of getting the steps for a given branch
  // - GetStepsInBranchWithDummy(branch) uses the branchSequence in SelectionBase
  // - GetStepsInBranchValidate(ID) uses the branchUniqueIDs saved in each of the steps
  // If they give different results it means that there is a bug either in StepBase or in SelectionBase
  for (UInt_t ID=0;ID<_branchAlias.size();ID++){
    std::vector<UInt_t> branch = GetBranchSequence(ID);
    
    std::vector<StepBase*> steps = GetStepsInBranchWithDummy(branch);
    std::vector<StepBase*> steps2 = GetStepsInBranchValidate(ID);

    if (steps.size() != steps2.size()){
      std::cout << "ERROR. SelectionBase::Validate(). There is a bug in the branch treatment. Please contact the highland developpers !!!! " << std::endl;
      exit(1);
    }
    
    for (UInt_t i=0;i<steps.size();i++ ){
      if (steps[i] != steps2[i]){
        std::cout << "ERROR. SelectionBase::Validate(). There is a bug in the branch treatment. Please contact the highland developpers !!!! " << std::endl;
        exit(1);
      }
    }
  }
  
  std::cout << " OK " << std::endl;
}

//********************************************************************
void SelectionBase::CreateToyBoxArray(Int_t nevents){
//********************************************************************

#ifndef MULTITHREAD
  nevents=1;
#endif

  // Create the array of PreviousToyBox for a given number of events 
  PreviousToyBox = new ToyBoxB*[nevents];
  for (Int_t k= 0;k<nevents;k++){
    PreviousToyBox[k]= MakeToyBox();

    // Calling the Reset method we check that it is implemented in the derived class (if the class is derived)
    PreviousToyBox[k]->Reset();

    anaUtils::CreateArray(PreviousToyBox[k]->AccumLevel,   GetNBranches(), 0);
    anaUtils::CreateArray(PreviousToyBox[k]->BranchPassed, GetNBranches(), true);
    PreviousToyBox[k]->nBranches = GetNBranches();

    PreviousToyBox[k]->SelectionEnabledIndex = _selEnabledIndex;

    // Set the DetectorFV into the ToyBox. By default the DetectorFV for a particular Toy is the one of the first branch in the selection. 
    // For selections in which the FV may change depending on the event and toy, this variable will be overwritten when applying the selection.
    PreviousToyBox[k]->DetectorFV = _detectorFV[0];
  }
}

//********************************************************************
void SelectionBase::FinalizeEvent(const AnaEventC& event){
//********************************************************************
  
  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#else
  (void)event;
#endif

  // Reset the PreviousToyBox for the input event. This should be called after processing all toys for a given event
  PreviousToyBox[uniqueID]->ResetBase();  // first the base class
  PreviousToyBox[uniqueID]->Reset();      // then the derived class (if any)
  PreviousToyBox[uniqueID]->MaxAccumLevel=-1;
      
}

//********************************************************************
const ToyBoxB& SelectionBase::GetPreviousToyBox(const AnaEventC& event) const{
//********************************************************************
  
  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#else
  (void)event;
#endif

  return *PreviousToyBox[uniqueID];
}

//********************************************************************
const ToyBoxB& SelectionBase::GetPreviousToyBox() const{
//********************************************************************
  
#ifdef MULTITHREAD
  std::cout << "ERROR. SelectionBase::GetPreviousToyBox() with no Event provided cannot be called in MULTITHREAD mode " << std::endl;
  exit(1);
#endif

  return *PreviousToyBox[0];
}

//********************************************************************
bool SelectionBase::Apply(AnaEventC& event, bool& redo){
//********************************************************************

    //------ This function is called for each event and toy experiment ----------

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#endif

    // Retrieve the ToyBoxB to store intermediate objects
    ToyBoxB* ToyBox = PreviousToyBox[uniqueID];

    // by default start from the first step
    Int_t firstStepToApply=0;
    // Check whether the selection has to be redone or not. To do that the ToyBox of the previous toy for this event should not have default values
    // That means that this is not the first toy
    if (ToyBox->MaxAccumLevel>-1){
      // This method computes also the step from which the selection should be reapplied
      redo = CheckRedoSelection(event, *ToyBox, firstStepToApply);
      // if the selection does not have to be reapplied just return the success boolean of the previous toy: any of the branches succesfull
      if (!redo) return (ToyBox->SuccessfulBranch!=-1);
    }
    else
      _maxAccumLevelForAllToys=0;


    // If we are here that means that the selection must be reapplied
    redo=true;

    // Reset the ToyBox 
    if (firstStepToApply==0){
      ToyBox->ResetBase(); // first the base class
      ToyBox->Reset();  // then the derived class (if any)
    }
    else
      ToyBox->SoftReset();
    // Apply all steps from firstStepToApply. Returns true when all cuts are passed
    bool ok = ApplySteps(event, *ToyBox, firstStepToApply); 
  
    // Save the accum level of this toy
    Int_t all_cuts_passed[NMAXBRANCHES];
    for (UInt_t i=0;i<GetNBranches();i++){
      if (ToyBox->BranchPassed[i]) ToyBox->SuccessfulBranch = i;
      all_cuts_passed[i]  = ToyBox->BranchPassed[i];
      // If we pass one branch then say that the event passed the selection
      if(all_cuts_passed[i]) ok = true;
    }

    // By default EventSummary class is created and filled when the selection is passed. But this might be necessary also for events not passing the selection
    // The user can force the EventSummary to be filled always by setting _forceFillEventSummary to true. 
    // TODO: return value of FillEventSummary ignored for the moment to solve bug 1160. Must check if there are side effects (bug 1164)
    if (ok || _forceFillEventSummary){
      // Tell the FillEventSummary which cuts were passed
      FillEventSummary(event, all_cuts_passed);
    }

    return ok;
}

//********************************************************************
void SelectionBase::PrintStatistics() const{
//********************************************************************

    std::cout << "----------- Statistics for selection '" << _name << "'" << std::endl;

    UInt_t ID=0;
    for (std::vector<std::string>::const_iterator it = _branchAlias.begin();it!=_branchAlias.end();it++, ID++){
        PrintStatistics(ID);
    }
}

//********************************************************************
void SelectionBase::PrintStatistics(UInt_t ID) const{
//********************************************************************

#ifndef MULTITHREAD
    std::cout << "branch '" << GetBranchAlias(ID) << "' --> # events = " << _nEventsPassed[ID] << std::endl;
    for(unsigned int cut = 0; cut < GetNCuts(ID); ++cut){
        std::cout << "cut " << cut << " = " << _cut_passed_stats[ID][cut] << std::endl;
    }
#else
    std::cout << "SelectionBase::PrintStatistics(). No statistics can be printed in MULTITHREAD mode !!!" << std::endl; 
#endif
}

//********************************************************************
void SelectionBase::PrintStatistics(const std::string& name) const{
//********************************************************************

    UInt_t ID = GetBranchUniqueID(name);
    PrintStatistics(ID);
}

//********************************************************************
void SelectionBase::InitializeCutLevels(){
//********************************************************************

    int nBranches = GetNBranches();

    memset(&_current_cut[0],     0, nBranches          *sizeof(_current_cut[0]));
    memset(&_cut_passed[0][0],   0, nBranches*NMAXSTEPS*sizeof(_cut_passed[0][0]));
}

//********************************************************************
bool SelectionBase::ApplyStep(AnaEventC& event, ToyBoxB& ToyBox, const StepBase& step, Int_t branch) {
//********************************************************************

  bool passed = step.Apply(event, ToyBox);  
  // If the step is not a cut it should return always true
  if (step.Type() != StepBase::kCut){
    CutPassed(passed, branch, ToyBox);
    return true;
  }
  
  return passed;
}

//********************************************************************
bool SelectionBase::ApplyStepRecursive(AnaEventC& event, ToyBoxB& ToyBox, const StepBase& step, Int_t firstStepToApply) {
//********************************************************************

    // This function applies all steps recursively provided the first step

    // Apply the current step
    // If the step is not a cut it should return always true

    // Steps below firstStepToApply are assumed to be already applied and passed
    bool passed = true;
    if (step.Index() >= firstStepToApply){  
      // Disabled actions (in all branches) are not applied
      if (step.Type() == StepBase::kCut || !step.IsDisabledInBranch(0))        
        passed = step.Apply(event, ToyBox);       
    } 

    // Deal with accum_level and individual steps passed
    if (step.Type() == StepBase::kCut){
        CutPassed(passed, step, ToyBox);

        // Stop applying steps when there is a break point.
        // Or when _forceBreak == true (we are not interested about intermediate steps)
        if (!passed && (step.Break() || _forceBreak)){
            return false;
        }
    }    

    for (UInt_t i = 0; i < step.GetNextSteps().size(); ++i){
        StepBase* step2=step.GetNextSteps()[i];
        bool passed2 = true;
        if (step2) passed2 = ApplyStepRecursive(event,ToyBox,*step2, firstStepToApply);
        // When a cut is not passed the final return must be false
        if (passed) passed=passed2;
    }

    return passed;
}

//********************************************************************
bool SelectionBase::ApplySteps(AnaEventC& event, ToyBoxB& ToyBox, const std::string& branch, Int_t ifirst, Int_t ilast) {
//********************************************************************

    // This function applies steps in a given branch from step ifirst to step ilast

    // initialize the cut levels
    InitializeCutLevels();

    bool all_passed=true;

    // Get all steps in a given branch
    std::vector<StepBase*> steps = GetStepsInBranch(branch);

    // Get the branch unique ID for a given branch name
    UInt_t ID = GetBranchUniqueID(branch);

    // Loop over steps
    Int_t ilast2 = std::min(ilast,(Int_t)steps.size());
    for (Int_t i=ifirst;i<ilast2;i++ ){
        bool passed = ApplyStep(event, ToyBox, *steps[i], ID);
        if (!passed) all_passed=false;
        // Don't continue when a given step(cut) has a break point
        // Or when _forceBreak == true (we are not interested about intermediate steps)
        if ((steps[i]->Break() || _forceBreak) && !passed) break; 
    }

    if (all_passed)
        _nEventsPassed[ID]++;

    // TODO. This return value is not really used (yet !!!!)
    return all_passed;
}

//********************************************************************
bool SelectionBase::ApplySteps(AnaEventC& event, ToyBoxB& ToyBox, Int_t firstStepToApply) {
//********************************************************************

    // This function applies all steps in all branches

    // There should be at least one step
    if ( _rootStep->GetNextSteps().size() == 0) return false;

#ifndef MULTITHREAD
    // initialize the cut levels
    InitializeCutLevels();
#endif

    // Apply all steps recursively
    bool ok = ApplyStepRecursive(event, ToyBox, *_rootStep, firstStepToApply);

#ifndef MULTITHREAD
    // update the event selection statistics
    for (UInt_t i=0; i<_nEventsPassed.size();i++){
        if (ToyBox.BranchPassed[i]) _nEventsPassed[i]++;
    }
#endif

    return ok;
}

//********************************************************************
bool SelectionBase::CutPassed(bool ok, const StepBase& step, ToyBoxB& box) {
//********************************************************************

    // if the step is not a cut it's allways passed
    if (step.Type() != StepBase::kCut) return true;

    // Get the list of branches unique IDs associated to this cut
    const std::vector<UInt_t>& branches = step.GetBranchUniqueIDs();

    // Process each of the branches
    for (UInt_t i = 0;i<branches.size();i++){
      bool ok2=ok;
      // If a cut is disabled in a given branch we assume it returns true for that branch
      if (step.IsDisabledInBranch(branches[i])) ok2=true; 
      CutPassed(ok2, branches[i], box);
    }

    return ok;
}

//********************************************************************
bool SelectionBase::CutPassed(bool ok, Int_t branch, ToyBoxB& box) {
//********************************************************************

    if (ok){
        // The event has passed all previous cuts and this one
        if(box.BranchPassed[branch]){
            box.AccumLevel[branch]++;

            // compute the max accum cut level from all branches
            if (box.AccumLevel[branch]> box.MaxAccumLevel) 
                box.MaxAccumLevel = box.AccumLevel[branch];

#ifndef MULTITHREAD
            if (box.AccumLevel[branch]> _maxAccumLevelForAllToys)
              _maxAccumLevelForAllToys = box.AccumLevel[branch];            
#endif

        }
#ifndef MULTITHREAD
        // the event has passed cut number "icut"
        _cut_passed[branch][_current_cut[branch] ]=true;
        _cut_passed_stats[branch][ _current_cut[branch] ]++;        
#endif
    }
    else 
        box.BranchPassed[branch] = false;

    // increment the current cut in each branch
 #ifndef MULTITHREAD
    _current_cut[branch]++;
#endif

    return ok;
}

//********************************************************************
bool SelectionBase::PreSelectionPassed(const AnaEventC& event){
//********************************************************************

  return (GetPreviousToyBox(event).MaxAccumLevel >= _presel_accum_cut_level);
}

//********************************************************************
UInt_t SelectionBase::GetNMaxCuts() const{
//********************************************************************

    /// Return the number of cuts of the branch with more cuts 

    UInt_t nmaxcuts=0;
    for (UInt_t i=0;i<GetNBranches();i++ )
        if ( GetNCuts(i) > nmaxcuts) nmaxcuts=GetNCuts(i);

    return nmaxcuts;
}

//********************************************************************
UInt_t SelectionBase::GetNCuts(Int_t ibranch) const{
//********************************************************************

    /// Return the number of cuts that have been added for a given branch
    if (!HasBranch(ibranch,"GetNCuts")) return 0;
    return _nCutsInBranch[ibranch];
}

//********************************************************************
void SelectionBase::DumpSteps(const std::string& name, bool onlycuts) const{
//********************************************************************

    if (_branchAlias.size() ==0){
        std::cout << "ERROR. SelectionBase::DumpSteps(). Cannot dump steps before setting the branch aliases" << std::endl;
        exit(1);
    }

    if (name==""){
        std::vector<std::string>::const_iterator it;
        for (it=_branchAlias.begin();it!=_branchAlias.end();it++)
            DumpSteps(*it,onlycuts);
    }
    else{
        Int_t ID = GetBranchUniqueID(name);
        DumpSteps(ID,onlycuts);
    }
}

//********************************************************************
void SelectionBase::DumpSteps(Int_t ID, bool onlycuts) const{
//********************************************************************

  if (ID==-1){
    std::vector<std::string>::const_iterator it;
    for (it=_branchAlias.begin();it!=_branchAlias.end();it++)
      DumpSteps(*it,onlycuts);
  }
  else{
    if (ID >= (Int_t)GetNBranches() || ID<0){ 
	  std::cout << "SelectionBase::DumpSteps(). Branch with index " << ID << " does not exist in selection '"<< _name <<"' !!!" << std::endl; 
	  return;
    }
    
    std::vector<UInt_t> branch = GetBranchSequence(ID);
    std::string alias = GetBranchAlias(ID);
	std::string steps_cuts = "Steps";
    std::stringstream sID;
    sID << ID;

	std::string sbranch = "branch " + sID.str() + " --> " + ConvertBranchToString(branch) + " '" + alias + "'";

	if (onlycuts) steps_cuts = "Cuts";
    if (_branchAlias.size()==1) sbranch = " with no branches";
	
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "      " << steps_cuts << " for selection '" << _name << "' " << sbranch << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;


    std::vector<StepBase*> steps = GetStepsInBranchWithDummy(branch);

    //    A minimum legth of 25 for the title field
    UInt_t max_length=25;
    for (UInt_t i=0;i<steps.size();i++ ){
      if (steps[i]->Title().length()+3 > max_length) max_length = steps[i]->Title().length()+3;
    }

    std::stringstream smax_length;
    smax_length << max_length;

    std::string format  = "%3s: %-6s %-10s %-"+smax_length.str() +"s %-6s %-10s";
    std::string format2 = "%3d: %-6d %-10s %-"+smax_length.str() +"s %-6d ";
    
    char out[256];  
    sprintf(out,format.c_str(), "#", "index", "type",  "title", "break", "branches");
    std::cout << out << "\n" << std::endl;
    
    UInt_t step=0;
    for (UInt_t i=0;i<steps.size();i++ ){
      if ((!onlycuts || steps[i]->Type() == StepBase::kCut) && steps[i]->Title()!="dummy"){      
        if (steps[i]->IsDisabledInBranch(ID))
          sprintf(out,format2.c_str(), (Int_t)i, steps[i]->Index(), (steps[i]->ConvertType()+" (D)").c_str(),  steps[i]->Title().c_str(), (int)steps[i]->Break());
        else
          sprintf(out,format2.c_str(), (Int_t)i, steps[i]->Index(), steps[i]->ConvertType().c_str(),           steps[i]->Title().c_str(), (int)steps[i]->Break());
        std::cout << out;
        const std::vector<UInt_t>& IDs = steps[i]->GetBranchUniqueIDs();
        for (UInt_t j=0;j<IDs.size();j++){
          if (steps[i]->IsDisabledInBranch(IDs[j]))
            std::cout << "("<<IDs[j] << ") ";
          else
            std::cout << IDs[j] << " ";
        }
        std::cout << std::endl;
        step++;
      }
      
      if (steps[i]->GetNextSteps().size()>1) {
        std::cout << "--- split with " << steps[i]->GetNextSteps().size() << " branches. Next steps: ";
        for (UInt_t j=0;j<steps[i]->GetNextSteps().size();j++)         {
          std::cout << " ( ";
          if (steps[i]->GetNextSteps()[j])
            std::cout << steps[i]->GetNextSteps()[j]->Title();
          else
            std::cout << "NULL";
          std::cout << " ) ";
        }
        std::cout << std::endl;
      }
    }
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    
  }
}

//********************************************************************
void SelectionBase::DumpBranches() const{
//********************************************************************

  std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
  std::cout << "       Branches for selection '" << _name << "' " << std::endl;
  std::cout << "------------------------------------------------------------------------------------------------" << std::endl;

  char out[256];  
  sprintf(out,"%3s: %-25s %-10s %-10s %-10s", "ID", "name", "Sequence", "#steps", "#cuts");
  std::cout << out << "\n" << std::endl;

  for (Int_t ID=0;ID<(Int_t)GetNBranches();ID++){
    
    std::vector<UInt_t> branch = GetBranchSequence(ID);
    std::string alias = GetBranchAlias(ID);	
    std::vector<StepBase*> steps = GetStepsInBranch(branch);
    
    sprintf(out,"%3d: %-25s %-10s %-10d %-10d", ID, alias.c_str(), ConvertBranchToString(branch).c_str(),(Int_t)steps.size(),GetNCuts(ID));
    std::cout << out << std::endl;
  }
  std::cout << "------------------------------------------------------------------------------------------------" << std::endl;

}


//********************************************************************
void SelectionBase::DisableStep(const std::string& title, Int_t ID) {
//********************************************************************

  Int_t ID2 = ID;
  if (ID==-1) ID2=0;
  StepBase* step = GetStep(title,ID2);
  if (!step){
    std::cout << "ERROR. SelectionBase::DisableStep(). Step with title '" << title << "' does not exist in branch "<< ID2 << std::endl;
    exit(1);
  }

  if (step->Type() == StepBase::kAction && ID!=-1){
    std::cout << "ERROR. SelectionBase::DisableStep(). Action '" << title << "' cannot be disabled in a single branch. " 
              << "Actions should be disabled in all branches. Don't specify branch ID" << std::endl;
    exit(1);
  }

  if (ID==-1){
    const std::vector<UInt_t> branches = step->GetBranchUniqueIDs();
    for (UInt_t i=0;i<branches.size();i++)
      step->DisableInBranch(i);
  }
  else
    step->DisableInBranch(ID);

}


//********************************************************************
void SelectionBase::RemoveStep(const std::string& title, Int_t branchID) {
//********************************************************************


  // This methods removes a step with a given title from the specified branch ID.
  // The branchID is needed because:
  //  - There could be two different steps with the same title in two bramnches
  //  - A step can be shared by several branches. If the step is removed in one of them a split will be created at that point.
  // When the branchID is not specified (branchID=-1) the step is removed in all the branches

  Int_t branchID2 = branchID;

  if (branchID==-1){
    // Get the first branch containing a step with a given title
    StepBase* step2 = GetStepAndBranchID(title,branchID2);
    (void) step2;
  }
  StepBase* step = GetStep(title,branchID2);
  if (!step){
    std::cout << "ERROR. SelectionBase::RemoveStep(). Step with title '" << title << "' does not exist in branch "<< branchID2 << std::endl;
    exit(1);
  }

  // Ge the previous step
  StepBase* previousStep = GetPreviousStep(title,branchID2);
    
  // Get the vector of next steps
  const std::vector<StepBase*> nextSteps = step->GetNextSteps();

  
  if (branchID!=-1){   
    // 1. Make a clone of the next step and all steps hanging from it keeping only the steps with the given branch ID
    StepBase* nextStep = NULL;
    Int_t branchIndex1 = step->BranchIndexForBranchUniqueID(branchID);  
    if (branchIndex1>-1) nextStep = nextSteps[branchIndex1]->MakeCloneRecursive(branchID);

    // 2. Remove all steps with a given branch ID below previousStep
    previousStep->RemoveStepsInBranchIDRecursive(branchID);
    
    // 3. Add all subsequent steps skipping the one to remove
    Int_t branchIndex = 0;
    // If we are removing the step from only one of the branches a split should be added at the previous step
    if (step->GetBranchUniqueIDs().size()>0){
      previousStep->AddBranches(1);
      branchIndex = previousStep->GetNextSteps().size()-1;
    }
    if (nextStep) previousStep->AddNextStep(nextStep,branchIndex);
  }
  else{
    // 2. Remove step with a given branch index
    previousStep->RemoveNextStep(0);

    // 3. Add all subsequent steps skipping the one to remove
    for (UInt_t i=0;i<nextSteps.size();i++)
      previousStep->AddNextStep(nextSteps[i],i);
  }

  // 4. Decrement the index by one unit since we have removed one step
  std::vector<StepBase*> nextSteps2;
  previousStep->GetNextSteps(nextSteps2,true,branchID);  // Get all next steps in the given branch ID
  for (std::vector<StepBase*>::iterator it=nextSteps2.begin();it!=nextSteps2.end();it++){
    (*it)->SetIndex((*it)->Index()-1);
  }
  
  // 5. Recompute the branch sequences 
  for (UInt_t i=0;i<_branchSequence.size();i++){    
    std::vector<UInt_t> branch_seq;
    _rootStep->GetBranchSequence(i,branch_seq);  
    _branchSequence[i]=branch_seq;
  }

  if (branchID==-1){
    // Check that there are still steps with the same title in other branches. If so remove them
    Int_t branchID3;
    if (GetStepAndBranchID(title,branchID3)) RemoveStep(title,branchID);
  }
}

//********************************************************************
void SelectionBase::InsertStep(const std::string& previous_title, Int_t branchID, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************


  // Get the previous step
  Int_t branchID2 = branchID;

  if (branchID==-1){
    // Get the first branch containing a step with a given title
    StepBase* step2 = GetStepAndBranchID(previous_title,branchID2);
    (void) step2;
  }
  StepBase* previousStep = GetStep(previous_title,branchID2);
  if (!previousStep){
    std::cout << "ERROR. SelectionBase::InserStep(). Step with title '" << previous_title << "' does not exist in branch "<< branchID2 << std::endl;
    exit(1);
  }

  // Configure the step to insert
  step->Configure(type,title,cut_break);
  step->SetIndex(previousStep->Index()+1);
  step->AddBranchUniqueID(branchID);
    
  // Get the vector of next steps
  const std::vector<StepBase*> nextSteps = previousStep->GetNextSteps();
  
  if (branchID!=-1){   
    // 1. Make a clone of the next step and all steps hanging from it keeping only the steps with the given branch ID
    StepBase* nextStep = NULL;
    Int_t branchIndex1 = previousStep->BranchIndexForBranchUniqueID(branchID);  
    if (branchIndex1>-1) nextStep = nextSteps[branchIndex1]->MakeCloneRecursive(branchID);

    // 2. Remove all steps with a given branch ID below previousStep
    previousStep->RemoveStepsInBranchIDRecursive(branchID);
    
    Int_t branchIndex = 0;
    // If we are removing the step from only one of the branches a split should be added at the previous step
    if (step->GetBranchUniqueIDs().size()>0){
      previousStep->AddBranches(1);
      branchIndex = previousStep->GetNextSteps().size()-1;
    }

    // 3. Add the step to insert
    previousStep->AddNextStep(step,branchIndex);

    // 4. Add all subsequent steps 
    if (nextStep) step->AddNextStep(nextStep,branchIndex);
  }
  else{
    // 2. Remove step with a given branch index
    previousStep->RemoveNextStep(0);

    // 3. Add all subsequent steps skipping the one to remove
    for (UInt_t i=0;i<nextSteps.size();i++)
      previousStep->AddNextStep(nextSteps[i],i);
  }
  
  // 4. Increment the index by one unit since we have added one step
  std::vector<StepBase*> nextSteps2;
  step->GetNextSteps(nextSteps2,true,branchID);  // Get all next steps in the given branch ID
  for (std::vector<StepBase*>::iterator it=nextSteps2.begin();it!=nextSteps2.end();it++){
    (*it)->SetIndex((*it)->Index()+1);
  }

  // 5. Recompute the branch sequences 
  for (UInt_t i=0;i<_branchSequence.size();i++){    
    std::vector<UInt_t> branch_seq;
    _rootStep->GetBranchSequence(i,branch_seq);  
    _branchSequence[i]=branch_seq;
  }


  if (branchID==-1){
    // Check that there are still steps with the same title in other branches. If so insert step after them
    Int_t branchID3;
    if (GetStepAndBranchID(title,branchID3)) InsertStep(previous_title,branchID, type, title, step, cut_break);
  }
}
//********************************************************************
void SelectionBase::ReplaceStep(const std::string& old_title, Int_t branchID, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************

  // Insert a new step after step with old_title in branch ID
  InsertStep(old_title,branchID,type,title,step,cut_break);
  
  // Remove the step old_title in branch ID
  RemoveStep(old_title,branchID);
}

//********************************************************************
void SelectionBase::AddStep(Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6, Int_t b7, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************
    std::vector<UInt_t> branch = ConvertBranch(b0,b1,b2,b3,b4,b5,b6,b7);
    AddStep(branch, type, title, step, cut_break);
}

//********************************************************************
void SelectionBase::AddStep(Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************
    std::vector<UInt_t> branch = ConvertBranch(b0,b1,b2,b3,b4,b5,b6);
    AddStep(branch, type, title, step, cut_break);
}

//********************************************************************
void SelectionBase::AddStep(Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************
    std::vector<UInt_t> branch = ConvertBranch(b0,b1,b2,b3,b4,b5);
    AddStep(branch, type, title, step, cut_break);
}

//********************************************************************
void SelectionBase::AddStep(Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************
    std::vector<UInt_t> branch = ConvertBranch(b0,b1,b2,b3,b4);
    AddStep(branch, type, title, step, cut_break);
}

//********************************************************************
void SelectionBase::AddStep(Int_t b0, Int_t b1, Int_t b2, Int_t b3, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************
    std::vector<UInt_t> branch = ConvertBranch(b0,b1,b2,b3);
    AddStep(branch, type, title, step, cut_break);
}

//********************************************************************
void SelectionBase::AddStep(Int_t b0, Int_t b1, Int_t b2, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************
    std::vector<UInt_t> branch = ConvertBranch(b0,b1,b2);
    AddStep(branch, type, title, step, cut_break);
}

//********************************************************************
void SelectionBase::AddStep(Int_t b0, Int_t b1, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************
    std::vector<UInt_t> branch = ConvertBranch(b0,b1);
    AddStep(branch, type, title, step, cut_break);
}

//********************************************************************
void SelectionBase::AddStep(Int_t b0, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************
    std::vector<UInt_t> branch = ConvertBranch(b0);
    AddStep(branch, type, title, step, cut_break);
}

//********************************************************************
void SelectionBase::AddStep(StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************
    std::vector<UInt_t> branch = ConvertBranch();
    AddStep(branch, type, title, step, cut_break);
}

//********************************************************************
void SelectionBase::AddStep(const std::vector<UInt_t>& branch, StepBase::TypeEnum type, const std::string& title, StepBase* step, bool cut_break){
//********************************************************************

    // Check that no step exist in the same branch with this title
    std::vector<StepBase*> steps = GetStepsInBranchWithDummy(branch);
    for (UInt_t i=0;i<steps.size();i++){
      if (steps[i]->Title() == title){
        std::cout << "ERROR. SelectionBase::AddStep(). An step with title '" << title << "' already exists in this branch " << ConvertBranchToString(branch) 
                  << " at position " << i << " in selection of type " << typeid(*this).name() << std::endl;
        exit(1);
      }
    }

    // Configure the step
    step->Configure(type, title, cut_break);

    // Add the step
    AddStep(branch, step);  
}

//********************************************************************
void SelectionBase::AddStep(const std::vector<UInt_t>& branch, StepBase* step){
//********************************************************************

    Int_t index=0;
    /*
    if (_firstSteps.size() ==0){
        // Add the first step
        _firstSteps.push_back(step);        
        if (step->Title()=="dummy") index=-1;
    }
    else if (_firstSteps.size() >1 && !_firstSteps[branch[0]]){
        _firstSteps[branch[0]] = step;
    }
    else{
        // Get the last step added to this branch sequence
        StepBase* laststep = GetLastStepInBranch(branch);        
        std::cout << "AddStep: " << step->Title() << " "  << laststep->Title() << " " << laststep->Index() << std::endl;
        if (!laststep) return;
        if (branch.size()==0)   
            // Add the step to trunk
            laststep->AddNextStep(step);  
        else  {
            // Add the step to branch
            laststep->AddNextStep(step, branch[branch.size()-1]);  
        }
        index = laststep->Index()+1;
    }

    // Set the step index
    step->SetIndex(index);
    */

    // Get the last step added to this branch sequence
    StepBase* laststep = GetLastStepInBranch(branch);        

    if (!laststep) return;
    if (branch.size()==0)   
      // Add the step to trunk
      laststep->AddNextStep(step);  
    else  {
      // Add the step to branch
      laststep->AddNextStep(step, branch[branch.size()-1]);  
    }
    index = laststep->Index()+1;

    // Set the step index
    step->SetIndex(index);

}

//********************************************************************
void SelectionBase::AddSteps(const std::vector<UInt_t>& branch, const std::vector<StepBase*>& steps){
//********************************************************************

    for (std::vector<StepBase*>::const_iterator it= steps.begin(); it!=steps.end();it++){
        AddStep(branch, *it);
    }
}

//********************************************************************
void SelectionBase::AddSplit(UInt_t nbranches, Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6, Int_t b7){
//********************************************************************

    std::vector<UInt_t> branch = ConvertBranch(b0,b1,b2,b3,b4,b5,b6,b7);
    AddSplit(nbranches,branch);
}

//********************************************************************
void SelectionBase::AddSplit(UInt_t nbranches, const std::vector<UInt_t>& branch_seq){
//********************************************************************

    // When there are no steps yet we should add a dummy one such that NULL steps can be added to this when doing AddSplit
  //    if (_firstSteps.size() ==0){
  //        AddStep(StepBase::kAction, "dummy", new StepBase(), false);  
  //    }

    // Get the last step in the branch sequence
    StepBase* laststep = GetLastStepInBranch(branch_seq);        
    if (!laststep) return;

    std::cout << "SelectionBase::AddSplit(). Add split with " << nbranches << " branches to branch "<< ConvertBranchToString(branch_seq)
              << " after step '" << laststep->Title() << "'" << std::endl;


    if (laststep->GetNextSteps().size() !=0){
        std::cout << "SelectionBase::AddSplit(). Last step in branch " << ConvertBranchToString(branch_seq) << " with title '" <<  laststep->Title() << "'" 
            << " already has an split with " << laststep->GetNextSteps().size() << " branches !!!!" << std::endl;
        return;
    }

    // Add "nbranches" NULL steps to the last step
    laststep->AddBranches(nbranches);
}

//********************************************************************
std::vector<StepBase*> SelectionBase::GetStepsInBranch(const std::vector<UInt_t>& branch, UInt_t first, UInt_t last) const{
//********************************************************************

    std::vector<StepBase*> steps = GetStepsInBranchWithDummy(branch, first, last);  
    if ((*steps.begin())->Title()=="dummy")
        steps.erase(steps.begin());
    return steps;
}

//********************************************************************
std::vector<StepBase*> SelectionBase::GetStepsInBranchValidate(Int_t branchID) const{
//********************************************************************

    std::vector<StepBase*> steps;
    std::vector<StepBase*> steps2;

    _rootStep->GetNextSteps(steps,true);  
    for (UInt_t i=0;i<steps.size();i++){
      if (steps[i]->HasBranchUniqueID(branchID)) steps2.push_back(steps[i]);
    }
    return steps2;
}

//********************************************************************
//std::vector<StepBase*> SelectionBase::GetStepsInBranchWithDummy(const std::vector<UInt_t>& branch, UInt_t first, UInt_t last) const{
//********************************************************************
/*
    std::vector<StepBase*> steps;

    // three different cases

    // 1. No steps were added yet, return an empty vector
    if ( _firstSteps.size() ==0) return steps;

    // Get all steps from trunk (up to first split)
    if ( _firstSteps.size() == 1){ 
        steps.push_back(*_firstSteps.begin());
        if (*steps.rbegin()) 
            (*steps.rbegin())->GetNextSteps(steps);  
    }

    StepBase::BranchStatus status= StepBase::BranchOK;
    for (UInt_t i=0;i<branch.size();i++){
        // 2. There is one split at the beginning of the step sequence
        if (i==0 && _firstSteps.size() > 1){
            steps.push_back(_firstSteps[branch[0]]);
            if (*steps.rbegin()) status = (*steps.rbegin())->GetNextStepsInBranch(steps,branch[i]);  // Get all steps up to (i+1)th split 
            status=StepBase::BranchOK;
        }    
        // 3. If there is no split at the beginning of the step sequence just go through all the branches collecting steps recursively
        else {
          if (status==StepBase::BranchOK && *steps.rbegin()){
            status = (*steps.rbegin())->GetNextStepsInBranch(steps,branch[i]);  // Get all steps up to (i+1)th split 
            if ((status == StepBase::NoBranches  && i>0)|| 
                (status == StepBase::BranchEmpty && i<branch.size()-1) || 
                (status == StepBase::BranchOutOfRange)){
              std::cout << "ERROR. SelectionBase::GetStepsInBranchWithDummy(). Branch " << ConvertBranchToString(branch) 
                        << " does not exist in selection of type " <<  typeid(*this).name() << ". ";
              if (status==StepBase::NoBranches && i>0){ 
                std::cout << "Branch " << ConvertBranchToString(branch,i-1) << " has no split" << std::endl;
              }
              else if (status == StepBase::BranchEmpty && i<branch.size()-1){ 
                std::cout << "Branch " << ConvertBranchToString(branch,i) << " has no split" << std::endl;
              }
              else if (status == StepBase::BranchOutOfRange){
                std::cout << "Last number in " << ConvertBranchToString(branch,i) << " is out of range" << std::endl;
              }
              exit(1);            
            }
          }          
        }
    }


    if (first==0 && last==NMAXSTEPS-1){
        // Return all steps in this branch
        return steps;
    }
    else{
        UInt_t first2 = first;
        UInt_t last2 = last;
        UInt_t size2 = steps.size();
      
        if (_firstSteps[0]->Title()=="dummy"){
          first2++;
          last2++;
          size2--;          
        }
        // Return only from step "first" to step "last"
        std::vector<StepBase*> steps2;
        if (first>=size2){
            std::cout << "SelectionBase::GetStepsInBranchWithDummy(). Invalid first step " << first << ", there are " << size2 << " steps !!!" << std::endl;
            return steps2;
        }
        if (last>=size2){
            std::cout << "SelectionBase::GetStepsInBranchWithDummy(). Invalid last step " << last << ", there are " << size2 << " steps !!!" << std::endl;
            return steps2;
        }
        if (last<first){
            std::cout << "SelectionBase::GetStepsInBranchWithDummy(). Last step " << last << " smaller than first step " << size2 << " !!!" << std::endl;
            return steps2;
        }

        for (UInt_t i=first2;i<=last2;i++)
            steps2.push_back(steps[i]);
        return steps2;
    }
}
*/
//********************************************************************
std::vector<StepBase*> SelectionBase::GetStepsInBranchWithDummy(const std::vector<UInt_t>& branch, UInt_t first, UInt_t last) const{
//********************************************************************

    std::vector<StepBase*> steps;

    // three different cases
    steps.push_back(_rootStep);

    // 1. No steps were added yet, return an empty vector
    if ( _rootStep->GetNextSteps().size() ==0) return steps;
    else if ( _rootStep->GetNextSteps().size() ==1) _rootStep->GetNextSteps(steps);

    StepBase::BranchStatus status= StepBase::BranchOK;
    for (UInt_t i=0;i<branch.size();i++){
      if (status==StepBase::BranchOK && (*steps.rbegin() || i==0)){
        status = (*steps.rbegin())->GetNextStepsInBranch(steps,branch[i]);  // Get all steps up to (i+1)th split 
        if ((status == StepBase::NoBranches  && i>0)|| 
            (status == StepBase::BranchEmpty && i<branch.size()-1) || 
            (status == StepBase::BranchOutOfRange)){
          std::cout << "ERROR. SelectionBase::GetStepsInBranchWithDummy(). Branch " << ConvertBranchToString(branch) 
                    << " does not exist in selection of type " <<  typeid(*this).name() << ". ";
          if (status==StepBase::NoBranches && i>0){ 
            std::cout << "Branch " << ConvertBranchToString(branch,i-1) << " has no split" << std::endl;
          }
          else if (status == StepBase::BranchEmpty && i<branch.size()-1){ 
            std::cout << "Branch " << ConvertBranchToString(branch,i) << " has no split" << std::endl;
          }
          else if (status == StepBase::BranchOutOfRange){
            std::cout << "Last number in " << ConvertBranchToString(branch,i) << " is out of range" << std::endl;
          }
          exit(1);            
        }
      }          
    }

    // Remove the rootStep, which is dummy
    steps.erase(steps.begin());

    if (first==0 && last==NMAXSTEPS-1){
        // Return all steps in this branch
        return steps;
    }
    else{
        // Return only from step "first" to step "last"
        std::vector<StepBase*> steps2;
        if (first>=steps.size()){
          std::cout << "SelectionBase::GetStepsInBranchWithDummy(). Invalid first step " << first << ", there are " << steps.size() << " steps !!!" << std::endl;
            return steps2;
        }
        if (last>=steps.size()){
            std::cout << "SelectionBase::GetStepsInBranchWithDummy(). Invalid last step " << last << ", there are " << steps.size() << " steps !!!" << std::endl;
            return steps2;
        }
        if (last<first){
            std::cout << "SelectionBase::GetStepsInBranchWithDummy(). Last step " << last << " smaller than first step " << steps.size() << " !!!" << std::endl;
            return steps2;
        }
        for (UInt_t i=first;i<=last;i++)
            steps2.push_back(steps[i]);
        return steps2;
    }
}


//********************************************************************
std::vector<StepBase*> SelectionBase::GetStepsInBranch(const std::string& name, UInt_t first, UInt_t last) const{
//********************************************************************

    std::vector<UInt_t> branch = GetBranchSequence(name);
    return GetStepsInBranch(branch,first,last);
}

//********************************************************************
std::vector<StepBase*> SelectionBase::GetStepsInBranch(UInt_t branchID, UInt_t first, UInt_t last) const{
//********************************************************************

    std::vector<UInt_t> branch = GetBranchSequence(branchID);
    return GetStepsInBranch(branch,first,last);
}

//********************************************************************
std::vector<StepBase*> SelectionBase::GetCutsInBranch(UInt_t branchID) const{
//********************************************************************

    std::vector<StepBase*> cuts;
    std::vector<StepBase*> steps = GetStepsInBranch(branchID);
    for (std::vector<StepBase*>::iterator it=steps.begin();it!=steps.end();it++){
        if ((*it)->Type() == StepBase::kCut) cuts.push_back(*it);
    }

    return cuts;
}

//********************************************************************
std::vector<StepBase*> SelectionBase::GetCutsInBranch(const std::string& name) const{
//********************************************************************

    UInt_t branchID = GetBranchUniqueID(name);
    return GetCutsInBranch(branchID);
}

//********************************************************************
StepBase* SelectionBase::GetLastStepInBranch(const std::vector<UInt_t>& branch) const{
//********************************************************************

    // get the vector of steps in this branch sequence
    std::vector<StepBase*> steps = GetStepsInBranchWithDummy(branch);

    // returns the last step if exists
    if (steps.size()!=0)
        return *(steps.rbegin());
    else{
        return NULL;
    }
}

//********************************************************************
StepBase* SelectionBase::GetLastStepInBranch(Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6, Int_t b7) const{
//********************************************************************

    std::vector<UInt_t> branch = ConvertBranch(b0,b1,b2,b3,b4,b5,b6,b7);
    return GetLastStepInBranch(branch);
}

//********************************************************************
StepBase* SelectionBase::GetLastStepInBranch(const std::string& name) const{
//********************************************************************

    std::vector<UInt_t> branch = GetBranchSequence(name);
    return GetLastStepInBranch(branch);
}


//********************************************************************
StepBase* SelectionBase::GetStepInBranch(const std::vector<UInt_t>& branch, UInt_t index) const{
//********************************************************************

  std::vector<StepBase*> steps = GetStepsInBranch(branch, index, index);
  if (steps.size()!=0)
    return *(steps.begin());
  else{
    return NULL;
  }
}

//********************************************************************
StepBase* SelectionBase::GetStepInBranch(const std::string& name, UInt_t index) const{
//********************************************************************

  std::vector<StepBase*> steps = GetStepsInBranch(name, index, index);
  if (steps.size()!=0)
    return *(steps.begin());
  else{
    return NULL;
  }
}

//********************************************************************
StepBase* SelectionBase::GetStepInBranch(UInt_t branchID, UInt_t index) const{
//********************************************************************

  std::vector<StepBase*> steps = GetStepsInBranch(branchID, index, index);
  if (steps.size()!=0)
    return *(steps.begin());
  else{
    return NULL;
  }
}

//********************************************************************
std::vector<UInt_t> SelectionBase::ConvertBranch(Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6, Int_t b7) const{
//********************************************************************

    std::vector<UInt_t> branch;
    if (b0>=0) branch.push_back(b0);
    if (b1>=0) branch.push_back(b1);
    if (b2>=0) branch.push_back(b2);
    if (b3>=0) branch.push_back(b3);
    if (b4>=0) branch.push_back(b4);
    if (b5>=0) branch.push_back(b5);
    if (b6>=0) branch.push_back(b6);
    if (b7>=0) branch.push_back(b7);

    return branch;
}

//********************************************************************
std::string SelectionBase::ConvertBranchToString(const std::vector<UInt_t>& branch, Int_t upToSplit) const{
//********************************************************************

    if (branch.size()==0) return "'trunk'";

    std::stringstream os;  

    UInt_t nsplits = branch.size();
    if (upToSplit>=0 && (UInt_t)upToSplit<branch.size())
      nsplits=(UInt_t)upToSplit+1;
    else if (upToSplit!=-1){
      std::cout << "ERROR. SelectionBase::ConvertBranchToString(). Branch sequence " << ConvertBranchToString(branch) << " has less than " << upToSplit+1 << " splits !!!" << std::endl;
        exit(1);
    }
      

    os << "(";  
    for (UInt_t i=0;i<nsplits;i++){
        if (i>0) os<< ",";
        os << branch[i];
    }
    os << ")";

    return  os.str();

}

//********************************************************************
void SelectionBase::AddBranch(Int_t ibranch, const std::vector<UInt_t>& branch_seq, const std::string& alias){
//********************************************************************

    // associate the branch unique ID with the branch alias (branch1, branch2, ...)
    // This is just a default name, that can be changed later
    if (alias==""){
        char branch[50];
        sprintf(branch, "branch%d", ibranch);
        _branchAlias.push_back(branch);  
    }
    else
        _branchAlias.push_back(alias);  

    // associate the branch unique ID with the branch sequence
    _branchSequence.push_back(branch_seq);

    // Initialize the event counter for this branch
    _nEventsPassed.push_back(0);

    // get the vector of steps in this branch sequence
    std::vector<StepBase*> steps = GetStepsInBranchWithDummy(branch_seq);
    if (steps.size()==0) return;

    // Add the branch uniqueID to all steps in this branch
    AddBranchUniqueID(steps, ibranch);

    //Store the number of cuts in each branch such that we don't have to recomput it everytime
    _nCutsInBranch.push_back(GetCutsInBranch(ibranch).size());
}

//********************************************************************
void SelectionBase::SetBranchAlias(Int_t ID, const std::string& name, Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6, Int_t b7){
//********************************************************************

    /*
        A full branch is defined by a branching sequence, that is the branch to be taken in each split. 
        But we can set a unique ID for the full branch and a name. In that way we can for example apply all steps 
        in a given branch by specifying the ID or the name. 
    */

    // Check that this branch has the correct ID
    if (ID != (Int_t)(_branchSequence.size())){
      std::cout << "ERROR. SelectionBase::SetBranchAlias(). Invalid branch unique ID " << ID << " for branch with alias '"<<  name << "' . It should be " << _branchSequence.size() << std::endl;
      exit(1);
    }

    // Check that the branch has not an alias already in use
    for (UInt_t i=0;i<_branchAlias.size();i++){
      if (_branchAlias[i] == name ){ 
        std::cout << "ERROR. SelectionBase::SetBranchAlias(). Alias '" << name << "' cannot be used for branch " << ID << " since is already used for branch " << i << std::endl;
        exit(1);
      }
    } 


    // Convert the branch sequence with individual integers into a vector of integers 
    std::vector<UInt_t> branch_seq = ConvertBranch(b0, b1, b2, b3, b4, b5, b6, b7);

    // get the vector of steps in this branch sequence
    //  std::vector<StepBase*> steps = GetStepsInBranch(branch_seq);
    //  if (steps.size()==0) return;

    // Add the branch uniqueID to all steps in this branch
    //  AddBranchUniqueID(steps, ID);

    AddBranch(ID,branch_seq,name);

    // associate the branch unique ID with the branch alias
    _branchAlias[ID] = name;  

    // associate the branch unique ID with the branch sequence
    _branchSequence[ID] = branch_seq;


}

//********************************************************************
void SelectionBase::AddBranchUniqueID(std::vector<StepBase*>& steps, UInt_t ID){
//********************************************************************

    std::vector<StepBase*>::iterator it;
    for (it=steps.begin();it!=steps.end();it++){
        if (*it) (*it)->AddBranchUniqueID(ID);  
    }
}

//********************************************************************
std::vector<UInt_t> SelectionBase::GetBranchSequence(Int_t ID) const{
//********************************************************************

    if (ID<0 || ID >= (Int_t)_branchSequence.size()){
        std::cout << "ERROR. SelectionBase::GetBranchSequence(). Branch with index " << ID << " does not exist in selection '"<< _name <<"' !!!" << std::endl; 
        exit(1);
    }
    return _branchSequence[ID];
}

//********************************************************************
std::vector<UInt_t> SelectionBase::GetBranchSequence(const std::string& name) const{
//********************************************************************

    Int_t  ID = GetBranchUniqueID(name);
    return GetBranchSequence(ID);
}

//********************************************************************
Int_t SelectionBase::GetBranchUniqueID(const std::string& name) const{
//********************************************************************

    Int_t ID = 0;
    for (std::vector<std::string>::const_iterator it=_branchAlias.begin();it!=_branchAlias.end();it++, ID++){
        if (*it == name) return ID; 
    }

    std::cout << "ERROR. SelectionBase::GetBranchUniqueID(). Branch with alias '" << name << "' does not exist in selection '"<< _name <<"' of type " << typeid(*this).name() << std::endl; 
    exit(1);

    return -1;
}

//********************************************************************
std::string SelectionBase::GetBranchAlias(Int_t ID) const{
//********************************************************************

    if (ID<0 || ID >= (Int_t)_branchAlias.size()) return "";
    return _branchAlias[ID];
}

//********************************************************************
std::vector<std::string> SelectionBase::GetCutNames(Int_t ID) const{
//********************************************************************

    std::vector<std::string> cut_names;
    std::vector<StepBase*> steps = GetStepsInBranch(ID);
    for (std::vector<StepBase*>::iterator it=steps.begin();it!=steps.end();it++){
        if ((*it)->Type() == StepBase::kCut) cut_names.push_back((*it)->Title());
    }
    return cut_names;
}

//********************************************************************
std::vector<std::string> SelectionBase::GetStepNames(Int_t ID) const{
//********************************************************************

    std::vector<std::string> step_names;
    std::vector<StepBase*> steps = GetStepsInBranch(ID);
    for (std::vector<StepBase*>::iterator it=steps.begin();it!=steps.end();it++){
        step_names.push_back((*it)->Title());
    }
    return step_names;
}


//********************************************************************
Int_t SelectionBase::GetCutNumber(const std::string& title, Int_t ID) const{
//********************************************************************

    Int_t icut=-1;
    std::vector<StepBase*> steps = GetStepsInBranch(ID);
    for (std::vector<StepBase*>::iterator it=steps.begin();it!=steps.end();it++){
        if ((*it)->Type() == StepBase::kCut) icut ++;
        if ((*it)->Title() == title) break;
    }
    return icut;
}

//********************************************************************
Int_t SelectionBase::GetStepNumber(const std::string& title, Int_t ID) const{
//********************************************************************

  StepBase* step = GetStep(title,ID);
  if (step) return step->Index();
  else{
    std::cout << "ERROR. SelectionBase::GetStepNumber(). Step with title '" << title << "' does not exist in branch "<< ID << std::endl;
    exit(1);
  }
}

//********************************************************************
StepBase* SelectionBase::GetStep(const std::string& title, Int_t ID) const{
//********************************************************************

    std::vector<StepBase*> steps = GetStepsInBranch(ID);
    for (std::vector<StepBase*>::iterator it=steps.begin();it!=steps.end();it++){
        if ((*it)->Title() == title) return *it;
    }
    return NULL;
}

//********************************************************************
StepBase* SelectionBase::GetStepAndBranchID(const std::string& title, Int_t& ID) const{
//********************************************************************


  ID = 0;
  for (std::vector<std::string>::const_iterator it=_branchAlias.begin();it!=_branchAlias.end();it++, ID++){
    StepBase* step = GetStep(title,ID);
    if (step) return step;
  }
  return NULL;
}


//********************************************************************
StepBase* SelectionBase::GetPreviousStep(const std::string& title, Int_t ID) const{
//********************************************************************

  StepBase* step = GetStep(title,ID);
  if (!step){
    std::cout << "ERROR. SelectionBase::GetPreviousStep(). Step with title '" << title << "' does not exist in branch "<< ID << std::endl;
    exit(1);
  }
  if (step->Index()>0)
    return GetStepInBranch(ID, step->Index()-1);
  else
    return _rootStep;
}

//********************************************************************
void SelectionBase::CopySteps(SelectionBase& sel1, UInt_t branchID1, UInt_t first, UInt_t last, const std::vector<UInt_t>& branch2){
//********************************************************************

    // Initialize the input selection
    sel1.Initialize();

    // Get the steps from the first selection
    std::vector<StepBase*> steps1 = sel1.GetStepsInBranch(branchID1, first, last);
    
    // Add a clone of the steps of the first selection to the second selection
    for (UInt_t i=0;i<steps1.size();i++){
      StepBase* step  = steps1[i];
      StepBase* stepC = step->MakeClone();
      if (typeid(*stepC) != typeid(*step)){
        std::cout << "ERROR. SelectionBase::CopySteps(). Error when cloning step '" << step->Title() <<"' in selection of type '" << typeid(*this).name() << "'" << std::endl; 
        std::cout << "    ---> Incompatiple types " << typeid(*stepC).name()  << " and " <<  typeid(*step).name() << std::endl; 
        std::cout << "    ---> Please correct the MakeClone() method of this step" << std::endl; 
        exit(1);
      }
      AddStep(branch2,step->Type(), step->Title(), stepC, step->Break());  
    }

}

//********************************************************************
void SelectionBase::CopySteps(SelectionBase& sel1, UInt_t branchID1, UInt_t first, UInt_t last, Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6, Int_t b7){
//********************************************************************

  CopySteps(sel1, branchID1, first, last, ConvertBranch(b0,b1,b2,b3,b4,b5,b6,b7));

}

//********************************************************************
void SelectionBase::CopySteps(SelectionBase& sel1, const std::string& sbranch1, UInt_t first, UInt_t last, Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6, Int_t b7){
//********************************************************************

  // Initialize the input selection
  sel1.Initialize();

  CopySteps(sel1, sel1.GetBranchUniqueID(sbranch1), first, last, ConvertBranch(b0,b1,b2,b3,b4,b5,b6,b7));
}

//********************************************************************
void SelectionBase::CopySteps(SelectionBase& sel1, const std::string& sbranch1){
//********************************************************************

  // Initialize the input selection
  sel1.Initialize();

  if (sbranch1=="trunk" && sel1.GetNBranches() != 1){
    std::cout << "ERROR. SelectionBase::CopySteps(). Selection of type " << typeid(sel1).name() << " has " << sel1.GetNBranches() 
              << " branches. Branch ID or name must be specified: CopySteps(SelectionBase& sel1, const std::string& sbranch1 ...) " << std::endl;
    exit(1);
  }

  // Copy all steps
  std::vector<StepBase*> steps1 = sel1.GetStepsInBranch(sbranch1);
  CopySteps(sel1,sbranch1,0,steps1.size()-1,0);
}

//********************************************************************
void SelectionBase::CopyStep(SelectionBase& sel1, const std::string& sbranch1, UInt_t istep, Int_t b0, Int_t b1, Int_t b2, Int_t b3, Int_t b4, Int_t b5, Int_t b6, Int_t b7){
//********************************************************************

    // Copy only one step
  CopySteps(sel1,sbranch1,istep,istep,b0,b1,b2,b3,b4,b5,b6,b7);
}

//********************************************************************
void SelectionBase::SetValidRunPeriods(std::string runPeriods){
//********************************************************************

    // First, set them all to false, since they are initialised to true
    for(int i = 0; i < 10; ++i){
        _validRunPeriods[i] = false;
    }
    
    // Now, parse the runPeriods string
    std::stringstream ss(runPeriods);
    int i;

    while (ss >> i){
        // If the selection is valid for this run period then set the corresponding array entry to be true
        _validRunPeriods[i] = true;
        // If the next character in the string is a comma, ignore it
        if (ss.peek() == ',') ss.ignore();
    }
}







