#include "ConfigTreeTools.hxx"

//********************************************************************
ConfigTreeTools::ConfigTreeTools(const std::string& file){
//********************************************************************

  Initialize(file);
}

//********************************************************************
ConfigTreeTools::ConfigTreeTools(SystematicManager& syst, ConfigurationManager& conf){
//********************************************************************

  Initialize(syst,conf);
}

//********************************************************************
void ConfigTreeTools::Initialize(SystematicManager& syst, ConfigurationManager& conf){
//********************************************************************

  _fileName="";
  _fromFile=false;
  _syst = &syst;
  _conf = &conf;
}

//********************************************************************
void ConfigTreeTools::Initialize(const std::string& file){
//********************************************************************

  _fromFile=true;
  _fileName = file;
  
  _syst = new SystematicManager();
  _conf = new ConfigurationManager();

  _syst->ReadSystematics(file);
  _conf->ReadConfigurations(file);
}

//********************************************************************
Int_t ConfigTreeTools::GetWeightIndex(const std::string& conf, const std::string& name) const{
//********************************************************************

  if (!_conf->GetConfiguration(conf)) return -1;
  if (_conf->GetConfiguration(conf)->GetEnabledSystematics().size()==0) return -1;

  _syst->DisableAllSystematics();
  _syst->EnableSystematics(_conf->GetConfiguration(conf)->GetEnabledSystematics());

  Int_t nSyst;
  EventWeightBase** systs = _syst->GetWeightSystematics(nSyst);
  Int_t j=0;
  for (int it = 0; it < nSyst; it++) {
    if (systs[it]->IsEnabled()){
      if (systs[it]->Name() == name) return j;
      j++;
    }
  }
  return -1;
}

//********************************************************************
Int_t ConfigTreeTools::GetWeightIndex(Int_t conf, Int_t weight) const{
//********************************************************************

  if (!_conf->GetConfiguration(conf)) return -1;
  if (_conf->GetConfiguration(conf)->GetEnabledSystematics().size()==0) return -1;

  _syst->DisableAllSystematics();
  _syst->EnableSystematics(_conf->GetConfiguration(conf)->GetEnabledSystematics());

  Int_t nSyst;
  EventWeightBase** systs = _syst->GetWeightSystematics(nSyst);
  Int_t j=0;
  for (int it = 0; it < nSyst; it++) {
    if (systs[it]->IsEnabled()){
      if (systs[it]->GetIndex() == weight) return j;
      j++;
    }
  }
  return -1;
}


//********************************************************************
Int_t ConfigTreeTools::GetVarValueI(const std::string& name) const{
//********************************************************************

  if (_fileName==""){
    std::cout << "no file and tree associated yet !!!" << std::endl;
    return -999;
  }

  
  TChain* chain = new TChain("config");
  chain->AddFile(_fileName.c_str());    
  Int_t var=0;
  if (chain->FindLeaf(name.c_str())){
    chain->SetBranchAddress(name.c_str(), &var);
    Long64_t centry = chain->LoadTree(0);
    Int_t nb = chain->GetEntry(0);
    (void) centry;
    (void) nb;
  }
  return var;

}

//********************************************************************
Float_t ConfigTreeTools::GetVarValueF(const std::string& name) const{
//********************************************************************

  if (_fileName==""){
    std::cout << "no file and tree associated yet !!!" << std::endl;
    return -999;
  }
  
  TChain* chain = new TChain("config");
  chain->AddFile(_fileName.c_str());    
  Float_t var=0;
  if (chain->FindLeaf(name.c_str())){
    chain->SetBranchAddress(name.c_str(), &var);
    Long64_t centry = chain->LoadTree(0);
    Int_t nb = chain->GetEntry(0);
    (void) centry;
    (void) nb;
  }
  return var;


}

//********************************************************************
Double_t ConfigTreeTools::GetVarValueD(const std::string& name) const{
//********************************************************************

  if (_fileName==""){
    std::cout << "no file and tree associated yet !!!" << std::endl;
    return -999;
  }

  
  TChain* chain = new TChain("config");
  chain->AddFile(_fileName.c_str());    
  Double_t var=0;
  if (chain->FindLeaf(name.c_str())){
    chain->SetBranchAddress(name.c_str(), &var);
    Long64_t centry = chain->LoadTree(0);
    Int_t nb = chain->GetEntry(0);
    (void) centry;
    (void) nb;
  }
  return var;
}

//********************************************************************
std::string ConfigTreeTools::GetVarValueC(const std::string& name) const{
//********************************************************************

  if (_fileName==""){
    std::cout << "no file and tree associated yet !!!" << std::endl;
    return "";
  }
  
  TChain* chain = new TChain("config");
  chain->AddFile(_fileName.c_str());    
  char var[200]="";
  if (chain->FindLeaf(name.c_str())){
    chain->SetBranchAddress(name.c_str(), var);
    Long64_t centry = chain->LoadTree(0);
    Int_t nb = chain->GetEntry(0);
    (void) centry;
    (void) nb;
  }

  return std::string(var);
}

//********************************************************************
std::string ConfigTreeTools::GetWeightName(const std::string& conf, Int_t weight_nbr) const{
//********************************************************************

  if (!_conf->GetConfiguration(conf)) return "";
  if (_conf->GetConfiguration(conf)->GetEnabledSystematics().size()==0) return "";

  _syst->DisableAllSystematics();
  _syst->EnableSystematics(_conf->GetConfiguration(conf)->GetEnabledSystematics());

  Int_t nSyst;
  EventWeightBase** systs = _syst->GetWeightSystematics(nSyst);
  Int_t j=0;
  for (int it = 0; it < nSyst; it++) {
    if (systs[it]->IsEnabled()){
      if (weight_nbr == j) return systs[it]->GetName();
      j++;
    }
  }
  return "";
}

//********************************************************************
Int_t ConfigTreeTools::GetNWeights(const std::string& conf) const{
//********************************************************************

  if (!_conf->GetConfiguration(conf)) return -1;
  if (_conf->GetConfiguration(conf)->GetEnabledSystematics().size()==0) return 0;

  _syst->DisableAllSystematics();
  _syst->EnableSystematics(_conf->GetConfiguration(conf)->GetEnabledSystematics());

  Int_t nSyst;
  EventWeightBase** systs = _syst->GetWeightSystematics(nSyst);
  Int_t nEnabled=0;
  for (int it = 0; it < nSyst; it++) {
    if (systs[it]->IsEnabled()){
      nEnabled++;
    }
  }

  return nEnabled;
}
