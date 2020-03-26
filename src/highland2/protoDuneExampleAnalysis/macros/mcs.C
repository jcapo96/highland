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

  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleYSize(0.05);
  gStyle->SetLegendTextSize(0.);

  gStyle->SetTitleXOffset(1);
  gStyle->SetTitleYOffset(1);

  

  //seltrk starting pos[2]
  //draw.SetMinY(0.5);
  //draw.SetLogY();
  draw.SetTitleX("start Z position [cm]");
  cout << "SELTRK_POS[2] DATA FIT" << endl; 
  draw.Draw(d,"seltrk_pos[2]",75,0,75,"all","accum_level[0][0]>1","","");
  draw.GetLastHisto()->Fit("gaus","","",-10,50);
  cout << "SELTRK_POS[2] 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_pos[2]",75,0,75,"all","accum_level[0][0]>1","","");
  draw.GetLastHisto()->Fit("gaus","","",-10,50);
  cout << "SELTRK_POS[2] SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_pos[2]",75,0,75,"all","accum_level[0][0]>1","","");
  draw.GetLastHisto()->Fit("gaus","","",10,30);
  cout << "SELTRK_POS[2] FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_pos[2]",75,0,75,"all","accum_level[0][0]>1","","");
  draw.GetLastHisto()->Fit("gaus","","",-10,50);
  draw.Draw(d,"seltrk_pos[2]",75,0,75,"all","accum_level[0][0]>1","hist","","Data run 5387");
  draw.Draw(mc3,"seltrk_pos[2]",75,0,75,"all","accum_level[0][0]>1","hist same","","MCC11 3ms");
  draw.Draw(mcs,"seltrk_pos[2]",75,0,75,"all","accum_level[0][0]>1","hist same","","MCC11 SCE",0.25);
  draw.Draw(mcf,"seltrk_pos[2]",75,0,75,"all","accum_level[0][0]>1","hist same","","MCC11 FLF",2.3);

  if (save_plots) c1->Print("seltrk_pos[2].png");

  c1->WaitPrimitive();

  //seltrk length. First draw each one separetely, fit the peak and afterwards draw all together
  draw.SetTitleX("track length [cm]");
  cout << "LENGTH DATA FIT" << endl; 
  draw.Draw(d,"seltrk_length",150,0,150,"all","accum_level[0][0]>2","","");
  draw.GetLastHisto()->Fit("gaus","","",40,100);
  cout << "LENGTH 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_length",150,0,150,"all","accum_level[0][0]>2","","");
  draw.GetLastHisto()->Fit("gaus","","",60,120);
  cout << "LENGTH SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_length",150,0,150,"all","accum_level[0][0]>2","","");
  draw.GetLastHisto()->Fit("gaus","","",60,120);
  cout << "LENGTH FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_length",150,0,150,"all","accum_level[0][0]>2","","");
  draw.GetLastHisto()->Fit("gaus","","",60,120);

  draw.Draw(d,"seltrk_length",150,0,150,"all","accum_level[0][0]>2","hist","","Data run 5387");
  draw.Draw(mc3,"seltrk_length",150,0,150,"all","accum_level[0][0]>2","hist same","","MCC11 3ms",2.5);
  draw.Draw(mcs,"seltrk_length",150,0,150,"all","accum_level[0][0]>2","hist same","","MCC11 SCE",0.7);
  draw.Draw(mcf,"seltrk_length",150,0,150,"all","accum_level[0][0]>2","hist same","","MCC11 FLF",3);

  if (save_plots) c1->Print("seltrk_length.png");

  c1->WaitPrimitive();

  //length/csdarange
  draw.SetTitleX("length/CSDA");
  cout << "LENGTH/CSDA DATA FIT" << endl; 
  draw.Draw(d,"seltrk_length/seltrk_csdarange_prot_raw",100,0,1.4,"all","accum_level[0][0]>2","","");
  draw.GetLastHisto()->Fit("gaus","","",0.6,1.4);
  cout << "LENGTH/CSDA 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_length/seltrk_csdarange_prot_raw",100,0,1.4,"all","accum_level[0][0]>2","","");
  draw.GetLastHisto()->Fit("gaus","","",0.6,1.4);
  cout << "LENGTH/CSDA SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_prot",100,0,1.4,"all","accum_level[0][0]>2","","");
  draw.GetLastHisto()->Fit("gaus","","",0.6,1.4);
  cout << "LENGTH/CSDA FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_length/seltrk_csdarange_prot_raw",100,0,1.4,"all","accum_level[0][0]>2","","");
  draw.GetLastHisto()->Fit("gaus","","",0.6,1.4);

  draw.SetLegendPos("l");
  draw.Draw(d,  "seltrk_length/seltrk_csdarange_prot_raw",100,0,1.4,"all","accum_level[0][0]>2",      "","","Data run 5387");
  draw.Draw(mc3,"seltrk_length/seltrk_csdarange_prot_raw",100,0,1.4,"all","accum_level[0][0]>2","hist same","","MCC11 3ms",3);
  draw.Draw(mcs,"seltrk_length/seltrk_csdarange_prot"    ,100,0,1.4,"all","accum_level[0][0]>2","hist same","","MCC11 SCE",0.7);
  draw.Draw(mcf,"seltrk_length/seltrk_csdarange_prot_raw",100,0,1.4,"all","accum_level[0][0]>2","hist same","","MCC11 FLF",4);  
  draw.DrawCutLinesVertical(0.69,1.05);

  draw.SetLegendPos("r");
  
  if (save_plots) c1->Print("seltrk_lengthcsda.png");

  c1->WaitPrimitive();

  //endpos[2] for sttoping protons
  draw.SetTitleX("end position Z [cm]");
  cout << "ENDPOS[2] DATA FIT" << endl; 
  draw.Draw(d,"seltrk_endpos[2]",130,20,150,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",20,150);
  cout << "ENDPOS[2] 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_endpos[2]",130,20,150,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",20,150);
  cout << "ENDPOS[2] SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_endpos[2]",130,20,150,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",20,150);
  cout << "ENDPOS[2] FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_endpos[2]",130,20,150,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",20,150);

  draw.Draw(d,  "seltrk_endpos[2]",130,20,150,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","",         "","Data run 5387");
  draw.Draw(mc3,"seltrk_endpos[2]",130,20,150,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","MCC11 3ms",2);
  draw.Draw(mcs,"seltrk_endpos[2]",130,20,150,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6"    ,"hist same","","MCC11 SCE",0.7);
  draw.Draw(mcf,"seltrk_endpos[2]",130,20,150,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","MCC11 FLF",2.5);

  
  if (save_plots) c1->Print("seltrk_endpos[2].png");

  c1->WaitPrimitive();

  //seltrk mom prot
  draw.SetTitleX("proton range momentum [GeV/c]");
  cout << "SELTRK_MOM_PROT DATA FIT" << endl; 
  draw.Draw(d,"seltrk_mom_prot",100,0,2,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0.6,1.4);
  cout << "SELTRK_MOM_PROT 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_mom_prot",100,0,2,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0.6,1.4);
  cout << "SELTRK_MOM_PROT SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_mom_prot",100,0,2,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0.6,1.4);
  cout << "SELTRK_MOM_PROT FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_mom_prot",100,0,2,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0.6,1.4);

  draw.Draw(d,  "seltrk_mom_prot",100,0,2,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","",         "","Data run 5387");
  draw.Draw(mc3,"seltrk_mom_prot",100,0,2,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","MCC11 3ms",2.2);
  draw.Draw(mcs,"seltrk_mom_prot",100,0,2,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6"    ,"hist same","","MCC11 SCE",0.55);
  draw.Draw(mcf,"seltrk_mom_prot",100,0,2,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","MCC11 FLF",3.5);

  
  if (save_plots) c1->Print("seltrk_mom_prot.png");

  c1->WaitPrimitive();

  //KE lenght
  draw.SetTitleX("KE_{length} [GeV]");
  cout << "KE LENGTH DATA FIT" << endl; 
  draw.Draw(d,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE LENGTH 3MS FIT" << endl; 
  draw.Draw(mc3,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE LENGTH SCE FIT" << endl; 
  draw.Draw(mcs,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE LENGTH FLF FIT" << endl; 
  draw.Draw(mcf,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);

  draw.Draw(d,  "sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",100,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6",""         ,"","Data run 5387");
  draw.Draw(mc3,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",100,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","MCC11 3ms",2.2);
  draw.Draw(mcs,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",100,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6",    "hist same","","MCC11 SCE",0.55);
  draw.Draw(mcf,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",100,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","MCC11 FLF",3.5);
    
  if (save_plots) c1->Print("KElength.png");

  c1->WaitPrimitive();
  
  std::string cut = "accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6 && seltrk_hit_dqdx_raw[2]>1 && seltrk_hit_x[2]>-100 && seltrk_hit_x[2]<0";

  //dEdx
  draw.SetTitleX("dE/dx (MeV/cm)");
  cout << "dEdx DATA FIT" << endl; 
  draw.Draw(d,"seltrk_hit_dedx[2]",50,0,15,"all",cut,"","");
  draw.GetLastHisto()->Fit("landau","","",0,15);
  cout << "dEdx 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_hit_dedx[2]",50,0,15,"all",cut,"","");
  draw.GetLastHisto()->Fit("landau","","",0,15);
  cout << "dEdx SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_hit_dedx[2]",50,0,15,"all",cut,"","");
  draw.GetLastHisto()->Fit("landau","","",0,15);
  cout << "dEdx FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_hit_dedx[2]",50,0,15,"all",cut,"","");
  draw.GetLastHisto()->Fit("landau","","",0,15);

  draw.Draw(d,"seltrk_hit_dedx[2]",50,0,15,"all",cut,"hist","","Data run 5387");
  draw.Draw(mc3,"seltrk_hit_dedx[2]",50,0,15,"all",cut,"hist same","","MCC11 3ms",1.8);
  draw.Draw(mcs,"seltrk_hit_dedx[2]",50,0,15,"all",cut,"hist same","","MCC11 SCE",0.5);
  draw.Draw(mcf,"seltrk_hit_dedx[2]",50,0,15,"all",cut,"hist same","","MCC11 FLF",2.7);

  if (save_plots) c1->Print("dedx.png");

  c1->WaitPrimitive();

  //KE calo
  draw.SetTitleX("KE_{calo} [GeV]");
  cout << "KE CALO DATA FIT" << endl; 
  draw.Draw(d,"seltrk_calo[2][3]",50,0,1,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE CALO 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_calo[2][3]",50,0,1,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE CALO SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_calo[2][3]",50,0,1,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE CALO FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_calo[2][3]",50,0,1,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);

  draw.Draw(d,"seltrk_calo[2][3]",50,0,1,"all",cut,"hist","","Data run 5387");
  draw.Draw(mc3,"seltrk_calo[2][3]",50,0,1,"all",cut,"hist same","","MCC11 3ms",1.5);
  draw.Draw(mcs,"seltrk_calo[2][3]",50,0,1,"all",cut,"hist same","","MCC11 SCE",0.5);
  draw.Draw(mcf,"seltrk_calo[2][3]",50,0,1,"all",cut,"hist same","","MCC11 FLF",2.4);
    
  if (save_plots) c1->Print("kecalo.png");

  c1->WaitPrimitive();

  //dEdx_binned[2][0]
  draw.SetTitleX("dE/dx binned[2][0] (MeV/cm)");
  cout << "dEdx BINNED[2][0] DATA FIT" << endl; 
  draw.Draw(d,"seltrk_dedx_binned[2][0]",50,0,20,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,20);
  cout << "dEdx BINNED[2][0] 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_dedx_binned[2][0]",50,0,20,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,20);
  cout << "dEdx BINNED[2][0] SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_dedx_binned[2][0]",50,0,20,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,20);
  cout << "dEdx BINNED[2][0] FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_dedx_binned[2][0]",50,0,20,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,20);

  draw.Draw(d,"seltrk_dedx_binned[2][0]",50,0,20,"all",cut,"hist","","Data run 5387");
  draw.Draw(mc3,"seltrk_dedx_binned[2][0]",50,0,20,"all",cut,"hist same","","MCC11 3ms",1.5);
  draw.Draw(mcs,"seltrk_dedx_binned[2][0]",50,0,20,"all",cut,"hist same","","MCC11 SCE",0.5);
  draw.Draw(mcf,"seltrk_dedx_binned[2][0]",50,0,20,"all",cut,"hist same","","MCC11 FLF",2.4);
    
  if (save_plots) c1->Print("dedxbinned0.png");

  c1->WaitPrimitive();
  
  //dEdx_binned[2][3]
  draw.SetTitleX("dE/dx binned[2][3] (MeV/cm)");
  cout << "dEdx BINNED[2][3] DATA FIT" << endl; 
  draw.Draw(d,"seltrk_dedx_binned[2][3]",50,0,10,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,10);
  cout << "dEdx BINNED[2][3] 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_dedx_binned[2][3]",50,0,10,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,10);
  cout << "dEdx BINNED[2][3] SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_dedx_binned[2][3]",50,0,10,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,10);
  cout << "dEdx BINNED[2][3] FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_dedx_binned[2][3]",50,0,10,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,10);

  draw.Draw(d,"seltrk_dedx_binned[2][3]",50,0,10,"all",cut,"hist","","Data run 5387");
  draw.Draw(mc3,"seltrk_dedx_binned[2][3]",50,0,10,"all",cut,"hist same","","MCC11 3ms");
  draw.Draw(mcs,"seltrk_dedx_binned[2][3]",50,0,10,"all",cut,"hist same","","MCC11 SCE",0.4);
  draw.Draw(mcf,"seltrk_dedx_binned[2][3]",50,0,10,"all",cut,"hist same","","MCC11 FLF",2.1);
    
  if (save_plots) c1->Print("dedxbinned3.png");

  c1->WaitPrimitive();

  //KE calo / KE length
  draw.SetTitleX("KE_{calo}/KE_{length}");
  cout << "BENCHMARK DATA FIT" << endl; 
  draw.Draw(d,"seltrk_calo[2][3]/(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)",25,0,2,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,2);
  cout << "BENCHMARK 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_calo[2][3]/(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)",25,0,2,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,2);
  cout << "BENCHMARK SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_calo[2][3]/(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)",25,0,2,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,2);
  cout << "BENCHMARK FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_calo[2][3]/(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)",25,0,2,"all",cut,"","");
  draw.GetLastHisto()->Fit("gaus","","",0,2);

  draw.Draw(d,"seltrk_calo[2][3]/(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)",25,0,2,"all",cut,"hist","","Data run 5387");
  draw.Draw(mc3,"seltrk_calo[2][3]/(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)",25,0,2,"all",cut,"hist same","","MCC11 3ms",1.2);
  draw.Draw(mcs,"seltrk_calo[2][3]/(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)",25,0,2,"all",cut,"hist same","","MCC11 SCE",0.5);
  draw.Draw(mcf,"seltrk_calo[2][3]/(sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938)",25,0,2,"all",cut,"hist same","","MCC11 FLF",1.8);
    
  if (save_plots) c1->Print("benchmarck.png");

  c1->WaitPrimitive();

  //plot now each kind of energy for every set. First data
  draw.SetTitleX("KE_{Data run 5387} [MeV]");
  cout << "KE TPC BEAM DATA FIT" << endl; 
  draw.Draw(d,"sqrt(pow(beam_mom_tpc,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE CALO DATA FIT" << endl; 
  draw.Draw(d,"seltrk_calo[2][3]",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE LENGTH DATA FIT" << endl; 
  draw.Draw(d,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  
  draw.Draw(d,"sqrt(pow(beam_mom_tpc,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist","","KE TPC beam");
  draw.Draw(d,"seltrk_calo[2][3]",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","KE calo");
  draw.Draw(d,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","KE length");

  if (save_plots) c1->Print("KEcomparativedata.png");

  c1->WaitPrimitive();


  //Second 3ms
  draw.SetTitleX("KE_{MCC11 3ms} [MeV]");
  cout << "KE TPC BEAM 3MS FIT" << endl; 
  draw.Draw(mc3,"sqrt(pow(beam_mom_tpc,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE CALO 3MS FIT" << endl; 
  draw.Draw(mc3,"seltrk_calo[2][3]",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE LENGTH 3MS FIT" << endl; 
  draw.Draw(mc3,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  
  draw.Draw(mc3,"sqrt(pow(beam_mom_tpc,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist","","KE TPC beam");
  draw.Draw(mc3,"seltrk_calo[2][3]",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","KE calo");
  draw.Draw(mc3,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","KE length");

  if (save_plots) c1->Print("KEcomparative3ms.png");

  c1->WaitPrimitive();

  //Second sce
  draw.SetTitleX("KE_{MCC11 SCE} [MeV]");
  cout << "KE TPC BEAM SCE FIT" << endl; 
  draw.Draw(mcs,"sqrt(pow(beam_mom_tpc,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE CALO SCE FIT" << endl; 
  draw.Draw(mcs,"seltrk_calo[2][3]",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE LENGTH SCE FIT" << endl; 
  draw.Draw(mcs,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);

  draw.Draw(mcs,"sqrt(pow(beam_mom_tpc,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6","hist","","KE TPC beam");
  draw.Draw(mcs,"seltrk_calo[2][3]",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6","hist same","","KE calo");
  draw.Draw(mcs,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.6","hist same","","KE length",0.8);

  if (save_plots) c1->Print("KEcomparativesce.png");

  c1->WaitPrimitive();

  //Second flf
  draw.SetTitleX("KE_{MCC11 FLF} [MeV]");
  cout << "KE TPC BEAM FLF FIT" << endl; 
  draw.Draw(mcf,"sqrt(pow(beam_mom_tpc,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE CALO FLF FIT" << endl; 
  draw.Draw(mcf,"seltrk_calo[2][3]",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  cout << "KE LENGTH FLF FIT" << endl; 
  draw.Draw(mcf,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","","");
  draw.GetLastHisto()->Fit("gaus","","",0,1);
  
  draw.Draw(mcf,"sqrt(pow(beam_mom_tpc,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist","","KE TPC beam");
  draw.Draw(mcf,"seltrk_calo[2][3]",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","KE calo");
  draw.Draw(mcf,"sqrt(pow(seltrk_mom_prot,2)+pow(0.938,2))-0.938",50,0,1,"all","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot_raw>0.6","hist same","","KE length");

  if (save_plots) c1->Print("KEcomparativeflf.png");

  c1->WaitPrimitive();
  

}
