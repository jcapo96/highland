#include "SystematicTools.hxx"
#include "WeightTools.hxx"
#include <TGraphAsymmErrors.h>
#include <math.h>
#include <TMath.h>

#include <sstream>


/*

//! [SystematicTools_cov]
\htmlonly
Systematic errors are propagated using toy experiments. The covariance for the bins \(i\) and \(j\) reads:

\[
  C_{ij} = \sum_{t=1}^{N_{toys}} [(N_i^t)^W-N_i^{avg}] [(N_j^t)^W-N_j^{avg}]\cdot w^t
\]

In the above expression  \(w^t\) is the toy experiment weight (i.e. the probability of this toy to occur). When random throws are used to generate the toy, it is just:

\[
  w^t = \frac{1}{N_{toys}}
\]

\((N_i^t)^W\) is the number of selected events for toy \(t\) and bin \(i\) once weight systematics (efficiency-like and normalization systematics, 
described above) have been applied (hence the superindex ``W''):

\[
  (N_i^t)^W = \sum_{e=1}^{N_{events}} (W^t)_e \cdot (\delta_i^t)_e
\]


being \((W^t)_e\) the total weight for toy \(t\) and event \(e\). This will be the product of individual weights for each systematic, \((W^t)_e = \prod_{s=1}^{N_s} (W^t)_e^s \). 
The expression \((\delta_i^t)_e\) has value 1 or 0 depending on whether the selection was passed 
for event \(e\) and toy \(t\) and the event felt in bin \(i\). 

Finally, \(N_i^{avg}\) is the average number of events for bin \(i\) when no weight systematics 
where applied: 

\[
  N_i^{avg} = \sum_{t=1}^{N_{toys}} w^t \cdot N_i^t  = \sum_{t=1}^{N_{toys}} w^t \cdot \sum_{e=1}^{N_{events}}  (\delta_i^t)_e 
\]
\endhtmlonly
//! [SystematicTools_cov]
 */

//********************************************************************
SystematicsTools::SystematicsTools(){
  //********************************************************************
  errdebug=false;
}

//*********************************************************
void SystematicsTools::UpdateSystematicCov(HistoStack* hs, TTree* tree, const std::string& var, int nx, double* xbins, const std::string& cut, int NTOYS, const std::string& uopt){
  //*********************************************************

  /* 
     This method allows the use of several trees (in general using Experiment) for computing the systematic covariance. The results for each tree are updated inside a HistoStack. 
  */

  // Create 2Ds histo with the specified variable in the x axis and the toy experiment index in the y axis.
  TH2_h hsyst("hsyst","hsyst",nx,xbins,NTOYS,0,NTOYS);

  std::string cut2=cut;
  if (cut2=="") cut2="1==1";
  
  cut2 = weightTools::ApplyWeights(tree,cut2,uopt);    

  // This is the weighted number of events for each toy experiment t and each bin i: N_i^t
  // In the microtree: t=toy_index, and i is given by the histogram binning
  tree->Project("hsyst",("toy_index:"+var).c_str(),("("+cut2+")").c_str());
 
  // Add it to the HistoStack
  hs->AddSystHisto(&hsyst);
}

//*********************************************************
TMatrixD SystematicsTools::GetSystematicCovBase(HistoStack* hs1, HistoStack* hs2, const std::string& uopt, const std::string& group){
  //*********************************************************

  // 2D histos 
  TH2_h* hsyst;

  // Compute the covariance matrix

  if (group!=""){
    GetSystematicHistos(group,hs1, hs2,hsyst);
    return GetSystematicCovBase(*hsyst,uopt);
  }

  std::vector<std::string> syst_groups = hs2->GetSystHistoGroups();
  std::vector<std::string>::iterator it;
  for (it=syst_groups.begin();it!=syst_groups.end();it++){
    GetSystematicHistos(*it,hs1, hs2,hsyst);
    TMatrixD C = GetSystematicCovBase(*hsyst,uopt);
    if (it==syst_groups.begin()){
      int nx = hsyst->GetNbinsX();
      _covTotal.ResizeTo(nx, nx);
      _covTotal = C;
    }
    else
      _covTotal = _covTotal + C;
  }

  return _covTotal;
}

