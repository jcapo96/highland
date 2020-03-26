#ifndef SystematicsTools_h
#define SystematicsTools_h

#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include "TVectorD.h"
#include "TMatrixD.h"
#include "HistoStack.hxx"

/// This class handles computing systematic errors.
///
/// TODO: This class needs to be documented properly.
class SystematicsTools{
 public :

  bool errdebug;

  SystematicsTools();

  virtual ~SystematicsTools(){}
  
  //----------- Functions to compute systematics ------------------

  /// Update the histos necesary to compute the covariance matrix
  void UpdateSystematicCov(HistoStack* hs, TTree* tree, const std::string& var, int nx, double* xbins, const std::string& cut, int NTOYS, const std::string& uopt);

  void GetSystematicHistos(const std::string& group, HistoStack* hs1, HistoStack* hs2, TH2_h*& hsyst);
  
  TMatrixD GetSystematicCovBase(HistoStack* hs1, HistoStack* hs2, const std::string& uopt, const std::string& group);

  TMatrixD GetSystematicCov(HistoStack* hs1, HistoStack* hs2, const std::string& uopt, const std::string& group="");
  TMatrixD GetSystematicCov(HistoStack* hs, const std::string& uopt, const std::string& group="");
  TMatrixD GetRatioSystematicCov(HistoStack* hs1, HistoStack* hs2, const std::string& uopt, const std::string& group="");
  TMatrixD GetSystematicCovBase(TH2_h& hsyst, const std::string& uopt);


  bool CheckSystComputed(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, const std::string& cut1, const std::string& cut2, double norm, int NTOYS, const std::string& uopt);

  bool IsValidValue(Double_t value);

 protected:

  TMatrixD _covTotal;
  TMatrixD _cov;
  bool _syst_computed;

  TTree* _tree1_syst;
  TTree* _tree2_syst;
  std::string _var_syst;
  int _nbins_syst;
  double _xbins_syst[1000];
  std::string _cut_syst1;
  std::string _cut_syst2;
  double _norm_syst;
  int _NTOYS_syst;
  std::string _uopt_syst;

};



#endif


