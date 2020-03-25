{

  // load input files 
  gROOT->ProcessLine((".L " + string(gSystem->Getenv("PROTODUNEEXAMPLEANALYSISROOT")) + "/macros/load.C").c_str());

  bool save_plots=false;
  bool wait_canvas=true;
  
  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleYSize(0.05);
  gStyle->SetLegendTextSize(0.);
 
  // Create a DrawingToolsBase instance initialized with a micro-tree file
  DrawingToolsBase draw(fmcsce,false);
  draw.SetLineWidth(1);

  gStyle->SetTitleXOffset(0.8);
  gStyle->SetTitleYOffset(1);



  // Momentum resolution/bias
  draw.SetLineWidth(3);
  draw.SetOptStat(1100);
  draw.SetTitle("for stopping particles");
  draw.SetTitleX("range - true momentum at TPC entrance [GeV/c]");
  draw.SetStatPos(0.9,0.59);
  draw.Draw(mc3,"seltrk_mom_prot-seltrk_truemom_tpc",100,-0.2,0.2,"all","accum_level[0][0]>3 && seltrk_trueendproc==0","hist"     ,"","MCC11 3ms",3);  
  draw.SetStatPos(0.9,0.75);
  draw.Draw(mcs,"seltrk_mom_prot-seltrk_truemom_tpc",100,-0.2,0.2,"all","accum_level[0][0]>3 && seltrk_trueendproc==0","hist sames","","MCC11 SCE",2.5);
  draw.SetStatPos(0.9,0.67);
  draw.Draw(mcf,"seltrk_mom_prot-seltrk_truemom_tpc",100,-0.2,0.2,"all","accum_level[0][0]>3 && seltrk_trueendproc==0","hist sames","","MCC11 FLF",4);  

  //  c1->WaitPrimitive();

  
}
