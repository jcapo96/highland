#include "DUNEAnalysisUtils.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "BaseDataClasses.hxx"
#include "DetectorDefinition.hxx"
#include "PionInteractionSystematic.hxx"
#include "Parameters.hxx"

//DUNEBeamBunching bunching;


//**************************************************
Int_t anaUtils::GetRunPeriod(Int_t run){
//**************************************************

    if      (             run<6000)   return 0;//run1  water                
    else if (run>=6462  && run<7664)  return 1;//run2  water                
    else if (run>=7665  && run<7754)  return 2;//run2  air                  
    else if (run>=8000  && run<8550)  return 3;//run3b air => mc associated to be the one of run2                  
    else if (run>=8550  && run<8800)  return 4;//run3c air                  
    else if (run>=8983  && run<9414)  return 5;//run4  water                
    else if (run>=9426  && run<9800)  return 6;//run4  air                  
    else if ((run>=10252 && run<10334)              //                    + 10252_10 to 10333
	     || (run>=10519 && run<10522)) return 7;//run5  water         + 10518_28 to 10521_13             
    else if (run>=10334 && run<10519) return 8;//run5  water (ANTINU) up to 10334_11 to 10518_9    
    else if (run>=10954 && run<11688) return 9;//run6  air (ANTINU)       

    else if (run>=90110000 && run<=90119999) return 0;//run1 water          
    else if (run>=90210000 && run<=90219999) return 1;//run2 water          
    else if (run>=90200000 && run<=90209999) return 2;//run2 air            
    else if (run>=90300000 && run<=90300015) return 3;//run3b air separated based on the pot of data (run3b/(run3b+run3c)=0.13542             
    else if (run>=90300016 && run<=90300110) return 4;//run3c air separated based on the pot of data            
    else if (run>=90410000 && run<=90419999) return 5;//run4 water          
    else if (run>=90400000 && run<=90409999) return 6;//run4 air            
    else if (run>=80510000 && run<=80519999) return 8;//run5 antinu-water
    else if (run>=80600000 && run<=80600159) return 9;//run6b antinu-air
    else if (run>=80600160 && run<=80600219) return 10;//run6c antinu-air - have to split Run 6 due to different flux tunings for the different parts
    else if (run>=80600220 && run<=80600299) return 11;//run6d antinu-air
    else if (run>=80600300 && run<=80600399) return 12;//run6e antinu-air
    else if (run>=80600400 && run<=80609999) return 12;//run6e antinu-air - set this as default, to catch any files that are not currently at TRIUMF

    else if (run>=80307000 && run<=80307999) return 8;//sand muons antinu
    else if (run>=90307000 && run<=90307999) return 4;//sand muons neut
    else if (run>=92307000 && run<=92307999) return 4;//sand muons old neut

    // genie
    else if (run>=91110000 && run<=91119999) return 0;//run1 water   
    else if (run>=91210000 && run<=91219999) return 1;//run2 water          
    else if (run>=91200000 && run<=91209999) return 2;//run2 air            
    else if (run>=91300000 && run<=91300015) return 3;//run3 air separated based on the pot of data (run3b/(run3b+run3c)=0.13542             
    else if (run>=91300016 && run<=91300110) return 4;//run3 air separated based on the pot of data (run3b/(run3b+run3c)=0.13542            
    else if (run>=91410000 && run<=91419999) return 5;//run4 water          
    else if (run>=91400000 && run<=91409999) return 6;//run4 air            
    else if (run>=81510000 && run<=81519999) return 8;//run5 antinu-water
    else if (run>=81600000 && run<=81600159) return 9;//run6b antinu-air
    else if (run>=81600160 && run<=81600219) return 10;//run6c antinu-air - have to split Run 6 due to different flux tunings for the different parts
    else if (run>=81600220 && run<=81600299) return 11;//run6d antinu-air
    else if (run>=81600300 && run<=81600399) return 12;//run6e antinu-air
    else if (run>=81600400 && run<=81609999) return 12;//run6e antinu-air - set this as default, to catch any files that are not currently at TRIUMF
    else return -1;

}


//**************************************************
int anaUtils::GetSandMode(int run){
//**************************************************

  if      (run>=80307000 && run<=80307999) return 0;	// antineutrino flux
  else if (run>=90307000 && run<=90307999) return 1;	// neutrino flux
  else if (run>=92307000 && run<=92307999) return 2;	// neutrino flux, old NEUT

  else return -1;
}

//********************************************************************
void anaUtils::IncrementPOTBySpill(const AnaSpillB& spill, Header& header) {
//********************************************************************

  const AnaBeamB& beam = *spill.Beam;
  const AnaEventInfoB& info = *spill.EventInfo;

  if (beam.POTSincePreviousSavedSpill > 1e+16) {
    std::cout << "WARNING: POT is suspiciously large for run " << info.Run << ", subrun " << info.SubRun << ", event " << info.Event << ": " << beam.POTSincePreviousSavedSpill << ". POT will not be counted for this event." << std::endl;
    return;
  }
  
  if (beam.POTSincePreviousSavedSpill < 0) {
    std::cout << "WARNING: POT is negative for run " << info.Run << ", subrun " << info.SubRun << ", event " << info.Event << ": " << beam.POTSincePreviousSavedSpill << ". POT will not be counted for this event." << std::endl;
    return;
  }

  // For real data
  if (!spill.GetIsMC()) {

    header._POT_NoCut   += beam.POTSincePreviousSavedSpill;
    header._Spill_NoCut += beam.SpillsSincePreviousSavedSpill;

    if (beam.GoodSpill == 0) {
      header._POT_BadBeam   += beam.POTSincePreviousSavedSpill;
      header._Spill_BadBeam += beam.SpillsSincePreviousSavedSpill;
      return;
    }

    if (!spill.DataQuality->GoodDaq) {
      header._POT_BadND280   += beam.POTSincePreviousSavedSpill;
      header._Spill_BadND280 += beam.SpillsSincePreviousSavedSpill;
      return;
    }

    header._POT_GoodBeamGoodND280   += beam.POTSincePreviousSavedSpill;
    header._Spill_GoodBeamGoodND280 += beam.SpillsSincePreviousSavedSpill;
  }
  else{
    // For MC there is no information about magnet current, Spill and DQ are always good
    header._Spill_NoCut             += beam.SpillsSincePreviousSavedSpill;
    header._Spill_GoodBeamGoodND280 += beam.SpillsSincePreviousSavedSpill;

    header._POT_NoCut             += beam.POTSincePreviousSavedSpill;
    header._POT_GoodBeamGoodND280 += beam.POTSincePreviousSavedSpill;
  }

}
