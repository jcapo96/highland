#include <ManagerBase.hxx>
#include <TFile.h>
#include <stdio.h>
#include <iostream>

#include <SystematicBase.hxx>
#include <SelectionBase.hxx>
#include <CategoryClasses.hxx>
#include <CorrectionBase.hxx>
#include <ConfigurationBase.hxx>
#include <DocString.hxx>
#include <VersionManager.hxx>

//********************************************************************
ManagerBase::ManagerBase(const std::string& tree_name, const std::string& vector_name, const std::string& class_name, void* ptr){
//********************************************************************

  _fileName   = "";  
  _treeName   = tree_name;
  _className  = class_name;
  _vectorName = vector_name;

  _pointer    = ptr;
  
  _chain = NULL;
}  

//********************************************************************
ManagerBase::~ManagerBase(){
//********************************************************************

//  delete _chain;
}

//********************************************************************
void ManagerBase::ReadTreeVector(const std::string& file){
//********************************************************************

  // Nothing to do if we read the same file
  if (file == _fileName) return;

  // the current file
  _fileName = file;

  if (!TFile::Open(file.c_str())) return;
  
  // Add the file to the chain
  if (_chain) delete _chain;
  _chain = new TChain(_treeName.c_str());
  _chain->AddFile(file.c_str());

  // Depending on the distribution, ROOT does not allow to set branch addresses with void pointers.
  // This is a walkaround for the moment, although a proper one is needed.
  //_chain->SetBranchAddress(_vectorName.c_str(), &_pointer, &_branch);

  if(_vectorName=="SEL"){
    std::vector<SelectionBase*> *v = static_cast<std::vector<SelectionBase*>*>(_pointer);
    _chain->SetBranchAddress(_vectorName.c_str(), &v, &_branch);
  }
  else if(_vectorName=="SYST"){
    std::vector<SystematicBase*> *v = static_cast<std::vector<SystematicBase*>*>(_pointer);
    _chain->SetBranchAddress(_vectorName.c_str(), &v, &_branch);
  }
  else if(_vectorName=="CATEG"){
    std::map<std::string, TrackCategoryDefinition*> *v = static_cast<std::map<std::string, TrackCategoryDefinition*>*>(_pointer);
    _chain->SetBranchAddress(_vectorName.c_str(), &v, &_branch);
  }
  else if(_vectorName=="CORR"){
    std::vector<CorrectionBase*> *v = static_cast<std::vector<CorrectionBase*>*>(_pointer);
    _chain->SetBranchAddress(_vectorName.c_str(), &v, &_branch);
  }
  else if(_vectorName=="CONF"){
    std::vector<ConfigurationBase* > *v = static_cast<std::vector<ConfigurationBase*>*>(_pointer);
    _chain->SetBranchAddress(_vectorName.c_str(), &v, &_branch);
  }
  else if(_vectorName=="DocString"){
    std::vector<DocString*> *v = static_cast<std::vector<DocString*>*>(_pointer);
    _chain->SetBranchAddress(_vectorName.c_str(), &v, &_branch);
  }
  else if(_vectorName=="VERSION"){
    std::vector<PackageVersion*> *v = static_cast<std::vector<PackageVersion*>*>(_pointer);
    _chain->SetBranchAddress(_vectorName.c_str(), &v, &_branch);
  }
  else
    std::cout << "ManagerBase::SetProperBranchAddress(). unknown class '" << _vectorName << "', exiting now !!!" << std::endl;
  
  // Read one entry
  Long64_t centry = _chain->LoadTree(0);
  if (centry<0){
    std::cout << "ManagerBase::ReadTreeVector(). failed in reading '" << _vectorName << "' vector !!!" << std::endl;
  }

  Int_t nb = _chain->GetEntry(0);
  if (nb==0){
    std::cout << "ManagerBase::ReadTreeVector(). failed in reading '" << _vectorName << "' vector !!!" << std::endl;
  }

  delete _chain; _chain=NULL;
  
}

//********************************************************************
void ManagerBase::WriteTreeVector(TTree& tree){
//********************************************************************

  
  tree.Bronch(_vectorName.c_str(), ("vector<"+_className+"*>").c_str(), &_pointer);
}


//********************************************************************
void ManagerBase::WriteTreeMap(TTree& tree){
//********************************************************************

  
  tree.Bronch(_vectorName.c_str(), ("map<std::string, "+_className+"*>").c_str(), &_pointer);
}

