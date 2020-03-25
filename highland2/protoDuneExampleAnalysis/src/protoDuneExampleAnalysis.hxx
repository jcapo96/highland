#ifndef protoDuneExampleAnalysis_h
#define protoDuneExampleAnalysis_h

#include "baseAnalysis.hxx"
#include "ToyBoxPD.hxx"

/* This is an example of analysis in ProtoDUNE-SP detector 
   This example contains several elements:
     - A simple event selection of kaons decaying at rest from a beam of single kaons at 1GeV. Than means 
       that this example should be run over that particular sample
          /pnfs/dune/scratch/dunepro/v06_05_00/mergeana/gen_protoDune_kaon_1GeV_mono/13405798_0/anahist.root
     - A example of propagation of event variation systematic: dE/dx scaling
     - Filling of a root tree containing summary information about the selection and systematic mentioned above
     - A macro to produce few plots using the root file produced by the executable and the DrawingTools
 */


class protoDuneExampleAnalysis: public baseAnalysis {
 public:
  protoDuneExampleAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~protoDuneExampleAnalysis(){}

  //---- These are mandatory functions
  void DefineSelections();
  void DefineCorrections();
  void DefineSystematics();
  void DefineConfigurations();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  using baseAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);
  //--------------------

  bool Initialize();
  void FillCategories();

  /// Returns the ToyBoxPD
  virtual const ToyBoxPD& box(Int_t isel=-1) const {return *static_cast<const ToyBoxPD*>(&boxB(isel));}

  /// Returns the vertex for the ToyBoxPD
  virtual AnaVertexB* GetVertex() const{return box().Vertex;}

  /// Returns the true vertex for the ToyBoxPD
  virtual AnaTrueVertexB* GetTrueVertex() const {return box().TrueVertex;}


private:

public:

  enum enumStandardMicroTrees_protoDuneExampleAnalysis{
    seltrk_truemom = enumStandardMicroTreesLast_baseAnalysis+1,
    seltrk_truemom_tpc,
    seltrk_trueendmom,
    seltrk_truepdg,
    seltrk_truebeta,
    seltrk_trueproc,
    seltrk_trueendproc,    
    truetrk_pdg,
    truetrk_endproc,
    truetrk_mom,
    truetrk_costheta,
    truetrk_pos,
    truetrk_endpos,
    truetrk_dir,
    seltrk_mom_muon,
    seltrk_mom_prot,
    seltrk_mom_muon_z0,
    seltrk_mom_prot_z0,
    seltrk_length_z0,
    seltrk_pos_z0,
    seltrk_csdarange_muon,
    seltrk_csdarange_prot,
    seltrk_csdarange_muon_raw,
    seltrk_csdarange_prot_raw,
    seltrk_csdarange_tpc_muon,
    seltrk_csdarange_tpc_prot,
    seltrk_theta,
    seltrk_costheta,
    seltrk_broken,
    seltrk_nhits,
    seltrk_length,
    seltrk_ndau,
    seltrk_dqdx,
    seltrk_dedx,
    seltrk_dedx_binned,
    seltrk_dedx_raw,
    seltrk_dedx_exp,
    seltrk_closest_tpc,
    seltrk_detectors,
    seltrk_truepos,
    seltrk_trueendpos,
    seltrk_truedir,
    seltrk_trueeff,
    seltrk_truepur,
    seltrk_dir,
    seltrk_enddir, 
    seltrk_pos,
    seltrk_endpos,
    seltrk_pid,
    seltrk_calo,
    seltrk_pida_raw,
    seltrk_pida_corr,
    seltrk_pida,
    seltrk_pida2,
    seltrk_pdg,
    seltrk_kinetic,
    seltrk_hit_dedx,
    seltrk_hit_dedx_raw,
    seltrk_hit_dedx_cor,
    seltrk_hit_dqdx,
    seltrk_hit_dqdx_raw,
    seltrk_hit_dqdx_cor,
    seltrk_hit_x,
    seltrk_hit_y,
    seltrk_hit_z,
    seltrk_hit_resrange,
    seltrk_nhitsperplane,
    seltrk_dau_mom,
    seltrk_dau_pos,
    seltrk_dau_dir,
    seltrk_dau_endpos,
    seltrk_dau_enddir,
    seltrk_dau_length,
    seltrk_dau_nhits,
    seltrk_dau_nhits2,
    seltrk_dau_hit_dedx,
    seltrk_dau_hit_dqdx_raw,    
    seltrk_dau_hit_resrange,
    seltrk_dau_dedx_binned, 
    seltrk_dau_truepdg,
    seltrk_dau_truepos,
    seltrk_dau_trueendpos,
    seltrk_dau_trueproc,
    seltrk_dau_trueendproc,
    seltrk_ntruedau,
    seltrk_dau_truemom,
    seltrk_dau_trueendmom,

    ntracks,
    trk_truemom,
    trk_truepdg,
    trk_truepos,
    trk_trueproc,
    trk_trueendproc,
    trk_truedir,
    trk_length,
    trk_dedx,
    trk_mom_muon,
    trk_mom_prot,
    trk_pos,
    trk_dir,
    trk_ndau,
    true_signal,
    beam_truepos,
    beam_trueendpos,
    beam_truemom,
    beam_truemom_tpc,
    beam_truedir,
    beam_truepdg,
    beam_trueendproc,
    beam_endpos,          
    beam_enddir,          
    beam_mom,                                 
    beam_mom_raw,                                 
    beam_mom_tpc,                                 
    beam_mom_tpc_raw,                                 
    beam_trigger,      
    beam_tof,
    beam_time,                                 
    beam_ckov_status,  
    beam_ckov_time,    
    beam_ckov_pressure,
    
    enumStandardMicroTreesLast_protoDuneExampleAnalysis
  };

  enum enumConf_protoDuneExampleAnalysis{
    detmass_syst=baseAnalysis::enumConfLast_baseAnalysis+1,    
    dedx_syst,
    enumConfLast_protoDuneExampleAnalysis
  };



};

#endif
