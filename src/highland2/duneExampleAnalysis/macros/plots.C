{

  // Create a DrawingTools instance initialized with a micro-tree file
  DrawingTools draw("test.root");
  
  // Number of tracks
  draw.SetTitleX("number of tracks");
  draw.Draw(default,"ntracks",50,0,200,"reaction","accum_level>0");
  
  getchar();

  // x starting position of muon candidate 
  draw.SetTitleX("x position (mm)");
  draw.Draw(default,"selmu_pos[0]",50,-5000,5000,"reaction","accum_level>1");

  getchar();

  // average dE/dx of muon candidate after multiplicity cut
  draw.SetTitleX("average dE/dx (MeV/mm)");
  draw.Draw(default,"selmu_dedx",50,0,2,"particle","accum_level>2");
  draw.DrawCutLineVertical(0.5,true,"l");


  getchar();

  // Number of events after each cut
  draw.DrawEventsVSCut(default);
  
  getchar();
  
  // Create a data sample instance with the micro-tree file.
  // Needed to plot efficiency (from truth tree) and purity (from default tree) simultaneously
  DataSample mc("test.root");
  
  // numuCC Efficiency and purity after each cut
  draw.SetTitleY("numu CC eff & purity");
  draw.DrawEffPurVSCut(mc,"reactionCC==1");

  getchar();

  // True muon momentum after all cuts
  draw.SetTitleX("true muon momentum (MeV/c)");
  draw.Draw(default,"selmu_truemom",50,0,10000,"reaction","accum_level>3");

  getchar();

  // Relative systematic error vs true muon momentum after all cuts
  draw.SetTitleX("true muon momentum (MeV/c)");
  draw.DrawRelativeErrors(all_syst,"selmu_truemom",10,0,10000,"accum_level>3","","SYS");

}
