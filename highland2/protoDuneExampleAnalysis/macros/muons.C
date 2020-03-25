{


  std::string hostname = gSystem->HostName();

  std::string dir = "/hep/DUNE/DataDir/MicroTrees/";
  if (hostname.find("neutrinos")!=std::string::npos) dir = "/data4/DUNE/DataDir/MicroTrees/";


  std::string fdata    = dir+"data_run5387_dau.root";
  std::string fmcsce   = dir+"mc11_sce_1GeV_dau.root";
  std::string fmc3ms   = dir+"mc11_3ms_1GeV_dau.root";
  std::string fmcflf   = dir+"mc11_flf_1GeV_dau.root";

  TFile *_file0 = TFile::Open(fdata.c_str());
  TFile *_file1 = TFile::Open(fmcsce.c_str());
  TFile *_file2 = TFile::Open(fmc3ms.c_str());
  TFile *_file3 = TFile::Open(fmcflf.c_str());

  TTree* d   = (TTree*)_file0->Get("default");
  TTree* mcs = (TTree*)_file1->Get("default");
  TTree* mc3 = (TTree*)_file2->Get("default");
  TTree* mcf = (TTree*)_file3->Get("default");



  bool save_plots=false;
  
  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleYSize(0.05);
  gStyle->SetLegendTextSize(0.);

  gStyle->SetTitleXOffset(1);
  gStyle->SetTitleYOffset(1);

   //Cut for dedx variables
  std::string cut = "seltrk_hit_dqdx_raw[2]>1 && accum_level[0][2]>3 && seltrk_hit_x[2]>-100 && seltrk_hit_x[2]<0";
 
  // Create a DrawingToolsBase instance initialized with a micro-tree file
  DrawingToolsBase draw(fmcsce,false);

  /*

  gStyle->SetPaperSize(26,26);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadBottomMargin(0.1);
  gStyle->SetPadLeftMargin(0.13);
  */

  //Cuts for muon selection
   // TOF distribution
  //draw.SetLogY(false);

  draw.SetTitleX("beam TOF [ns]");
  draw.Draw(d,"beam_tof",100,0,250,"all","beam_tof>1","HIST","");
  draw.DrawCutLinesVertical(150,170);
  
  c1->WaitPrimitive();

 //angle between beam direction and track direction distribution
  draw.SetMinY(3);
  draw.SetLogY();
  draw.SetTitleX("cos #theta_{#beta}");
  draw.Draw(d,"-0.18*seltrk_dir[0]+-0.20*seltrk_dir[1]+0.96*seltrk_dir[2]",100,0,1,"all","","hist","");
  draw.DrawCutLineVertical(0.93,true,"r");
  
  c1->WaitPrimitive();


  //angle between beam direction and track direction distribution
  draw.SetLegendPos("l");
  draw.SetLegendSize(0.2,0.005);
  draw.SetTitle("");
  draw.SetTitleX("cos #theta_{#beta}");
  draw.Draw(d,mcs,"-0.18*seltrk_dir[0]+-0.20*seltrk_dir[1]+0.96*seltrk_dir[2]",100,0,1,"particle","bamparticle!=-11","hist","AREA");
  draw.DrawCutLineVertical(0.93,true,"r");
  draw.SetLegendPos("r");
    
  c1->WaitPrimitive();

    // Track length distribution
  draw.SetLegendPos("l");
  draw.SetLogY(false);
  draw.SetLegendSize(0.2,0.005);
  draw.SetTitleX("track length [cm]");
  draw.Draw(d,mcs,"seltrk_length",100,0,400,"particle","accum_level[0][2]>2","","AREA");

  if (save_plots) c1->Print("seltrk_length_muon.png");

  c1->WaitPrimitive();
  
  // Track length/CSDA range
  draw.SetTitle("Data");
  draw.SetTitleX("track length/CSDA range");
  draw.Draw(d,"seltrk_length/seltrk_csdarange_muon_raw",100,0,2,"all","accum_level[0][2]>2","histo","");
  
  c1->WaitPrimitive();


  // Track length/CSDA range mc comparative
  draw.SetTitle("");
  draw.SetTitleX("track length/CSDA range");
  draw.SetLegendPos("r");
  draw.Draw(d,  "seltrk_length/seltrk_csdarange_muon_raw",100,0,1.4,"all","accum_level[0][2]>2","histo",     "","data");
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_muon"    ,100,0,1.4,"all","accum_level[0][2]>2","histo same","","MC sce",0.7);
  draw.Draw(mc3,"seltrk_length/seltrk_csdarange_muon"    ,100,0,1.4,"all","accum_level[0][2]>2","histo same","","MC 3ms",3);
  //draw.Draw(mcf,"seltrk_length/seltrk_csdarange_muon"    ,100,0,1.4,"all","accum_level[0][2]>2","histo same","","MC flf",3);

  
  if (save_plots) c1->Print("seltrk_length_csdarange_muon.png");

  c1->WaitPrimitive();

  // Track length/CSDA range ndau class for MCS
  draw.SetTitleX("track length/CSDA range");
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_muon",100,0,2,"ndau","accum_level[0][2]>2","","");

  if (save_plots) c1->Print("seltrk_length_csdarange_muon_ndau.png");

  c1->WaitPrimitive();

  // Proton range momentum
  draw.SetTitle("Data & MC SCE");
   draw.SetTitleX("proton range momentum [GeV/c]");
  draw.Draw(d,mcs,"seltrk_mom_muon",100,0,2,"particle","accum_level[0][2]>3","","AREA");

  if (save_plots) c1->Print("seltrk_length_muon_rangemom.png");

  c1->WaitPrimitive();


  //now dedx variables. First for data.
  
  // dEdx per hit
  draw.SetTitle("Data");
  draw.SetTitleX("dEdx [MeV/cm]");
  draw.Draw(d,"seltrk_hit_dedx[2]",100,0,30,"all",cut.c_str(),"HIST","NOLEG");
  
  if (save_plots) c1->Print("seltrk_dEdx_muon.png");

  c1->WaitPrimitive();

  // Residual range of the selected track
  draw.SetTitleX("Residual Range  [cm]");
  draw.Draw(d,"seltrk_hit_resrange[2]",100,0,300,"all",cut.c_str(),"HIST","NOLEG");

  if (save_plots) c1->Print("seltrk_resrange_muon.png");

  c1->WaitPrimitive();

  // dEdx vs residual range 
  draw.SetTitleX("Residual Range [cm]");
  draw.SetTitleY("dEdx [MeV/cm]");
  draw.Draw(d,"seltrk_hit_dedx[2]:seltrk_hit_resrange[2]",170,1,170,300,1,30,"all",cut,"colz");

  if (save_plots) c1->Print("dEdxVsResRange_muon_data.png");

  c1->WaitPrimitive();

  // dEdx vs residual range for MCs
  draw.SetTitle("MC SCE");
  draw.Draw(mcs,"seltrk_hit_dedx[2]:seltrk_hit_resrange[2]",170,1,170,300,1,30,"all",cut,"colz");
  
  if (save_plots) c1->Print("dEdxVsResRange_muon_mcs.png");

  c1->WaitPrimitive();
  
  draw.SetTitle("MC 3ms");
  draw.Draw(mc3,"seltrk_hit_dedx[2]:seltrk_hit_resrange[2]",170,1,1700,300,1,30,"all",cut,"colz");
  
  if (save_plots) c1->Print("dEdxVsResRange_muon_mc3.png");

  c1->WaitPrimitive();

  //dEdx comparative
  draw.SetTitleX("dEdx [MeV/cm]");
  draw.SetLegendPos("r");
  draw.SetTitle("Data and MC SCE");
  draw.Draw(d,mcs,"seltrk_hit_dedx[2]",100,0,15,"particle",cut.c_str(),"HIST","AREA");

   c1->WaitPrimitive();

   draw.SetTitle("Data and MC 3ms");
  draw.SetTitleX("dEdx [MeV/cm]");
  draw.Draw(d,mc3,"seltrk_hit_dedx[2]",100,0,15,"particle",cut.c_str(),"HIST","AREA");
  
  c1->WaitPrimitive();

  //dedx protons and muons
  draw.SetMarkerStyle(1);
  draw.SetTitle("data");
  draw.SetTitleX("Residual Range [cm]");
  draw.SetTitleY("dEdx [MeV/cm]");
  draw.Draw(d,"seltrk_hit_dedx[2]:seltrk_hit_resrange[2]",100,1,120,300,0,15,"all",cut,"");
  draw.Draw(d,"seltrk_hit_dedx[2]:seltrk_hit_resrange[2]",100,1,120,300,0,15,"all","seltrk_hit_dqdx_raw[2]>1 && accum_level[0][2]>3 && seltrk_hit_x[2]>-100 && seltrk_hit_x[2]<0","same","");

  c1->WaitPrimitive();

  draw.SetTitleX("dEdx [MeV/cm]");
  draw.SetTitleY("");
  draw.Draw(d,"seltrk_hit_dedx[2]",150,0,15,"all","seltrk_hit_dqdx_raw[2]>1 && accum_level[0][2]>3 && seltrk_hit_x[2]>-100 && seltrk_hit_x[2]<0","histo","");
  draw.Draw(d,"seltrk_hit_dedx[2]",150,0,15,"all",cut,"histo same");
 
  c1->WaitPrimitive();



 //energies and benchmark quantities for data
  draw.SetLineWidth(3.);

  draw.SetTitle("Data");
  draw.SetTitleX("Muon Kinetic Energy [GeV]");
  draw.SetLineColor(1);
  draw.Draw(d,"sqrt(pow(beam_mom,2)+pow(0.105,2))-0.105"       ,100,0,2,"all","accum_level[0][2]>3","histo",     "","beam");  
  draw.SetLineColor(4);
  draw.Draw(d,"sqrt(pow(seltrk_mom_muon,2)+pow(0.105,2))-0.105",100,0,2,"all","accum_level[0][2]>3","histo same","","range");
  draw.SetLineColor(2);
  draw.Draw(d,"seltrk_calo[2][3]"                              ,100,0,2,"all","accum_level[0][2]>3","histo same","","calorimetric");

  draw.SetLineWidth(1.);
  draw.UseAutoColors();
  
  c1->WaitPrimitive();

  draw.SetTitleX("KE_{calo}/KE_{beam}");
  draw.Draw(d,"seltrk_calo[2][3]/(sqrt(pow(beam_mom_raw,2)+pow(0.105,2))-0.105)",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{calo}");
  draw.Draw(d,"(sqrt(pow(seltrk_mom_muon,2)+pow(0.105,2))-0.105)/seltrk_calo[2][3]",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{beam}");
  draw.Draw(d,"(sqrt(pow(seltrk_mom_muon,2)+pow(0.105,2))-0.105)/(sqrt(pow(beam_mom_raw,2)+pow(0.105,2))-0.105)",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  //energies and benchmark quantities for mc 3ms
  draw.SetLineWidth(3.);
  draw.SetTitle("mc 3ms");
  draw.SetTitleX("Muonon Kinetic Energy [GeV]");
  draw.SetLineColor(1);
  draw.Draw(mc3,"sqrt(pow(beam_mom,2)+pow(0.105,2))-0.105"       ,100,0,1,"all","accum_level[0][2]>3","histo"     ,"","beam");
  draw.SetLineColor(4);
  draw.Draw(mc3,"sqrt(pow(seltrk_mom_muon,2)+pow(0.105,2))-0.105",100,0,1,"all","accum_level[0][2]>3","histo same","","range");
  draw.SetLineColor(2);
  draw.Draw(mc3,"seltrk_calo[2][3]"                              ,100,0,1,"all","accum_level[0][2]>3","histo same","","calorimetric");

  draw.SetLineWidth(1.);
  draw.UseAutoColors();
  
  c1->WaitPrimitive();

  draw.SetTitleX("KE_{calo}/KE_{beam}");
  draw.Draw(mc3,"seltrk_calo[2][3]/(sqrt(pow(beam_mom,2)+pow(0.105,2))-0.105)",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{calo}");
  draw.Draw(mc3,"(sqrt(pow(seltrk_mom_muon,2)+pow(0.105,2))-0.105)/seltrk_calo[2][3]",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{beam}");
  draw.Draw(mc3,"(sqrt(pow(seltrk_mom_muon,2)+pow(0.105,2))-0.105)/(sqrt(pow(beam_mom,2)+pow(0.105,2))-0.105)",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  //energies and benchmark quantities for mc sce
  draw.SetLineWidth(3.);
  draw.SetTitle("MC SCE");
  draw.SetTitleX("Muonon Kinetic Energy [GeV]");
  draw.SetLineColor(1);
  draw.Draw(mcs,"sqrt(pow(beam_mom,2)+pow(0.105,2))-0.105"       ,100,0,1,"all","accum_level[0][2]>3","histo"     ,"","beam");
  draw.SetLineColor(4);
  draw.Draw(mcs,"sqrt(pow(seltrk_mom_muon,2)+pow(0.105,2))-0.105",100,0,1,"all","accum_level[0][2]>3","histo same","","range");
  draw.SetLineColor(2);
  draw.Draw(mcs,"seltrk_calo[2][3]"                              ,100,0,1,"all","accum_level[0][2]>3","histo same","","calorimetric");

  draw.SetLineWidth(1.);
  draw.UseAutoColors();
  
  c1->WaitPrimitive();

  draw.SetTitleX("KE_{calo}/KE_{beam}");
  draw.Draw(mcs,"seltrk_calo[2][3]/(sqrt(pow(beam_mom,2)+pow(0.105,2))-0.105)",100,0,2,"all",cut,"histo","AREA");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{calo}");
  draw.Draw(mcs,"(sqrt(pow(seltrk_mom_muon,2)+pow(0.105,2))-0.105)/seltrk_calo[2][3]",100,0,2,"all",cut,"histo","AREA");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{beam}");
  draw.Draw(mcs,"(sqrt(pow(seltrk_mom_muon,2)+pow(0.105,2))-0.105)/(sqrt(pow(beam_mom,2)+pow(0.105,2))-0.105)",100,0,2,"all",cut,"histo","AREA");

  c1->WaitPrimitive();
}