//*********************************************************
void SystematicsTools::GetSystematicHistos(const std::string& group, HistoStack* hs1, HistoStack* hs2, TH2_h*& hsyst){
  //*********************************************************

  // 2D histos
  TH2_h* hsyst_1;
  TH2_h* hsyst_2;

  if (!hs1){
    hs2->GetSystHisto(group,hsyst);  
    return;
  }

  // Get the systematic histos from the stack
  hs1->GetSystHisto(group,hsyst_1);
  hs2->GetSystHisto(group,hsyst_2);

  TH2_h hsyst_1p(*hsyst_1);

  if (hsyst_2->GetNbinsY() > hsyst_1->GetNbinsY()){
    hsyst_1p = TH2_h(*hsyst_2);
    for (Int_t i=0;i<hsyst_2->GetNbinsX();i++){
      for (Int_t j=0;j<hsyst_2->GetNbinsY();j++){
        hsyst_1p.SetBinContent(i+1,j+1,hsyst_1->GetBinContent(i+1,1));
      }
    }
  }


  // compute the ratio  
  hsyst = new TH2_h(*hsyst_2);
  hsyst->Divide(&hsyst_1p, hsyst_2);
}

//*********************************************************
TMatrixD SystematicsTools::GetSystematicCov(HistoStack* hs1, HistoStack* hs2, const std::string& uopt, const std::string& group){
  //*********************************************************

  if (hs1) return GetRatioSystematicCov(hs1,hs2,uopt,group);
  else     return GetSystematicCov(hs2,uopt,group);  
}

//*********************************************************
TMatrixD SystematicsTools::GetSystematicCov(HistoStack* hs, const std::string& uopt, const std::string& group){
  //*********************************************************
  if(errdebug)    std::cout<<"GetSystematicCov \n============================= "<<std::endl;


  if (uopt.find("DUMMY")!=std::string::npos){
    _cov.ResizeTo(1, 1);
    return _cov;
  }

  /* 
     Compute the covariance matrix from the histos stored in the HistoStack
  */

  TMatrixD cov = GetSystematicCovBase(NULL,hs,uopt,group);
  
  // Compute the covariance matrix
  return cov;
}

//*********************************************************
TMatrixD SystematicsTools::GetRatioSystematicCov(HistoStack* hs1, HistoStack* hs2, const std::string& uopt, const std::string& group){
  //*********************************************************
  if(errdebug)    std::cout<<"GetSystematicCov \n============================= "<<std::endl;

  /* 
     Compute the covariance matrix from the histos stored in the HistoStack
     */

  // Compute the covariance matrix
  return GetSystematicCovBase(hs1,hs2,uopt+" RATIO",group);


  // 2D histos 
  TH2_h* hsyst_1;
  TH2_h* hsyst_2;

  // Get the systematic histos from the stack
  hs1->GetSystHisto("default",hsyst_1);
  hs2->GetSystHisto("default",hsyst_2);

  TH2_h hsyst_1p(*hsyst_1);

  if (hsyst_2->GetNbinsY() > hsyst_1->GetNbinsY()){
    hsyst_1p = TH2_h(*hsyst_2);
    for (Int_t i=0;i<hsyst_2->GetNbinsX();i++){
      for (Int_t j=0;j<hsyst_2->GetNbinsY();j++){
        hsyst_1p.SetBinContent(i+1,j+1,hsyst_1->GetBinContent(i+1,1));
      }
    }
  }


  // compute the ratio
  TH2_h hsyst_ratio(*hsyst_2);
  hsyst_ratio.Divide(&hsyst_1p, hsyst_2);

  // Compute the covariance matrix
  return GetSystematicCovBase(hsyst_ratio,uopt+" RATIO");

}

