#include <ManagerBase.hxx>
#include <stdio.h>
#include <iostream>


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

  // Add the file to the chain
  if (_chain) delete _chain;
  _chain = new TChain(_treeName.c_str());
  _chain->AddFile(file.c_str());

  // Set the branch address
  _chain->SetBranchAddress(_vectorName.c_str(), &_pointer, &_branch);
  
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
