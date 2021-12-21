#ifndef StepBase_h
#define StepBase_h


#include <stdio.h>
#include <iostream>
#include <map>

#include "ToyBoxB.hxx"
#include "CoreDataClasses.hxx"

/// Base class for an event selection step. All user steps should inherit from
/// this class, and implement the Apply() function. The type and title are
/// set when the step is added to the step manager using StepManager::AddStep().
/// Steps can be of type Cut or Action. 
/// Cuts would have to return a meaninful boolean while for actions the boolean will be ignored

class StepBase{
 public :

  /// Enumerator describing the values that _type member can take.
  enum TypeEnum {
    kCut=0,
    kAction,
    kUnknown
  }; //!

  enum BranchStatus{
    BranchOK=0,
    BranchEmpty,
    NoBranches,
    BranchOutOfRange       
  }; //!

  StepBase();
  StepBase(Float_t lower_cut, Float_t upper_cut);
  StepBase(Float_t equal_cut);
  virtual ~StepBase(){}

  StepBase(const StepBase& obj);
  
  //-----------------------------------------------------
  /// MANDATORY FUNCTIONS !!! 

  // Clones this step. Keeps only Type, Title and Break
  virtual StepBase* MakeClone(){std::cout << "ERROR: StepBase::MakeClone() must be implemented in your Cut!!!" << std::endl;exit(1);return NULL;}

  /// The return value should specify whether the step was passed.
  /// For actions the return value is ignored
  virtual bool Apply(AnaEventC& event, ToyBoxB& box) const {(void)event;(void)box;return true;std::cout << "ERROR: StepBase::Apply() must be implemented in your Cut!!!" << std::endl;exit(1);}
  //------------------------------------------------------


  /// Copy the step attributes
  void CopyStep(const StepBase& step, Int_t branch=-1);

  /// Make a clone of all next steps copiying their attributes
  StepBase* MakeCloneRecursive(Int_t branch=-1);
  
  /// Set attributes to the step
  void Configure(StepBase::TypeEnum type, const std::string& title, bool cut_break=false);
  
  /// Set the title of this step, which is the "nice" version of the step name,
  /// used when plotting the step.
  void SetTitle(const std::string& title){_title=title;}

  /// Set the type of this step.
  void SetType(TypeEnum type) {_type = type;}

  /// Set whether the cut sequence should be stopped when this cut fails
  void SetBreak(bool b){_break=b;}

  /// Set the index of this step.
  void SetIndex(Int_t index) {_index = index;}

  /// Return the title of this step.
  const std::string& Title() const{return _title;}

  /// Return the type of this step.
  TypeEnum Type() const{return _type;}

  /// Return true if the cut sequence should be stopped when a cut fails
  bool Break() const{return _break;}

  /// Return the index of this step
  Int_t Index() const{return _index;}

  /// Return cut values
  Int_t LowerCut() const{return _lower_cut;}
  Int_t UpperCut() const{return _upper_cut;}
  Int_t EqualCut() const{return _equal_cut;}

  /// Convert into a string the type of this step
  std::string ConvertType() const;

  /// Disable step in a given branch
  void DisableInBranch(Int_t branch) {_disabledInBranch[branch] = true;}

  /// Enable step in a given branch
  void EnableInBranch(Int_t branch) {_disabledInBranch[branch] = false;}

  /// Returns true if the step is disabled in the specified branch
  bool IsDisabledInBranch(Int_t branch) const {return _disabledInBranch[branch];}

  /// Check if this step has a given Branch Unique ID
  bool HasBranchUniqueID(UInt_t branchID) const;

  /// Get the branch index (index in the vector of next steps) corresponding to a given branch ID
  Int_t BranchIndexForBranchUniqueID(UInt_t branchID) const;
  
  /// Remove all steps with a given branch ID recursively starting at the next steps
  void RemoveStepsInBranchIDRecursive(Int_t branchID);
  
  /// Returns the vector of next steps
  const std::vector<StepBase*>& GetNextSteps() const{return _nextSteps;}

  /// Remove step in a given branch ID from next steps
  void RemoveNextStep(Int_t ID=-1);

  /// Clear the vector of next steps. Does not keep the size !!! 
  void ClearNextSteps();
    
  /// Add a branch unique ID to this step
  /// Although steps are unique and can be shared by different branches we need to store
  /// the branches in which this step is used
  void AddBranchUniqueID(UInt_t ibranch);

  /// Remove a branch unique ID from this step
  void RemoveBranchUniqueID(UInt_t branch);

  /// Computes the branch sequence of subsequent steps provided the branch ID
  void GetBranchSequence(Int_t branchID, std::vector<UInt_t>& branch_seq) const;
  
  /// Returns the vector of branche unique IDs associated to this step
  const std::vector<UInt_t>& GetBranchUniqueIDs() const {return _branchUniqueIDs;}

  /// Add split with nbranches. This create a vector of nbranches components with NULL elements
  void AddBranches(UInt_t nbranches);
  
  /// Add a next step
  /// If no second argument is specified this step will be added to all branches
  /// If this step contains a split we need to specify the branch to which the next step is added
  void AddNextStep(StepBase* step, Int_t branch=-1);

  /// Get the vector of next steps
  void GetNextSteps(std::vector<StepBase*>& steps, bool withSplits=false, Int_t branchID=-1) const;

  /// Get the vector of next steps in a given branch, returns a status code
  BranchStatus GetNextStepsInBranch(std::vector<StepBase*>& steps, Int_t branch) const ;

  /// check that the step and all its suns are correct (it was branch unique IDs defined)
  void ValidateStepRecursive() const;

protected:

  /// The type of this step: cut or action
  TypeEnum _type;

  /// true if the cut sequence should be stopped when a cut fails
  bool _break;

  /// The title of this step, which is used by the DrawingTools.
  std::string _title;

  /// Is the step disabled in a given branch
  std::vector<bool> _disabledInBranch;

  /// The vector of next steps
  std::vector<StepBase*> _nextSteps;

  /// Branch unique IDs associated to this step
  std::vector<UInt_t> _branchUniqueIDs;

  /// Index of the step in the selection
  Int_t _index;

  /// Cut limits for selection inheritance
  Float_t _upper_cut;
  Float_t _lower_cut;
  Float_t _equal_cut;
};


/// A helper class that allows to simply invert a cut
class InvertedStep: public StepBase{
public:
  InvertedStep(StepBase* step){
    if (!step){
      std::cout << " ERROR: InvertedStep " << typeid(*this).name() << " ()  original step is empty,  cannot proceed " << std::endl;
      exit(1);
    }
    _step = step;
  }
  
  virtual ~InvertedStep(){
    //Assume one fully owns an object
    if (_step)
      delete _step;
    _step = NULL;
  }
  
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const{
    return not _step->Apply(event, box);
  }
  StepBase* MakeClone(){ 
    return new InvertedStep(_step->MakeClone());
  }

protected:
  StepBase* _step;
};


#endif
