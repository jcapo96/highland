{

  // load input files 
   gROOT->ProcessLine((".L " + string(gSystem->Getenv("PROTODUNEEXAMPLEANALYSISROOT")) + "/macros/load.C").c_str());
   //std::string fmcs   = "/data4/DUNE/DataDir/MicroTrees/micro_mc11_sce_1GeV_filter_pos_tof0-250.root";
   //std::string fmc3ms   = "/data4/DUNE/DataDir/MicroTrees/mc11_3ms_1GeV_raw.root";
   //std::string fdata = "/data4/DUNE/DataDir/MicroTrees/micro_run5387_1GeV_filter_pos_tof0-250.root";
  
   // TFile *_file0 = TFile::Open(fmcs.c_str());
  //TFile *_file1 = TFile::Open(fmc3ms.c_str());
  //TFile *_file2 = TFile::Open(fdata.c_str());
  //TTree* mcs = (TTree*)_file0->Get("default");
  //  TTree* mc3 = (TTree*)_file1->Get("default");
  //TTree* d = (TTree*)_file2->Get("default");

  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleYSize(0.05);
  gStyle->SetTitleXOffset(0.8);
  gStyle->SetTitleYOffset(0.9);
  gStyle->SetLegendTextSize(0.);
 
  // Create a DrawingToolsBase instance initialized with a micro-tree file
  DrawingToolsBase draw(fmcs,false);

  bool save_plot = false;

   std::string cut = "seltrk_dau_hit_dqdx_raw>1 && accum_level[0][2]>3 && abs(beamparticle)!=11 && seltrk_dau_hit_dedx>0.1 && seltrk_dau_endpos[][2]!=-999 && seltrk_dau_hit_resrange>0";
  // Comparing the start position of daughters track with the end position of selected tracks
  //X position
  draw.SetTitleX("x position [cm]");
  draw.SetTitle("Daughters start position & selected tracks end position (stopping muons)");
  draw.Draw(d,"seltrk_endpos[0]",100,-200,200,"all","seltrk_endpos[0]!=-999 && accum_level[0][2]>3","HIST","","tracks");
  draw.Draw(d,"seltrk_dau_pos[][0]",100,-200,200,"all","seltrk_dau_pos[][0]!=-999 && accum_level[0][2]>3","same HIST","","daughters");
  if (save_plot) c1->Print("xpos_mdau_comp.PNG");

  c1->WaitPrimitive();

  //Y position
   draw.SetTitleX("y position [cm]");
  draw.SetTitle("Daughters start position & selected tracks end position (stoppinng muons)");
  draw.Draw(d,"seltrk_endpos[1]",100,0,600,"all","seltrk_endpos[1]!=-999 && accum_level[0][2]>3","HIST","","tracks");
  draw.Draw(d,"seltrk_dau_pos[][1]",100,0,600,"all","seltrk_dau_pos[][1]!=-999 && accum_level[0][2]>3","same HIST","","daughters");
  if (save_plot) c1->Print("ypos_com.PNG");

  c1->WaitPrimitive();

  //Z position
   draw.SetTitleX("z position [cm]");
  draw.SetTitle("Daughters start position & selected tracks end position (stopping muons)");
  draw.Draw(d,"seltrk_endpos[2]",100,0,500,"all","seltrk_endpos!=-999 && accum_level[0][2]>3","HIST","","tracks");
  draw.Draw(d,"seltrk_dau_pos[][2]",100,0,500,"all","seltrk_dau_pos[][2]!=-999 && accum_level[0][2]>3","same HIST","","daughters");
  if (save_plot) c1->Print("zpos_comp.PNG");
  c1->WaitPrimitive();

  //Length and true momentum of daughters tracks 
  draw.SetTitleX("Track length [cm]");
  draw.SetTitle("No beam e's, accum_level[0][2]>2");
  draw.Draw(d,mcs,"seltrk_dau_length",100,0,300,"particle","abs(beamparticle)!=11 && accum_level[0][2]>2","HIST","area");
  // draw.Draw(mc3,"seltrk_dau_length",100,0,300,"all","beamparticle!=-11 && accum_level[0][2]>2","same HIST","area","MC");
  if (save_plot) c1->Print("dau_length.png");

  c1->WaitPrimitive();
  
  /*draw.SetTitle(" Proton range momentum");
  draw.SetTitleX("Momentum [GeV]");
  draw.Draw(d,"seltrk_dau_mom",100,0,2,"all","",,"HIST","area","Data");
  draw.Draw(mcs,"seltrk_dau_mom",100,0,2,"all","beamparticle!=-11","same HIST","area","MC SCE");
  draw.Draw(mc3,"seltrk_dau_mom",100,0,2,"all","beamparticle!=-11","same HIST","area","MC");
  

  if (save_plot) c1->Print("dau_mom.png");
  
  c1->WaitPrimitive();
  */

  draw.SetTitle("True momentum (No beam e's, stopping muons");
  draw.SetTitleX("Momentum (GeV)");
  draw.Draw(mcs,"seltrk_dau_truemom",20,0,2,"particle","abs(beamparticle)!=11 && accum_level[0][2]>3","HIST","area");
  //  draw.Draw(mc3,"seltrk_dau_truemom",100,2,"all","abs(beamparticle)!=11","same HIST","area","MC");

  
  c1->WaitPrimitive();

  //Number of daughters from seltrk
  
  draw.SetTitle("No beam e's, no showers");
  draw.SetTitleX("# of daughters");
  draw.Draw(d,mcs,"seltrk_ndau",10,0,10,"particle","abs(beamparticle)!=11 && seltrk_dau_endpos[][2]!=-999","HIST","area");
  if (save_plot) c1->Print("ndau_mcs.png");

  c1->WaitPrimitive();
  
  draw.SetTitle("MC");
  //draw.Draw(mc3,"seltrk_ndau",10,0,10,"particle");
  
  // c1->WaitPrimitive();

  //Number of daughters for protons and muons

  draw.SetTitle("no beam e's, no showers, stopping protons");
  draw.Draw(d,mcs,"seltrk_ndau",10,0,10,"particle","accum_level[0][0]>3 && abs(beamparticle)!=11 && seltrk_dau_endpos[][2]!=-999","HIST","area");
  if (save_plot) c1->Print("prot_ndau.png");

  c1->WaitPrimitive();

  draw.SetTitle("no beam e's, no showers, stopping muons");
  draw.Draw(d,mcs,"seltrk_ndau",10,0,10,"particle","accum_level[0][2]>3 && abs(beamparticle)!=11 && seltrk_dau_endpos[][2]!=-999","HIST","area");
  if (save_plot) c1->Print("muon_ndau.png");
  
  c1->WaitPrimitive();

  //dEdx variables for daughters
  draw.SetTitle("");
  draw.SetTitleX("dEdx (MeV/cm)");
  draw.Draw(d,mcs,"seltrk_dau_hit_dedx",100,0,15,"particle","abs(beamparticle)!=11 && seltrk_dau_hit_dedx>0.1 && seltrk_dau_endpos[][2]!=-999 && seltrk_dau_hit_dqdx_raw>1","HIST","area");
  
  c1->WaitPrimitive();

  draw.SetTitle("Proton daughters");
  draw.Draw(d,mcs,"seltrk_dau_hit_dedx",100,0,15,"particle","abs(beamparticle)!=11 && accum_level[0][0]>3 && seltrk_dau_hit_dedx>0.1 && seltrk_dau_hit_dqdx_raw>1","HIST","area");  

  c1->WaitPrimitive();

  draw.SetTitle("Muon daughters");
  draw.Draw(d,mcs,"seltrk_dau_hit_dedx",100,0,15,"particle",cut,"HIST","area");
  if (save_plot) c1->Print("dedx_mdau.png");
  
  c1->WaitPrimitive();

  draw.SetTitle("");
  draw.SetTitleX("Residual range (cm)");
  draw.Draw(d,mcs,"seltrk_dau_hit_resrange",100,0,120,"particle",cut,"HIST","area");
  if (save_plot) c1->Print("resrange_mdau.png");

  c1->WaitPrimitive();

  // dEdx vs residualrange
  draw.SetTitle("Data");
  draw.SetTitleY("dEdx (MeV/cm");
  draw.Draw(d,"seltrk_dau_hit_dedx:seltrk_dau_hit_resrange",100,0,120,100,0,30,"all",cut,"colz");
  if (save_plot) c1->Print("dedx_resrange_mdau_dat.png");

  c1->WaitPrimitive();
  
  draw.SetTitle("MC SCE");
  draw.Draw(mcs,"seltrk_dau_hit_dedx:seltrk_dau_hit_resrange",100,0,120,100,0,30,"all",cut,"colz");
  if (save_plot) c1->Print("dedx_resrange_mdau_mcs.png");

  c1->WaitPrimitive();

   draw.SetTitle("MC");
   //draw.Draw(mc3,"seltrk_dau_hit_ddx[][2]:seltrk_dau_hit_resrange[][2]",100,0,120,300,0,30,"all""seltrk_dau_hit_dedx[][2]>0 && seltrk_dau_hit_resrange[][2]>0 && beamparticle!=-11","colz);

  c1->WaitPrimitive();

  //binned dEdx
  draw.SetTitleY("");
  draw.SetTitleX("dEdx (MeV/cm)");
  draw.SetTitle("data");
  draw.Draw(d,"seltrk_dau_dedx_binned",100,0,15,"all",cut,"HIST","area");

  c1->WaitPrimitive();

  //Creation Process of the daughter
  draw.SetTitle("Process categorie");
  draw.SetTitleX("daughters true process");
  draw.Draw(mcs,"seltrk_dau_trueproc",11,0,11,"process","abs(beamparticle)!=11 && accum_level[0][2]>3 && seltrk_dau_endpos[][2]!=-999");
  if (save_plot) c1->Print("proc_mdau.png");

  c1->WaitPrimitive();

  //End Process of the parent
  draw.SetTitleX("parent true end process")
  draw.Draw(mcs,"seltrk_trueendproc",11,0,11,"process","abs(beamparticle)!=11 && accum_level[0][2]>3 && seltrk_dau_endpos[][2]!=-999");
  
  //True PDG of daughters
  draw.SetLegendSize(0.2,0.005);
  draw.SetTitle("daughters (no showers)");
  draw.SetTitleX("PDG");
  draw.Draw(mcs,"seltrk_dau_truepdg",300,-20,20,"particle","abs(beamparticle)!=11 && accum_level[0][2]>3 && seltrk_dau_pos[][2]!=-999");
  if (save_plot) c1->Print("pdg_mdau.png");

  c1->WaitPrimitive();

  draw.SetTitle("daughters (showers)");
  draw.Draw(mcs,"seltrk_dau_truepdg",300,-20,20,"particle","abs(beamparticle)!=11 && accum_level[0][2]>3");

  c1->WaitPrimitive();

  // Length for daughters of stopping muons with no beam electrons
  draw.SetTitle("");
  draw.SetTitleX("Track length (cm)");
  draw.Draw(d,mcs,"seltrk_dau_length",60,0,60,"particle","abs(beamparticle)!=11 && accum_level[0][2]>3","HIST","area");

  c1->WaitPrimitive();

  draw.Draw(d,mcs,"seltrk_dau_length",60,0,60,"process","abs(beamparticle)!=11 && accum_level[0][2]>3","HIST","area");
  
  c1->WaitPrimitive();

  // dedx of daughters for differet true PDG
  draw.SetTitle("Electrons");
  draw.SetTitleX("dEdx (MeV/cm)");
  draw.Draw(mcs,"seltrk_dau_hit_dedx",100,0,10,"particle","abs(beamparticle)!=11 && abs(seltrk_dau_truepdg)==11 && seltrk_dau_endpos[][2]!=-999 && seltrk_dau_hit_dedx>0.1 && accum_level[0][2]>3");

  c1->WaitPrimitive();

  draw.SetTitle("muons");
  draw.SetTitleX("dEdx (MeV/cm)");
  draw.Draw(mcs,"seltrk_dau_hit_dedx",100,0,10,"process","abs(beamparticle)!=11 && abs(seltrk_dau_truepdg)==13 && seltrk_dau_endpos[][2]!=-999 && seltrk_dau_hit_dedx>0.1 && accum_level[0][2]>3");

  c1->WaitPrimitive();


}