//*********************************************************
TMatrixD SystematicsTools::GetSystematicCovBase(TH2_h& hsyst,const std::string& uopt){
  //*********************************************************
  //     Compute the covariance matrix from the 1D and 2D histograms 
  /*
     Computes the standard (or variation) systematic covariance matrix for both the method with random throws and the method with binned PDF.
     In fact the only difference between both is the number of toys (larger for throws) and the fact that for throws the weight is the same for all toys 
     while it is different for binned PDF    
     */

  int nx    = hsyst.GetNbinsX();
  int NTOYS = hsyst.GetNbinsY();
  //  int toy_ref=0;  //TODO

  std::vector< std::vector<double> > na;
  std::vector<double> avg;

  std::vector<double> wpdf;
  std::vector<double> na_tot;
  std::vector<double> norm;
  double avg_tot=0;

  na.resize(nx);
  avg.resize(nx);

  na_tot.resize(NTOYS);
  norm.resize(NTOYS);
  wpdf.resize(NTOYS);

  for (int itoy=0;itoy<NTOYS;itoy++){      
    na_tot[itoy]=0;
    norm[itoy]=1.;
    wpdf[itoy]=1./NTOYS;
  }

  for (int i=0;i<nx;i++){
    avg[i]=0;
    na[i].resize(NTOYS);

    for (int itoy=0;itoy<NTOYS;itoy++){      
      // this is the number of events with systematic applied in each bin and toy experiment (N_i^t)_w
      na[i][itoy]=hsyst.GetBinContent(i+1,itoy+1);
      // This is the average number of events (with no systematic weights): N_avg_i = sum_t N_i^t*w^t
      // for weight systematics, for each toy hsyst.GetBinContent is the same
      // for variation system. it is different.
      avg[i]+=hsyst.GetBinContent(i+1,itoy+1)*wpdf[itoy];     

      // Total number of events for this toy experiment
      if (IsValidValue(na[i][itoy]))
        na_tot[itoy]+=na[i][itoy];
    }
    // Average of the total number of events
    avg_tot +=avg[i]; 
  }


  // compute normalization factor for shapeonly systematics
  if (uopt.find("SHAPEONLY")!=std::string::npos){    
    for (int itoy=0;itoy<NTOYS;itoy++){      	
      if (na_tot[itoy]!=0)
        norm[itoy] = avg_tot/na_tot[itoy];
      else
        norm[itoy] = 0.;
    }
  }

  // Give the appropriate dimensions to the output cov matrix
  _cov.ResizeTo(nx, nx);
  for (int i=0;i<nx;i++){
    for (int j=0;j<nx;j++){
      _cov(i,j)=0;      
      for (int itoy=0;itoy<NTOYS;itoy++){	  
        if (!IsValidValue(na[i][itoy])) continue;
        if (!IsValidValue(na[j][itoy])) continue;
        if (na[i][itoy]< 0 || na[j][itoy]<0) continue;
        _cov(i,j) += (norm[itoy]*na[i][itoy]- avg[i])*(norm[itoy]*na[j][itoy]- avg[j])*wpdf[itoy];
        //	std::cout<<itoy<<" wpdf "<<wpdf[itoy]<<std::endl;
        //_cov(i,j) += (na[i][itoy]- avg[i])*(na[j][itoy]- avg[j])*wpdf[itoy];
      }

      // compute relative errors when requested
      if (uopt.find("RELATIVE")!=std::string::npos){
        if (avg[i]*avg[j]!=0){
          _cov(i,j) /= (avg[i]*avg[j]);	 
        }
        else
          _cov(i,j) = 0;
      }	
    }
  }

  return _cov;
}

//*********************************************************
bool SystematicsTools::CheckSystComputed(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, const std::string& cut1, const std::string& cut2, double norm, int NTOYS, const std::string& uopt){
  //*********************************************************

  /* 
     Check whether systematics were already computed for this particular conditions
     */


  if (_syst_computed && _tree1_syst==tree1 && _tree2_syst==tree2 && _var_syst==var && _nbins_syst==nx && _cut_syst1==cut1 && _cut_syst2==cut2 && _norm_syst==norm && _NTOYS_syst==NTOYS && _uopt_syst == uopt){

    bool diff=false;
    for (int i=0;i<=nx;i++){
      if (xbins[i] != _xbins_syst[i]) diff=true;
    }
    if (!diff)
      return true;
  }

  _tree1_syst = tree1;
  _tree2_syst = tree2;
  _var_syst=var;
  _nbins_syst=nx;
  _cut_syst1=cut1;
  _cut_syst2=cut2;
  _norm_syst = norm;
  _NTOYS_syst = NTOYS;
  _uopt_syst=uopt;
  _syst_computed=true;

  for (int i=0;i<=nx;i++){
    _xbins_syst[i] = xbins[i];
  }

  return false;
}

//*********************************************************
bool SystematicsTools::IsValidValue(Double_t value){
  //*********************************************************

  if (!TMath::Finite(value)) return false;
  if (value != value) return false;

  return true;
}
