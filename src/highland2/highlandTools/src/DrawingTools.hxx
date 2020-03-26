#ifndef DrawingTools_h
#define DrawingTools_h

#include "DataSample.hxx"
#include "Experiment.hxx"
#include "DrawingToolsBase.hxx"

/// The DrawingTools class provides functions for plotting information in the
/// micro-tree file. The DrawingTools can be used in interactive ROOT sessions
/// and simple ROOT macros.
///
/// There is a distinction between the code for drawing plots (this class,
/// DrawingTools) and the code for selecting the right data to plot
/// (DataSample).
///
/// A file must be specified when instantiating the DrawingTools class, as the
/// file contains important information in the "config" tree, that is used by
/// the DrawingTools (including the cuts that an analysis performed, and the
/// categories used when drawing stacked histograms.
///
/// More documentation on how to use the DrawingTools, including example
/// macros, can be found in the highLevelAnalysis package documentation (follow
/// the links from the main page).
///
/// TODO: This class must be properly documented.
class DrawingTools: public DrawingToolsBase {
public :

  DrawingTools(const std::string& file="", Int_t T2KstyleIndex=0);
  DrawingTools(Experiment& exp, Int_t T2KstyleIndex=0);
  virtual ~DrawingTools(){}
  
  void DrawToys(DataSample& data, const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");
  void DrawToysRatio(DataSample& sample1, DataSample& sample2, const std::string& cut="", 
                     const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="", double norm=-1);
  void DrawToysRatioTwoCuts(DataSample& sample1, DataSample& sample2, const std::string& cut1, const std::string& cut2, 
                            const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="", double norm=-1);


  TH1_h* GetHisto(DataSample& data,const std::string& name, const std::string& var, int nx, double* xbins, 
		 const std::string& cut, const std::string& root_opt, const std::string& opt, double scale, bool scale_errors=true);

  using DrawingToolsBase::GetHisto;
  TH1_h* GetHisto(HistoStack* hs, TTree* tree ,const std::string& name, const std::string& var, int nx, double* xbins, 
		 const std::string& cut, const std::string& root_opt, const std::string& opt, double scale, bool scale_errors=true, int refana=-1);

  using DrawingToolsBase::GetRatioHisto;
  TH1_h* GetRatioHisto(TTree* tree1, TTree* tree2, const std::string& name, const std::string& var, int nx, double* xbins, 
		      const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, double norm, double scale, bool scale_errors=true, int refana=-1);

  using DrawingToolsBase::FillHistoErrors;
  void FillHistoErrors(HistoStack* hs1, HistoStack* hs2, TH1_h* histo, const std::string uopt);

  void FillHistoErrors(HistoStack* hs1, HistoStack* hs2, TTree* tree1, TTree* tree2, const std::string& name, const std::string& var, int nx, double* xbins, 
		        const std::string& cut1, const std::string& cut2, const std::string& opt, double norm, TH1_h* hstat, TH1_h*& hsyst);

  using DrawingToolsBase::UpdateSystInfo;
  void UpdateSystInfo(HistoStack* hs1, HistoStack* hs2, TTree* tree1,TTree* tree2, const std::string& var, int nx, double* xbins,
                      const std::string& cut1, const std::string& cut2, const std::string& opt, double norm);
 
   
  using DrawingToolsBase::FillGraphErrors;
  void FillGraphErrors(HistoStack* hs1, HistoStack* hs2, TGraphAsymmErrors* graph,  const std::string uopt);

  TH1_h* GetHistoWithSystErrors(HistoStack* hs1, HistoStack* hs2, TTree* tree1, TTree* tree2, const std::string& name, const std::string& var, int nx, double* xbins, 
                               const std::string& cut1, const std::string& cut2, const std::string& opt, double norm);

  using DrawingToolsBase::PrintEventNumbers;
  void PrintEventNumbers(DataSample& data, const std::string& cut, const std::string& file="", int refana=-1);


  void Project(HistoStack* hs, const std::string& sample_name, DataSample& sample, const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
	       const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm,  bool scale_errors);

  void Project(HistoStack* hs1, HistoStack* hs2, DataSample* sample1, DataSample* sample2, 
               const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
               const std::string& cut,  const std::string& root_opt, const std::string& opt, bool scale_errors, double norm1=1, double norm2=-1);


  void Project(HistoStack* hs1, HistoStack* hs2, SampleGroup& sampleGroup, const std::string& mcSampleName,  
               const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
               const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors);


  void Project(HistoStack* hs1, HistoStack* hs2, Experiment& exp,  const std::string& groupName, const std::string& mcSampleName, 
               const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
               const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors);


  std::string GetCombinedCut(DataSample& sample, const std::string& cut);
  //---------- Drawing functions -------------

  /// Print on the screen the purities for the different categories defined by "categ" and with cuts "cut".
  /// The expected number of events are also printed, after scaling by events_ratio.
  void PrintPurities(DataSample& data, const std::string& categ,  const std::string& cut, double events_ratio = 1);

  /// Print on the screen the purities for the different categories defined by "categ" and with cuts "cut".
  /// The expected number of events are also printed, after scaling by the data POT in the Experiment.
  void PrintPurities(Experiment& exp, const std::string& categ,  const std::string& cut, const std::string& opt="");


  void Draw(DataSample& data, const std::string& name, int nbins, double* xbins, const std::string& categ="all", 
	    const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1,bool scale_errors=true);
  void Draw(DataSample& data, const std::string& name, int nbins, double xmin, double xmax, const std::string& categ="all", 
	    const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1,bool scale_errors=true);

  void Draw(DataSample& data, const std::string& name, int nx, double* xbins, int ny, double* ybins, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1);
  void Draw(DataSample& data, const std::string& name, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1);

  using DrawingToolsBase::DrawRatio;
  void DrawRatio(DataSample& data, const std::string& name, int nbins, double* xbins, 
		 const std::string& cut1, const std::string& cut2, const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");
  void DrawRatio(DataSample& data, const std::string& name, int nx, double xmin, double xmax, 
		 const std::string& cut1, const std::string& cut2,  const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");

  void DrawEff(DataSample& data, const std::string& name, int nbins, double* xbins, 
	       const std::string& cut1, const std::string& cut2, const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");
  void DrawEff(DataSample& data, const std::string& name, int nx, double xmin, double xmax, 
	       const std::string& cut1, const std::string& cut2, const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");

  void DrawDoubleEff(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double* xbins,
		     const std::string& cut1, const std::string& cut2, const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  
  void DrawDoubleEff(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, 
		     const std::string& cut1, const std::string& cut2, const std::string& root_opt="", const std::string& opt="", const std::string& leg="");


  void DrawSignificance(DataSample& data, const std::string& name, int nbins, double* xbins, const std::string& cut1, const std::string& cut2, 
			double norm=1, double rel_syst=0,const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");
  void DrawSignificance(DataSample& data, const std::string& name, int nbins, double xmin, double xmax, const std::string& cut1, const std::string& cut2, 
			double norm=1, double rel_syst=0,const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");

  // ---------- Sample1/SAMPLE2 comparisons -----------------

  void Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, 
      const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1, bool scale_errors=true);
  void Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nbins, double* xbins,
      const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1,  bool scale_errors=true);

  void Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax,
            const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm1, double norm2, bool scale_errors=true);
  void Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nbins, double* xbins,
            const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm1, double norm2, bool scale_errors=true);
  
  void Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax,
      const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1, bool scale_errors=true);
  void Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double* xbins, int ny, double* ybins,
      const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1,  bool scale_errors=true);

  void Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax,
            const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm1, double norm2,  bool scale_errors=true);
  void Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double* xbins, int ny, double* ybins,
            const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm1, double norm2,  bool scale_errors=true);
  
  
  void DrawRatio(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, 
		 const std::string& cut1, const std::string& cut2, double norm=-1, const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");
  void DrawRatio(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double* xbins, 
		 const std::string& cut1, const std::string& cut2, double norm=-1, const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");

  void DrawRatio(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, 
		 const std::string& cut="",  double norm=-1, const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");
  void DrawRatio(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double* xbins, 
		 const std::string& cut="",  double norm=-1, const std::string& root_opt="", const std::string& opt="", const std::string& leg_name="");


  // ---------- Plots VS cut -----------------
  void DrawEventsVSCut(DataSample& sample , const std::string& cut_norm="", int first_cut=-1, int last_cut=-1, 
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="")
  {ReadOther(sample.GetTree("config"));DrawingToolsBase::DrawEventsVSCut(sample.GetTree(),cut_norm,first_cut,last_cut,root_opt,opt,leg);}

  void DrawEventsVSCut(DataSample& sample , int branch, const std::string& cut_norm="", int first_cut=-1, int last_cut=-1, 
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="")
  {ReadOther(sample.GetTree("config"));DrawingToolsBase::DrawEventsVSCut(sample.GetTree(),branch,cut_norm,first_cut,last_cut,root_opt,opt,leg);}

  void DrawEventsVSCut(DataSample& sample , int isel, int branch, const std::string& cut_norm="", int first_cut=-1, int last_cut=-1, 
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="")
  {ReadOther(sample.GetTree("config"));DrawingToolsBase::DrawEventsVSCut(sample.GetTree(),isel,branch,cut_norm,first_cut,last_cut,root_opt,opt,leg);}

  void DrawEffVSCut(DataSample& sample , const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
                    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  //  {DrawingToolsBase::DrawEffVSCut(sample.GetTree("truth"),signal,precut,first_cut,last_cut,root_opt,opt,leg);}

  void DrawEffVSCut(DataSample& sample , int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
                    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  //  {DrawingToolsBase::DrawEffVSCut(sample.GetTree("truth"),branch,signal,precut,first_cut,last_cut,root_opt,opt,leg);}

  void DrawEffVSCut(DataSample& sample , int isel, int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
                    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  //  {DrawingToolsBase::DrawEffVSCut(sample.GetTree("truth"),isel,branch,signal,precut,first_cut,last_cut,root_opt,opt,leg);}

  void DrawPurVSCut(DataSample& sample , const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="")
  {ReadOther(sample.GetTree("config"));DrawingToolsBase::DrawPurVSCut(sample.GetTree(),signal,precut,first_cut,last_cut,root_opt,opt,leg);}

  void DrawPurVSCut(DataSample& sample , int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="")
  {ReadOther(sample.GetTree("config"));DrawingToolsBase::DrawPurVSCut(sample.GetTree(),branch,signal,precut,first_cut,last_cut,root_opt,opt,leg);}

  void DrawPurVSCut(DataSample& sample , int isel, int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="")
  {ReadOther(sample.GetTree("config"));DrawingToolsBase::DrawPurVSCut(sample.GetTree(),isel,branch,signal,precut,first_cut,last_cut,root_opt,opt,leg);}

  //! [DrawingToolsVsCutsMethods]
  /// Draw the ratio between two trees as a function of the cut
  void DrawRatioVSCut(DataSample& sample1, DataSample& sample2, const std::string& precut="", int first_cut=-1, int last_cut=-1,
                      const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=-1.);
  void DrawRatioVSCut(DataSample& sample1, DataSample& sample2, int branch, const std::string& precut="", int first_cut=-1, int last_cut=-1,
                      const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=-1.);
  void DrawRatioVSCut(DataSample& sample1, DataSample& sample2, int isel, int branch, const std::string& precut="", int first_cut=-1, int last_cut=-1,
                      const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=-1.);



  void DrawEffPurVSCut(DataSample& sample, const std::string& signal, const std::string& precut="", int first_cut=-1, int last_cut=-1,
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawEffPurVSCut(DataSample& sample, int branch, const std::string& signal, const std::string& precut="", int first_cut=-1, int last_cut=-1,
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawEffPurVSCut(DataSample& sample, int isel, int branch, const std::string& signal, const std::string& precut="", int first_cut=-1, int last_cut=-1,
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawEffPurVSCut(DataSample& sample, int isel, int branch, const std::string& signal, const std::string& precut, 
		       int first_cut_pur, int last_cut_pur, int first_cut_eff, int last_cut_eff,
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  //! [DrawingToolsVsCutsMethods]
  
  // ---------- POT -----------------

  //! [DrawingTools_POT]
  /// Dump the POT information for this sample, provided the sample it self
  void DumpPOT(DataSample& data);

  /// Dump the POT information for this sample, provided an Experiment and a sample group name (run1, run2a, run2w, ...)
  void DumpPOT(Experiment& exp, const std::string& samplegroup_name);
  
  /// return the Good POT used to create this DataSample object.
  double GetGoodPOT(DataSample& data);
  
  /// return the number of Good spills used to create this DataSample object.
  int GetGoodSpills(DataSample& data);

  /// Print out the ratio of POTs used to create these DataSample objects.
  /// Ratio is set to 1 if either the numerator or denominator are 0.
  double GetPOTRatio(DataSample& sample1, DataSample& sample2, double POTsample1_byhand=-1);

  /// Print out the ratio of POTs between data and MC in this Experiment
  /// Ratio is set to 1 if either the numerator or denominator are 0.
  double GetPOTRatio(Experiment& exp);

  /// Get the normalisation factor to apply when comparing sample1/sample2. Four possible options
  /// 1. The option NOPOTNORM disables POT normalization (norm=1 is returned)
  /// 2. By default POT normalization ( the POT ratio between the two samples is returned) is used unless a valid normalization factor is given  
  /// 3. When norm>0 and the POTNORM option is given, the second sample is normalized to the POT indicated by norm, regardless of the POT of sample 1
  /// 4. otherwise, if the factor is still negative  don't normalize  
  double GetNormalisationFactor(DataSample* sample1, DataSample* sample2, double norm=-1., const std::string& opt="");

  /// Get the normalisation factor to apply when comparing sample1/sample2. All the above plus a normalisation factor for the first sample that is
  /// also computed
  void GetNormalisationFactor(DataSample* sample1, DataSample* sample2, double& norm1, double& norm2, const std::string& opt="");
  
  //! [DrawingTools_POT]

  // ---------- Plots with Experiments -----------------

  /// This method draws the distribution of the number of selected events for all toys (one entry per toy in the histogram), provided a cut
  /// This is useful to check that the systematic error bars correspond to the RMS of this distribution
  void DrawToys(Experiment& exp, const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  
  /// Data-MC comparison plots using a vector of pairs of data and MC samples (Experiment), properly normalised to each other.  
  void Draw(Experiment& exp, const std::string& var, int nx, double xmin, double xmax, 
            const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="",double norm=-1., bool scale_errors=true);
  void Draw(Experiment& exp, const std::string& var, int nbins, double* xbins, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1., bool scale_errors=true);

  void Draw(Experiment& exp, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="",double norm=-1., bool scale_errors=true);
  void Draw(Experiment& exp, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1., bool scale_errors=true);


  /// Data-MC comparison plots using a single pair of data and MC samples properly normalised to each other.  
  void Draw(Experiment& exp, const std::string& groupName, const std::string& var, int nx, double xmin, double xmax, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="",double norm=-1., bool scale_errors=true);
  void Draw(Experiment& exp, const std::string& groupName, const std::string& var, int nbins, double* xbins, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1., bool scale_errors=true);

  void Draw(Experiment& exp, const std::string& groupName, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="",double norm=-1., bool scale_errors=true);
  void Draw(Experiment& exp, const std::string& groupName, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1., bool scale_errors=true);


  /// Data-MC comparison plots using a single pair of data and MC samples properly normalised to each other.  
  void Draw(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="",double norm=-1., bool scale_errors=true);
  void Draw(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nbins, double* xbins, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1., bool scale_errors=true);

  void Draw(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="",double norm=-1., bool scale_errors=true);
  void Draw(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
	    const std::string& categ="all", const std::string& cut="", const std::string& root_opt="", const std::string& opt="", double norm=-1., bool scale_errors=true);


  /// Compare Data and MC for each sample properly normalised to each other. All samples are drawn using the
  /// "all" category (i.e. as 'data points' in different colours), with a legend created based on the
  /// names given when generating the SampleGroup.
  /// normtype can be POT or ONE
  void CompareSampleGroups(Experiment& exp, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,
      const std::string& cut="", const std::string& root_opt="", const std::string& opt="",bool scale_errors=true, const std::string& normtype="POT");
  void CompareSampleGroups(Experiment& exp, const std::string& mcSampleName, const std::string& var, int nx, double* xbins,
      const std::string& cut="", const std::string& root_opt="", const std::string& opt="",bool scale_errors=true, const std::string& normtype="POT");

  /// data and mc will be FILLED here
  void GetEventsVSCut(Experiment& exp, const std::string& name, const std::string& cut_norm, int isel, int ibranch, int& first_cut, int& last_cut,
           const std::string& root_opt, const std::string& opt, TH1_h*& data, TH1_h*& mc);


  void DrawEventsVSCut(Experiment& exp, const std::string& cut_norm="", int first_cut=-1, int last_cut=-1, 
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawEventsVSCut(Experiment& exp, int branch, const std::string& cut_norm="", int first_cut=-1, int last_cut=-1, 
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawEventsVSCut(Experiment& exp, int isel, int branch, const std::string& cut_norm="", int first_cut=-1, int last_cut=-1, 
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");
  
  void DrawPurVSCut(Experiment& exp, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawPurVSCut(Experiment& exp, int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawPurVSCut(Experiment& exp, int isel, int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawEffVSCut(Experiment& exp, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawEffVSCut(Experiment& exp, int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawEffVSCut(Experiment& exp, int isel, int branch, const std::string& signal="", const std::string& precut="", int first_cut=-1, int last_cut=-1,
		    const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawEffPurVSCut(Experiment& exp , const std::string& signal, const std::string& precut, int first_cut, int last_cut,
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawEffPurVSCut(Experiment& exp , int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
		       const std::string& root_opt, const std::string& opt="", const std::string& leg="");

  void DrawEffPurVSCut(Experiment& exp , int isel, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
		       const std::string& root_opt, const std::string& opt="", const std::string& leg="");

  void DrawEffPurVSCut(Experiment& exp , int isel, int branch, const std::string& signal="", const std::string& precut="", 
		       int first_cut_pur=-1, int last_cut_pur=-1, int first_cut_eff=-1, int last_cut_eff=-1,
		       const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawRatioVSCut(Experiment& exp, const std::string& precut="", int first_cut=-1, int last_cut=-1,
		      const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawRatioVSCut(Experiment& exp, int branch, const std::string& precut="", int first_cut=-1, int last_cut=-1,
		      const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawRatioVSCut(Experiment& exp, int isel, int branch, const std::string& precut="", int first_cut=-1, int last_cut=-1,
		      const std::string& root_opt="", const std::string& opt="", const std::string& leg="");


  void DrawEff(Experiment& exp, bool usedata, const std::string& var, int nx, double xmin, double xmax,
	       const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt = "", const std::string& leg_name = "");
  void DrawEff(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins,
	       const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt = "", const std::string& leg_name = "");

  void DrawEffNew(Experiment& exp, bool usedata, const std::string& var, int nx, double xmin, double xmax,
		  const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt1 = "", const std::string& opt2 = "", 
		  const std::string& leg_name = "");
  void DrawEffNew(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins,
		  const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt1 = "", const std::string& opt2 = "", 
		  const std::string& leg_name = "");

  void DrawRatioNew(Experiment& exp, bool usedata, const std::string& var, int nx, double xmin, double xmax,
		  const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt1 = "", const std::string& opt2 = "");
  void DrawRatioNew(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins,
		  const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt1 = "", const std::string& opt2 = "");

  void DrawPur(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,
	       const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt = "", const std::string& leg_name = "");
  void DrawPur(Experiment& exp, const std::string& var, int nx, double* xbins,
	       const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt = "", const std::string& leg_name = "");

  double GetEff(Experiment& exp, bool usedata,
               const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt = "");
  double GetEff(Experiment& exp, bool usedata, double& errlow, double& errhigh, double& nev1, double& nev2,
               const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt = "");
  TGraphAsymmErrors* GetEff(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins, double& nev1, double& nev2,
                            const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt = "", bool scale_errors = true);
  TGraphAsymmErrors* GetEffNew(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins, double& nev1, double& nev2,
			       const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt1 = "", 
			       const std::string& opt2 = "", bool scale_errors = true);

  void DrawRatioNew(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins, double& nev1, double& nev2,
			       const std::string& cut1, const std::string& cut2, const std::string& root_opt = "", const std::string& opt1 = "", 
			       const std::string& opt2 = "", bool scale_errors = true);

  void DrawRatio(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
		 const std::string& cut,  const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawRatio(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, 
		 const std::string& cut,  const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawRatio(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,
		 const std::string& cut,  const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawRatio(Experiment& exp, const std::string& var, int nx, double* xbins, 
		 const std::string& cut,  const std::string& root_opt="", const std::string& opt="", const std::string& leg="");

  void DrawRatio(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,
		 const std::string& cut,  const std::string& root_opt="", const std::string& opt="", const std::string& leg="");


  void AnalysisResults(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& cut,  const std::string& opt, const std::string& categ="all");

  void AnalysisResults(Experiment& exp, const std::string& cut,  const std::string& opt="", const std::string& categ="all");

  //------------- Draw errors -------------------


  //! [DrawingToolsDrawErrors]  
  // Draw absolute and relative errors for a given selection and binning provided a micro-tree. 
  // The option opt allows chosing statistical or systematics errors (or both). 
  Double_t DrawErrors(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
                      const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=1, bool scale_errors=false);
  Double_t DrawRelativeErrors(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
                              const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=1, bool scale_errors=false);
  Double_t DrawErrors(TTree* tree, const std::string& var, int nx, double* xbins, 
                      const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=1,  bool scale_errors=false);
  Double_t DrawRelativeErrors(TTree* tree, const std::string& var, int nx, double* xbins, 
                              const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=1,  bool scale_errors=false);
  
  // compare two different trees
  Double_t DrawRelativeErrors(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax,
                              const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=1, bool scale_errors=false);
  Double_t DrawRelativeErrors(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, 
                              const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=1,  bool scale_errors=false);
  
  
  
  // Draw absolute and relative errors for a given selection and binning provided an Experiment
  Double_t DrawErrors(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,
                      const std::string& cut="",  const std::string& root_opt="", const std::string& opt="", const std::string& leg="", bool scale_errors=false);
  Double_t DrawRelativeErrors(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,
                              const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", bool scale_errors=false);
  Double_t DrawErrors(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins,
                      const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", bool scale_errors=false);
  
  Double_t DrawRelativeErrors(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins,
                              const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", bool scale_errors=false);
  
  // Draw absolute and relative errors for a given selection and binning provided an Experiment, 
  // and for a given MC sample ("magnet", "sand") in a sample group (i.e. "run3")
  Double_t DrawErrors(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,
                      const std::string& cut="",  const std::string& root_opt="", const std::string& opt="", const std::string& leg="", bool scale_errors=false);
  Double_t DrawRelativeErrors(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,
                              const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", bool scale_errors=false);
  Double_t DrawErrors(Experiment& exp, const std::string& var, int nx, double* xbins,
                      const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", bool scale_errors=false);
  
  Double_t DrawRelativeErrors(Experiment& exp, const std::string& var, int nx, double* xbins,
                              const std::string& cut="", const std::string& root_opt="", const std::string& opt="", const std::string& leg="", bool scale_errors=false);
  //! [DrawingToolsDrawErrors]

  //! [DrawingToolsDrawCovMatrix]
  /// Draw covariance matrix given a tree
  void DrawCovMatrix(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
                     const std::string& cut="", const std::string& root_opt="", const std::string& uopt="");  
  void DrawCovMatrix(TTree* tree, const std::string& var, int nx, double* xbins,
                     const std::string& cut="", const std::string& root_opt="", const std::string& uopt="");
  
  /// Draw covariance matrix given an experiment; and for a given MC sample ("magnet", "sand") in a sample group (i.e. "run3"). MC information will be used
  void DrawCovMatrix(Experiment& exp,const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, 
                     const std::string& cut="", const std::string& root_opt="", const std::string& uopt="");  
  void DrawCovMatrix(Experiment& exp,const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,  
                     const std::string& cut="", const std::string& root_opt="", const std::string& uopt="");
  
  /// Draw covariance matrix given an experiment; MC information will be used
  void DrawCovMatrix(Experiment& exp, const std::string& var, int nx, double* xbins,  
                     const std::string& cut="", const std::string& root_opt="", const std::string& uopt="");  
  void DrawCovMatrix(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,  
                     const std::string& cut="", const std::string& root_opt="", const std::string& uopt="");
  
  /// Returns covariance matrix given a tree
  TMatrixD GetCovMatrix(TTree* tree, const std::string& var, int nx, double* xbins,
                               const std::string& cut="", const std::string& uopt="");
  TMatrixD GetCovMatrix(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
                               const std::string& cut="", const std::string& uopt=""); 
  
  /// Returns covariance matrix given an experiment; and for a given MC sample ("magnet", "sand") in a sample group (i.e. "run3"). MC information will be used
  TMatrixD GetCovMatrix(Experiment& exp,const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,  
                               const std::string& cut="", const std::string& uopt="");
  TMatrixD GetCovMatrix(Experiment& exp,const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins,  
                               const std::string& cut="", const std::string& uopt="");
  
  /// Returns covariance matrix given an experiment; MC information will be used
  TMatrixD GetCovMatrix(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,  
                               const std::string& cut="", const std::string& uopt="");
  TMatrixD GetCovMatrix(Experiment& exp, const std::string& var, int nx, double* xbins, 
                               const std::string& cut="", const std::string& uopt="");

  //! [DrawingToolsDrawCovMatrix]
  
  /// Draw a (covariance) matrix
  void DrawMatrix(const TMatrixD& m, int lc, int lw, int fc, int fs, const std::string& root_opt="", const std::string& opt="");


  
  
  void SetTreeForSystErrors(TTree* tree){_treeForSystErrors= tree;}


 protected:

  Double_t DrawErrorsBase(TTree* tree, const std::string& var, int nx, double* xbins, 
		      const std::string& cut="", bool relative=false, const std::string& root_opt="", const std::string& opt="", const std::string& leg="", double norm=1,  bool scale_errors=false);

  Double_t DrawErrorsBase(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins,
		      const std::string& cut="",  bool relative=false, const std::string& root_opt="", const std::string& opt="", const std::string& leg="", bool scale_errors=false);

  Double_t DrawErrorsBase(HistoStack* hs, bool relative, const std::string& root_opt, const std::string& opt,const std::string& leg);

  HistoStack* GetCovMatrixHistoStack(Experiment& exp,const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins,  
                                     const std::string& cut="", const std::string& uopt="");


  HistoStack* GetCovMatrixHistoStack(TTree* tree, const std::string& var, int nx, double* xbins,
                                     const std::string& cut="", const std::string& uopt="", double norm=1., bool scale_errors=false);



  Int_t GetMinAccumLevelToSave(Experiment& exp, const std::string& uopt="");
  
  TTree* _treeForSystErrors;


};

#endif
