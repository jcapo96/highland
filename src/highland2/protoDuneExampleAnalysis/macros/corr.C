{

  // load input files 
  gROOT->ProcessLine((".L " + string(gSystem->Getenv("PROTODUNEEXAMPLEANALYSISROOT")) + "/macros/load.C").c_str());

  bool save_plots=false;
  bool wait_canvas=true;
  
  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleYSize(0.05);
  gStyle->SetLegendTextSize(0.);
 
  // Create a DrawingToolsBase instance initialized with a micro-tree file
  DrawingToolsBase draw(fmcsce);
  draw.SetLineWidth(3);

  gStyle->SetTitleXOffset(1);
  gStyle->SetTitleYOffset(1);

  
  //beam mom for data
  draw.SetTitleX("beam momentum [GeV/c]");
  draw.Draw(d,"beam_mom_raw",100,0.4,1.6,"all","accum_level[0][0]>0","hist",     "","Raw");
  draw.Draw(d,"beam_mom",    100,0.4,1.6,"all","accum_level[0][0]>0","hist same","","Corrected");
  draw.Draw(d,"beam_mom_tpc",100,0.4,1.6,"all","accum_level[0][0]>0","hist same","","Corrected at TPC");
  
  if (save_plots) c1->Print("seltrk_pos[2].png");

  if (wait_canvas) c1->WaitPrimitive();

  //beam mom for mc sce
  draw.SetTitleX("beam momentum [GeV/c]");
  draw.Draw(mcs,"beam_truemom",100,0.4,1.6,"all","accum_level[0][0]>0","hist",     "","True");
  draw.Draw(mcs,"beam_mom",    100,0.4,1.6,"all","accum_level[0][0]>0","hist same","","Smeared");
  draw.Draw(mcs,"beam_mom_tpc",100,0.4,1.6,"all","accum_level[0][0]>0","hist same","","Smeared at TPC");
  
  if (save_plots) c1->Print("seltrk_pos[2].png");

  if (wait_canvas) c1->WaitPrimitive();

  //comparative outside tpc
  draw.SetTitleX("beam momentum [GeV/c]");
  draw.Draw(d,  "beam_mom_raw",100,0.4,1.6,"all","accum_level[0][0]>0","",         "","DATA Raw");
  draw.Draw(d,  "beam_mom",100,0.4,1.6,    "all","accum_level[0][0]>0","same",     "","DATA Corrected");
  draw.Draw(mcs,"beam_mom",100,0.4,1.6,    "all","accum_level[0][0]>0","hist same","","MC Smeared",0.58);
    
  if (save_plots) c1->Print("seltrk_pos[2].png");

  if (wait_canvas) c1->WaitPrimitive();

  //comparative inside tpc
  draw.SetTitleX("beam momentum [GeV/c]");
  draw.Draw(d,  "beam_mom_raw",100,0.4,1.6,"all","accum_level[0][0]>0",""         ,"","DATA Raw");
  draw.Draw(d,  "beam_mom_tpc",100,0.4,1.6,"all","accum_level[0][0]>0","same"     ,"","DATA Corrected at TPC");
  draw.Draw(mcs,"beam_mom_tpc",100,0.4,1.6,"all","accum_level[0][0]>0","hist same","","MC Smeared at TPC",0.58);
    
  if (save_plots) c1->Print("seltrk_pos[2].png");

  if (wait_canvas) c1->WaitPrimitive();

  //csdarange comparative outside tpc
  draw.SetTitleX("CSDA range [cm]");
  draw.Draw(d,  "seltrk_csdarange_prot_raw",130,20,150,"all","accum_level[0][0]>2","histo"     ,"","DATA Raw");
  draw.Draw(d,  "seltrk_csdarange_prot"    ,130,20,150,"all","accum_level[0][0]>2","histo same","","DATA Corrected");
  draw.Draw(mcs,"seltrk_csdarange_prot"    ,130,20,150,"all","accum_level[0][0]>2","histo same","","MC Smeared",0.58);
      
  if (save_plots) c1->Print("seltrk_pos[2].png");

  if (wait_canvas) c1->WaitPrimitive();

  //csdarange comparative inside tpc
  draw.SetTitleX("CSDA range [cm]");
  draw.Draw(d,  "seltrk_csdarange_prot_raw",130,20,150,"all","accum_level[0][0]>2","histo"     ,"","DATA Raw");
  draw.Draw(d,  "seltrk_csdarange_tpc_prot",130,20,150,"all","accum_level[0][0]>2","histo same","","DATA Corrected at TPC");
  draw.Draw(mcs,"seltrk_csdarange_tpc_prot",130,20,150,"all","accum_level[0][0]>2","histo same","","MC Smeared at TPC",0.58);
      
  if (save_plots) c1->Print("seltrk_pos[2].png");

  if (wait_canvas) c1->WaitPrimitive();

  //length/csdarange comparative outside 
  draw.SetTitleX("Length/CSDA");
  draw.Draw(d  ,"seltrk_length/seltrk_csdarange_prot_raw",100,0,1.4,"all","accum_level[0][0]>2","histo"     ,"","DATA Raw");
  draw.Draw(d  ,"seltrk_length/seltrk_csdarange_prot"    ,100,0,1.4,"all","accum_level[0][0]>2","histo same","","DATA Corrected");
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_prot"    ,100,0,1.4,"all","accum_level[0][0]>2","histo same","","MC Smeared",0.58);
      
  if (save_plots) c1->Print("seltrk_pos[2].png");

  if (wait_canvas) c1->WaitPrimitive();

  //length/csdarange comparative outside 
  draw.SetTitleX("Length/CSDA");
  draw.Draw(d  ,"seltrk_length/seltrk_csdarange_prot_raw",100,0,1.4,"all","accum_level[0][0]>2","histo"     ,"","DATA Raw");
  draw.Draw(d  ,"seltrk_length/seltrk_csdarange_tpc_prot",100,0,1.4,"all","accum_level[0][0]>2","histo same","","DATA Corrected at TPC");
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_tpc_prot",100,0,1.4,"all","accum_level[0][0]>2","histo same","","MC Smeared at TPC",0.58);
      
  if (save_plots) c1->Print("seltrk_pos[2].png");

  if (wait_canvas) c1->WaitPrimitive();

  //ke from beam outside TPC
  draw.SetTitleX("KE_{beam} [GeV/c]");
  draw.Draw(d  ,"sqrt(pow(beam_mom_raw,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>1","histo"     ,"","DATA Raw");
  draw.Draw(d  ,"sqrt(pow(beam_mom,2)+pow(0.938,2))-0.938"    ,50,0,1,"all","accum_level[0][0]>1","histo same","","DATA Corrected");
  draw.Draw(mcs,"sqrt(pow(beam_mom,2)+pow(0.938,2))-0.938"    ,50,0,1,"all","accum_level[0][0]>1","histo same","","MC Smeared",0.4);
      
  if (save_plots) c1->Print("seltrk_pos[2].png");

  if (wait_canvas) c1->WaitPrimitive();

  //ke from beam inside TPC
  draw.SetTitleX("KE_{beam} [GeV/c]");
  draw.Draw(d  ,"sqrt(pow(beam_mom_raw,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>1","histo"     ,"","DATA Raw");
  draw.Draw(d  ,"sqrt(pow(beam_mom_tpc,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>1","histo same","","DATA Corrected at TPC");
  draw.Draw(mcs,"sqrt(pow(beam_mom_tpc,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>1","histo same","","MC Smeared at TPC",0.4);
      
  if (save_plots) c1->Print("seltrk_pos[2].png");

  if (wait_canvas) c1->WaitPrimitive();
}
