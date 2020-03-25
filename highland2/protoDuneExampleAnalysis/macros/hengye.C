{

  // load input files 
  gROOT->ProcessLine((".L " + string(gSystem->Getenv("PROTODUNEEXAMPLEANALYSISROOT")) + "/macros/load.C").c_str());
  
  bool save_plots=false;
  
  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleYSize(0.05);
  gStyle->SetLegendTextSize(0.);

  gStyle->SetTitleXOffset(1);
  gStyle->SetTitleYOffset(1);

  //Cut for dedx variables
  std::string cut = "seltrk_hit_dqdx_raw[2]>1 && accum_level[0][0]>3 && seltrk_hit_x[2]>-100 && seltrk_hit_x[2]<0";

    
  // Create a DrawingToolsBase instance initialized with a micro-tree file
  DrawingToolsBase draw(fmcsce);

  /*

  gStyle->SetPaperSize(26,26);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadBottomMargin(0.1);
  gStyle->SetPadLeftMargin(0.13);
  */

  //beam mom
  draw.SetLegendSize(0.2,0.005);  
  draw.SetTitleX("beam momentum [GeV/c]");
  draw.Draw(d,mcs,"beam_mom_raw",100,0,2,"beamparticle","","","AREA");

  c1->WaitPrimitive();


  //beam mom
  draw.SetLegendSize(0.2,0.005);  
  draw.SetTitleX("beam momentum [GeV/c]");
  draw.Draw(d,mcs,"beam_mom_raw",100,0,2,"beamparticle","accum_level[0][0]>2","","AREA");

  c1->WaitPrimitive();

  
  // TOF distribution
  //draw.SetLogY(false);

  draw.SetTitleX("beam TOF [ns]");
  draw.Draw(d,"beam_tof",100,0,250,"all","beam_tof>1","HIST","");
  draw.DrawCutLinesVertical(170,210);
  
  c1->WaitPrimitive();

  
  //beam position and seltrk position X
  draw.SetMinY(0.5);
  draw.SetMaxY(5000);
  draw.SetLogY();
  draw.SetLegendSize(0.3,0.02);
  
  draw.SetTitleX("X position [cm]");
  draw.Draw(d,"seltrk_pos[0] ",400,-200,200,"all","beam_tof>170","hist",     "","primary track");
  draw.Draw(d,"beam_endpos[0]",400,-200,200,"all","beam_tof>170","hist same","","beam instrumentation");

    
  c1->WaitPrimitive();

  //beam position and seltrk position Y
  draw.SetTitleX("Y position [cm]");
  draw.Draw(d,"seltrk_pos[1]", 350,250,600,"all","beam_tof>170","hist",     "","primary track");
  draw.Draw(d,"beam_endpos[1]",350,250,600,"all","beam_tof>170","hist same","","beam instrumentation");
  
  c1->WaitPrimitive();

  //beam position and seltrk position Z
  draw.SetTitleX("Z position [cm]");
  draw.Draw(d,"seltrk_pos[2]", 110,-5,105,"all","beam_tof>170","hist",     "","primary track");
  draw.Draw(d,"beam_endpos[2]",110,-5,105,"all","beam_tof>170","hist same","","beam instrumentation");

  draw.SetMaxY();
  
  c1->WaitPrimitive();

  draw.SetMinY(0);
  draw.SetLogY(false);

  // x difference with cut
  draw.SetTitleX("#Deltax [cm]");
  draw.Draw(d,"seltrk_pos[0]-beam_endpos[0] ",400,-50,50,"all","accum_level[0][0]>0 && seltrk_pos[0]-beam_endpos[0]!=0","hist",     "","");
  draw.DrawCutLinesVertical(-5,25);
  
  c1->WaitPrimitive();

  // Y difference with cut
  draw.SetTitleX("#Deltay [cm]");
  draw.Draw(d,"seltrk_pos[1]-beam_endpos[1] ",400,-50,50,"all","accum_level[0][0]>0 && seltrk_pos[1]-beam_endpos[1]!=0","hist",     "","");
  draw.DrawCutLinesVertical(-10,10);
  
  c1->WaitPrimitive();

  // Z pos with cut
  draw.SetTitleX("Z position [cm]");
  draw.Draw(d,"seltrk_pos[2]",400,-50,350,"all","accum_level[0][0]>0 && seltrk_pos[2]!=0","hist",     "","");
  draw.DrawCutLineVertical(100,true,"l");
  
  c1->WaitPrimitive();



    // x difference with cut
  draw.SetTitleX("#Deltax [cm]");
  draw.Draw(d,"trk_pos[][0]-beam_endpos[0] ",400,-50,50,"all","accum_level[0][0]>0 && seltrk_pos[0]-beam_endpos[0]!=0","hist",     "","");
  draw.DrawCutLinesVertical(-5,25);
  
  c1->WaitPrimitive();

  // Y difference with cut
  draw.SetTitleX("#Deltay [cm]");
  draw.Draw(d,"trk_pos[][1]-beam_endpos[1] ",400,-50,50,"all","accum_level[0][0]>0 && seltrk_pos[1]-beam_endpos[1]!=0","hist",     "","");
  draw.DrawCutLinesVertical(-10,10);
  
  c1->WaitPrimitive();

  // Z pos with cut
  draw.SetTitleX("Z position [cm]");
  draw.Draw(d,"trk_pos[][2]",400,-50,350,"all","accum_level[0][0]>0 && seltrk_pos[2]!=0","hist",     "","");
  draw.DrawCutLineVertical(100,true,"l");
  
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
  draw.SetTitleX("cos #theta_{#beta}");
  draw.Draw(d,mcs,"-0.18*seltrk_dir[0]+-0.20*seltrk_dir[1]+0.96*seltrk_dir[2]",100,0,1,"particle","accum_level[0][0]>1","hist","AREA");
  draw.DrawCutLineVertical(0.93,true,"r");



  draw.SetLegendPos("r");
    
  c1->WaitPrimitive();

  
  //track length distribution for different cuts
  draw.SetMinY(0.5);
  draw.SetLogY();
  draw.SetTitleX("seltrk_length (cm)");
  draw.Draw(d,"seltrk_length",500,0,500,"all","beam_tof>170","hist","","before cut");
  draw.Draw(d,"seltrk_length",500,0,500,"all","beam_tof>170 && (-0.18*seltrk_dir[0]+-0.20*seltrk_dir[1]+0.96*seltrk_dir[2])>0.93","hist same","","after angle cut");
  draw.Draw(d,"seltrk_length",500,0,500,"all","accum_level[0][0]>2", "hist same","","Angle Cut + Position Cut");

  c1->WaitPrimitive();

  // Track length distribution
  draw.SetLogY(false);
  draw.SetLegendSize(0.2,0.005);
  draw.SetTitleX("track length [cm]");
  draw.Draw(d,mcs,"seltrk_length",100,0,140.,"particle","accum_level[0][0]>2","","AREA");

  if (save_plots) c1->Print("seltrk_length.png");

  c1->WaitPrimitive();


  // CSDA range
  draw.SetLegendPos("l");
  draw.SetTitleX("CSDA range [cm]");
  draw.Draw(mcs,"seltrk_csdarange_prot",    100,0,140,"particle","accum_level[0][0]>2","","",0.65);
  draw.Draw(d,  "seltrk_csdarange_prot_raw",100,0,140,"all",     "accum_level[0][0]>2","same","","data");
  draw.SetLegendPos("r");
  
  c1->WaitPrimitive();

  
  // Track length/CSDA range
  draw.SetTitleX("track length/CSDA range");
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_prot",    100,0,2,"particle","accum_level[0][0]>2","","",0.8);
  draw.Draw(d,  "seltrk_length/seltrk_csdarange_prot_raw",100,0,2,"all",     "accum_level[0][0]>2","same","","data");
  draw.DrawCutLinesVertical(0.69,1.05);

  c1->WaitPrimitive();


  // Track length/CSDA range
  draw.SetLineWidth(3.);
  draw.SetTitleX("track length/CSDA range");
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_prot",    100,0,2,"all","accum_level[0][0]>2 && seltrk_trueendproc==0","HIST",     "","stopping");
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_prot",    100,0,2,"all","accum_level[0][0]>2 && seltrk_trueendproc!=0","HIST same","","interacting");
  draw.SetLineWidth(1.);
  
  c1->WaitPrimitive();

  

  // Track length/CSDA range mc comparative
  draw.SetTitleX("track length/CSDA range");
  draw.Draw(d,  "seltrk_length/seltrk_csdarange_prot_raw",100,0,1.4,"all","accum_level[0][0]>2","histo",     "","data");
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_prot"    ,100,0,1.4,"all","accum_level[0][0]>2","histo same","","MC sce",0.7);
  draw.Draw(mc3,"seltrk_length/seltrk_csdarange_prot"    ,100,0,1.4,"all","accum_level[0][0]>2","histo same","","MC 3ms",3);
  draw.Draw(mcf,"seltrk_length/seltrk_csdarange_prot"    ,100,0,1.4,"all","accum_level[0][0]>2","histo same","","MC flf",3);

  
  if (save_plots) c1->Print("seltrk_length_csdarange.png");

  c1->WaitPrimitive();

  // Track length/CSDA range ndau class for MCS
  draw.SetTitleX("track length/CSDA range");
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_prot",100,0,2,"ndau","accum_level[0][0]>2","","");

  if (save_plots) c1->Print("seltrk_length_csdarange_ndau.png");

  c1->WaitPrimitive();

  // Proton range momentum
  draw.SetTitleX("proton range momentum [GeV/c]");
  draw.Draw(d,mcs,"seltrk_mom_prot",100,0,2,"particle","accum_level[0][0]>3","","AREA");

  if (save_plots) c1->Print("seltrk_length_prot_rangemom.png");

  c1->WaitPrimitive();


  //now dedx variables. First for data.
  
  // dEdx per hit
  draw.SetTitleX("dEdx [MeV/cm]");
  draw.Draw(d,"seltrk_hit_dedx[2]",100,0,30,"particle",cut.c_str(),"HIST","NOLEG");
  
  if (save_plots) c1->Print("seltrk_dEdx.png");

  c1->WaitPrimitive();

  // Residual range of the selected track
  draw.SetTitleX("Residual Range  [cm]");
  draw.Draw(d,"seltrk_hit_resrange[2]",100,0,120,"particle",cut.c_str(),"HIST","NOLEG");

  if (save_plots) c1->Print("seltrk_resrange.png");

  c1->WaitPrimitive();

  // dEdx vs residual range 
  draw.SetTitleX("Residual Range [cm]");
  draw.SetTitleY("dEdx [MeV/cm]");
  draw.Draw(d,"seltrk_hit_dedx[2]:seltrk_hit_resrange[2]",100,1,120,300,1,30,"all",cut,"colz");

  if (save_plots) c1->Print("dEdxVsResRange_data.png");

  c1->WaitPrimitive();

  // dEdx vs residual range for MCs 
  draw.Draw(mcs,"seltrk_hit_dedx[2]:seltrk_hit_resrange[2]",100,1,120,300,1,30,"all",cut,"colz");
  
  if (save_plots) c1->Print("dEdxVsResRange_mcs.png");

  c1->WaitPrimitive();

  draw.Draw(mc3,"seltrk_hit_dedx[2]:seltrk_hit_resrange[2]",100,1,120,300,1,30,"all",cut,"colz");
  
  if (save_plots) c1->Print("dEdxVsResRange_mc3.png");

  c1->WaitPrimitive();

  //dEdx comparative
  draw.SetTitleX("dEdx [MeV/cm]");
  draw.Draw(d,mcs,"seltrk_hit_dedx[2]",100,0,30,"particle",cut.c_str(),"HIST","AREA");
  
  c1->WaitPrimitive();

  draw.SetTitleX("dEdx [MeV/cm]");
  draw.Draw(d,mc3,"seltrk_hit_dedx[2]",100,0,30,"particle",cut.c_str(),"HIST","AREA");
  
  c1->WaitPrimitive();

  //dedx protons and muons
  draw.SetMarkerStyle(1);
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

  draw.SetTitleX("Proton Kinetic Energy [GeV]");
  draw.SetLineColor(1);
  draw.Draw(d,"sqrt(pow(beam_mom_raw,2)+pow(0.938,2))-0.938"   ,100,0,1,"all","accum_level[0][0]>3","histo",     "","beam");  
  draw.SetLineColor(4);
  draw.Draw(d,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",100,0,1,"all","accum_level[0][0]>3","histo same","","range");
  draw.SetLineColor(2);
  draw.Draw(d,"seltrk_calo[2][3]"                              ,100,0,1,"all","accum_level[0][0]>3","histo same","","calorimetric");

  draw.SetLineWidth(1.);
  draw.UseAutoColors();
  
  c1->WaitPrimitive();

  draw.SetTitleX("KE_{calo}/KE_{beam}");
  draw.Draw(d,"seltrk_calo[2][3]/(sqrt(pow(beam_mom_raw,2)+pow(0.938,2))-0.938)",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{calo}");
  draw.Draw(d,"(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)/seltrk_calo[2][3]",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{beam}");
  draw.Draw(d,"(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)/(sqrt(pow(beam_mom_raw,2)+pow(0.938,2))-0.938)",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  //energies and benchmark quantities for mc 3ms
  draw.SetLineWidth(3.);
  draw.SetTitleX("Proton Kinetic Energy [GeV]");
  draw.SetLineColor(1);
  draw.Draw(mc3,"sqrt(pow(beam_mom,2)+pow(0.938,2))-0.938"       ,100,0,1,"all","accum_level[0][0]>3","histo"     ,"","beam");
  draw.SetLineColor(4);
  draw.Draw(mc3,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",100,0,1,"all","accum_level[0][0]>3","histo same","","range");
  draw.SetLineColor(2);
  draw.Draw(mc3,"seltrk_calo[2][3]"                              ,100,0,1,"all","accum_level[0][0]>3","histo same","","calorimetric");

  draw.SetLineWidth(1.);
  draw.UseAutoColors();
  
  c1->WaitPrimitive();

  draw.SetTitleX("KE_{calo}/KE_{beam}");
  draw.Draw(mc3,"seltrk_calo[2][3]/(sqrt(pow(beam_mom,2)+pow(0.938,2))-0.938)",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{calo}");
  draw.Draw(mc3,"(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)/seltrk_calo[2][3]",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{beam}");
  draw.Draw(mc3,"(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)/(sqrt(pow(beam_mom,2)+pow(0.938,2))-0.938)",100,0,2,"all",cut,"histo","");

  c1->WaitPrimitive();

  //energies and benchmark quantities for mc sce
  draw.SetLineWidth(3.);
  draw.SetTitleX("Proton Kinetic Energy [GeV]");
  draw.SetLineColor(1);
  draw.Draw(mcs,"sqrt(pow(beam_mom,2)+pow(0.938,2))-0.938"       ,100,0,1,"all","accum_level[0][0]>3","histo"     ,"","beam");
  draw.SetLineColor(4);
  draw.Draw(mcs,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",100,0,1,"all","accum_level[0][0]>3","histo same","","range");
  draw.SetLineColor(2);
  draw.Draw(mcs,"seltrk_calo[2][3]"                              ,100,0,1,"all","accum_level[0][0]>3","histo same","","calorimetric");

  draw.SetLineWidth(1.);
  draw.UseAutoColors();
  
  c1->WaitPrimitive();

  draw.SetTitleX("KE_{calo}/KE_{beam}");
  draw.Draw(mcs,"seltrk_calo[2][3]/(sqrt(pow(beam_mom,2)+pow(0.938,2))-0.938)",100,0,2,"all",cut,"histo","AREA");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{calo}");
  draw.Draw(mcs,"(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)/seltrk_calo[2][3]",100,0,2,"all",cut,"histo","AREA");

  c1->WaitPrimitive();

  draw.SetTitleX("KE_{range}/KE_{beam}");
  draw.Draw(mcs,"(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)/(sqrt(pow(beam_mom,2)+pow(0.938,2))-0.938)",100,0,2,"all",cut,"histo","AREA");

  c1->WaitPrimitive();
}
