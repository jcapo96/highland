#include "BasicUtils.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>

//********************************************************************
void anaUtils::ReclaimArray(AnaSubdet1ParticleB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL ) continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ReclaimArray(AnaSubdet2ParticleB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL ) continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}


//********************************************************************
void anaUtils::ReclaimArray(AnaParticleB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL)continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ReclaimArray(AnaTrueParticleB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL)continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ReclaimArray(AnaTrackB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL)continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaSubdet1ParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaSubdet1ParticleB();
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaSubdet2ParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaSubdet2ParticleB();
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaParticleB;
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaTrueParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaTrueParticleB();
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaTrackB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaTrackB();
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaSubdet1ParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaSubdet2ParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaTrueParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaTrackB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}
//********************************************************************
void anaUtils::CopyArray(AnaTrackB** const &src, AnaTrackB** &dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = src[i];
    }
    return;
}

//********************************************************************
void anaUtils::CopyArray(AnaTrueParticleB** const &src, AnaTrueParticleB** &dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = src[i];
    }
    return;
}

//********************************************************************
void anaUtils::CopyArray(AnaParticleB** const &src, AnaParticleB** &dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = src[i];
    }
    return;
}

//********************************************************************
void anaUtils::CopyArray(AnaRecObjectC** tgtArr, AnaParticleB** srcArr, int nObj){
//********************************************************************

    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = srcArr[i];
    }
}

//********************************************************************
void anaUtils::CopyArray(AnaRecObjectC** tgtArr, AnaTrackB** srcArr, int nObj){
//********************************************************************

    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = srcArr[i];
    }
}

//********************************************************************
void anaUtils::CopyArray(AnaTrueObjectC** tgtArr, AnaTrueParticleB** srcArr, int nObj){
//********************************************************************

    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = srcArr[i];
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaTrackB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaTrackB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaVertexB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaVertexB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaTrueVertexB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaTrueVertexB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaParticleB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaParticleB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaSubdet1ParticleB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaSubdet1ParticleB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaSubdet2ParticleB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaSubdet2ParticleB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaDetCrossingB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaDetCrossingB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(SubDetId::SubDetEnum* &tgtArr, int nObj, SubDetId::SubDetEnum ini){
//********************************************************************

    tgtArr = new SubDetId::SubDetEnum[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = ini;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaTrueParticleB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaTrueParticleB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::ResizeArray(AnaTrueParticleB** &tgtArr, int nObj){
//********************************************************************

  tgtArr = (AnaTrueParticleB**) realloc (tgtArr, nObj*sizeof(AnaTrueParticleB*));
}


//********************************************************************
void anaUtils::ResizeArray(AnaTrackB** &tgtArr, int nObj){
//********************************************************************

  tgtArr = (AnaTrackB**) realloc (tgtArr, nObj*sizeof(AnaTrackB*));
}
