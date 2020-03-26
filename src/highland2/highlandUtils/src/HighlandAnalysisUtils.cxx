#include "HighlandAnalysisUtils.hxx"
#include <TMath.h>
#include <math.h>

//*********************************************************
AnaVertexB* anaUtils::GetGlobalVertex(const AnaEventB& event, AnaTrackB** Tracks, int nTracks){
//*********************************************************
  AnaVertexB* vertex = NULL;
  
  // Loop over vertices available in the event
  for (int i = 0; i < event.nVertices; i++){
    
    if (!event.Vertices[i]) continue;
    
    int nMatch_tmp  = 0;
    int nTracks_tmp = 0;
    bool tracks_loop = false;
    
    // Loop through vertex constituents
    for (int j = 0; j < event.Vertices[i]->nParticles; j++){
      
      if (!event.Vertices[i]->Particles[j]) continue;
      
      // Loop through input tracks
      for (int k = 0; k < nTracks; k++){
        if (!Tracks[k]) continue;
        
        // Count valid tracks
        if (!tracks_loop) nTracks_tmp++; 
        
        // Check whether have the same objects, check UniqueID just in case some copying took place 
        // (probably redundunt)
        if (event.Vertices[i]->Particles[j]->UniqueID == Tracks[k]->UniqueID)
          nMatch_tmp++;
      }
      
      tracks_loop = true; 
    }
    
    // Check whether all valid tracks provided belong to the vertex
    if (nTracks_tmp == nMatch_tmp){ 
      vertex = event.Vertices[i];
      break;
    }
  }
  
  return vertex;

}


//*********************************************************
std::vector<AnaTrueParticle*> anaUtils::GetTrueDaughters(std::vector<AnaTrueParticleB*>& trueParticles, const AnaTrueParticle* truePart, bool recursive){
//*********************************************************

  std::vector<AnaTrueParticle*> daughters;

  // Daughters of the input particle
  std::vector<Int_t> daughters_ind = truePart->Daughters;

  //  std::cout << " find daughters: " << truePart << " " << truePart->ID << std::endl; 
  
  // loop over all true particles
  for (std::vector<AnaTrueParticleB*>::iterator it1=trueParticles.begin();it1!=trueParticles.end();it1++){      
    AnaTrueParticle* truePart0 = static_cast<AnaTrueParticle*>(*it1);    

    bool found= false;
    // loop over all daughters of the input particle
    for (std::vector<Int_t>::iterator it2=daughters_ind.begin();it2!=daughters_ind.end();it2++){      
      if (truePart0->ID == *it2){
        //        std::cout << " - " << daughters.size() << " :" <<  truePart0->ID << std::endl;
        // Add the particle pointer to the vector of daughters
        daughters.push_back(truePart0);
        found = true;
      }
    }

    // If te current true particle is a dougher of the input particle add also its daughters
    if (found && recursive){
      for (std::vector<Int_t>::iterator it3=truePart0->Daughters.begin();it3!=truePart0->Daughters.end();it3++){      
        daughters_ind.push_back(*it3);
        //        std::cout << "   - " << *it3 << std::endl;
      }
    }
  }
  
  return daughters;
}
