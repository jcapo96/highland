{

  // load input files 
  gROOT->ProcessLine((".L " + string(gSystem->Getenv("PROTODUNEEXAMPLEANALYSISROOT")) + "/macros/load.C").c_str());

  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleYSize(0.05);
  gStyle->SetTitleXOffset(0.8);
  gStyle->SetTitleYOffset(0.8);


  gStyle->SetLegendTextSize(0.);
 
  // Create a DrawingToolsBase instance initialized with a micro-tree file
  DrawingToolsBase draw(fmcsce,false);
  DrawingTools draw2(fmcsce,false);

  
  // True momentum at TPC entrance
  draw.SetTitleX("true momentum at TPC entrance [GeV/c]");
  draw.Draw(mcs,"seltrk_truemom_tpc",100,0.,1.5,"particle","accum_level[0][0]>2","","");
  c1->WaitPrimitive();



  // True momentum at TPC entrance for non interacting particles
  draw.SetTitle("for stopping particles");
  draw.SetTitleX("true momentum at TPC entrance [GeV/c]");
  draw.Draw(mcs,"seltrk_truemom_tpc",100,0.,1.5,"particle","accum_level[0][0]>2 && seltrk_trueendproc==0","","");
  c1->WaitPrimitive();

   
  // Momentum resolution/bias
  draw.SetStatPos(0.9,0.98);
  draw.SetOptStat(1100);
  draw.SetTitle("for stopping particles");
  draw.SetTitleX("range - true momentum at TPC entrance [GeV/c]");
  draw.Draw(mcs,"seltrk_mom_prot-seltrk_truemom_tpc",100,-0.2,0.5,"particle","accum_level[0][0]>2 && seltrk_trueendproc==0","","");

  c1->WaitPrimitive();
 

  // Momentum resolution/bias  for 
  draw.SetStatPos(0.9,0.98);
  draw.SetOptStat(1100);
  draw.SetTitle("for stopping particles with L/CSDA>0.69");
  draw.SetTitleX("range - true momentum at TPC entrance [GeV/c]");
  draw.Draw(mcs,"seltrk_mom_prot-seltrk_truemom_tpc",100,-0.2,0.5,"particle","accum_level[0][0]>3 && seltrk_trueendproc==0","","");

  c1->WaitPrimitive();
 

  draw.ResetStat();
  
  
  // Track length distribution
  draw.SetLegendSize(0.2,0.005);
  draw.SetTitleX("track length");
  draw.Draw(d,mcs,"seltrk_length",100,0,140.,"particle","accum_level[0][0]>2","","AREA");

  c1->WaitPrimitive();
  
  // Track length/CSDA range
  draw.SetTitleX("track length/CSDA range");
  draw.Draw(d,mcs,"seltrk_length/seltrk_csdarange_prot",100,0,2,"particle","accum_level[0][0]>2","","AREA");

  c1->WaitPrimitive();

  c1->Clear();
  
  c1->Divide(2,2);
  
  c1->cd(1);
  draw.SetTitle("for residual range <5cm");
  draw.SetTitleX("dE/dx [MeV/cm]");
  draw.Draw(d,mcs,"seltrk_dedx_binned[2][0]",100,0,30,"particle","accum_level[0][0]>2","HIST","AREA");

  c1->cd(2);
  draw.SetTitle("for residual range 5-10 cm");
  draw.SetTitleX("dE/dx [MeV/cm]");
  draw.Draw(d,mcs,"seltrk_dedx_binned[2][1]",100,0,30,"particle","accum_level[0][0]>2","HIST","AREA");

  c1->cd(3);
  draw.SetTitle("for residual range 5-15 cm");
  draw.SetTitleX("dE/dx [MeV/cm]");
  draw.Draw(d,mcs,"seltrk_dedx_binned[2][2]",100,0,30,"particle","accum_level[0][0]>2","HIST","AREA");

  c1->cd(4);
  draw.SetTitle("for residual range 15-20 cm");
  draw.SetTitleX("dE/dx [MeV/cm]");
  draw.Draw(d,mcs,"seltrk_dedx_binned[2][3]",100,0,30,"particle","accum_level[0][0]>2","HIST","AREA");

  
  c1->WaitPrimitive();

  c1->Clear();  
  c1->Divide(2,3);

  gStyle->SetTitleSize(0.1);
  gStyle->SetTitleXSize(0.07);
  gStyle->SetTitleYSize(0.07);
  gStyle->SetTitleXOffset(0.6);
  gStyle->SetTitleYOffset(0.6);


  
  c1->cd(1);
  draw.SetTitle("for residual range <5cm,  L/CSDA>0.8");
  draw.SetTitleX("dE/dx [MeV/cm]");
  draw.Draw(d,mcs,"seltrk_dedx_binned[2][0]",100,0,30,"particle","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.8","HIST","AREA");
  
  c1->cd(3);
  draw.SetTitle("for residual range <5cm,  L/CSDA>0.8, endproc==0");
  draw.SetTitleX("dE/dx [MeV/cm]");
  draw.Draw(mcs,"seltrk_dedx_binned[2][0]",100,0,30,"particle","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.8 && seltrk_trueendproc==0","","AREA");

  c1->cd(5);
  draw.SetTitle("for residual range <5cm,  L/CSDA>0.8, endproc!=0");
  draw.SetTitleX("dE/dx [MeV/cm]");
  draw.Draw(mcs,"seltrk_dedx_binned[2][0]",100,0,30,"particle","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot>0.8 && seltrk_trueendproc!=0","","AREA");


  c1->cd(2);
  draw.SetTitle("for residual range <5cm,  L/CSDA<0.5");
  draw.SetTitleX("dE/dx [MeV/cm]");
  draw.Draw(d,mcs,"seltrk_dedx_binned[2][0]",100,0,30,"particle","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot<0.5","HIST","AREA");

  
  c1->cd(4);
  draw.SetTitle("for residual range <5cm,  L/CSDA<0.5, endproc==0");
  draw.SetTitleX("dE/dx [MeV/cm]");
  draw.Draw(mcs,"seltrk_dedx_binned[2][0]",100,0,30,"particle","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot<0.5 && seltrk_trueendproc==0","","AREA");

  c1->cd(6);
  draw.SetTitle("for residual range <5cm,  L/CSDA<0.5, endproc!=0");
  draw.SetTitleX("dE/dx [MeV/cm]");
  draw.Draw(mcs,"seltrk_dedx_binned[2][0]",100,0,30,"particle","accum_level[0][0]>2 && seltrk_length/seltrk_csdarange_prot<0.5 && seltrk_trueendproc!=0","","AREA");

  
  
}
