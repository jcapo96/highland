#ifndef BasicUtils_h
#define BasicUtils_h

#include "BasicTypes.hxx"
#include "CoreUtils.hxx"
#include "BaseDataClasses.hxx"
#include "SubDetId.hxx"

/// This namespace contains useful functions for analyses
/// SHOULD CONSIDER TEMPLATING THE METHODS!
namespace anaUtils{

    void ClearArray(AnaTrackB* arr[], int n);
    void ClearArray(AnaParticleB* arr[], int n);
    void ClearArray(AnaTrueParticleB* arr[], int n);
    void ClearArray(AnaSubdet1ParticleB* arr[], int n);
    void ClearArray(AnaSubdet2ParticleB* arr[], int n);

    void ReserveArray(AnaTrackB* arr[], int n);
    void ReserveArray(AnaParticleB* arr[], int n);
    void ReserveArray(AnaTrueParticleB* arr[], int n);
    void ReserveArray(AnaSubdet1ParticleB* arr[], int n);
    void ReserveArray(AnaSubdet2ParticleB* arr[], int n);

    void ReclaimArray(AnaTrackB* arr[], int n, int size);
    void ReclaimArray(AnaParticleB* arr[], int n, int size);
    void ReclaimArray(AnaTrueParticleB* arr[], int n, int size);
    void ReclaimArray(AnaSubdet1ParticleB* arr[], int n, int size);
    void ReclaimArray(AnaSubdet2ParticleB* arr[], int n, int size);

    void CopyArray( AnaTrackB** const     &src,     AnaTrackB**     &dest,  int n);


    void CopyArray(AnaRecObjectC**  tgtArr, AnaParticleB**     srcArr, int nObj);
    void CopyArray(AnaRecObjectC**  tgtArr, AnaTrackB**        srcArr, int nObj);
    void CopyArray(AnaTrueObjectC** tgtArr, AnaTrueParticleB** srcArr, int nObj);


    /// Create varaible sized arrays of pointers
    void CreateArray(AnaTrackB**          &tgtArr,  int nObj);
    void CreateArray(AnaTrueParticleB**      &tgtArr,  int nObj);
    void CreateArray(AnaVertexB**         &tgtArr,  int nObj);
    void CreateArray(AnaTrueVertexB**     &tgtArr,  int nObj);
    void CreateArray(AnaParticleB**       &tgtArr,  int nObj);
    void CreateArray(AnaSubdet1ParticleB**       &tgtArr,  int nObj);
    void CreateArray(AnaSubdet2ParticleB**       &tgtArr,  int nObj);
    void CreateArray(AnaDetCrossingB**    &tgtArr,  int nObj);
    void CreateArray(SubDetId::SubDetEnum*  &tgtArr,  int nObj, SubDetId::SubDetEnum ini=SubDetId::kInvalid);

    void CopyArray(   AnaTrueParticleB** const &src, AnaTrueParticleB** &dest, int n);
    void CopyArray(   AnaParticleB**     const &src, AnaParticleB**     &dest, int n);
    
    void ResizeArray( AnaTrueParticleB**   &tgtArr,    int nObj);
    void ResizeArray( AnaTrackB**       &tgtArr,    int nObj);
}
#endif
