#include "SubDetId.hxx"
#include <iostream>
#include <bitset>

const unsigned long SubDetId::DetMask[] = {
    1<<SubDetId::kSubdet1_1,
    1<<SubDetId::kSubdet1_2,
    1<<SubDetId::kSubdet2_1,
    1<<SubDetId::kSubdet2_2,
    1<<SubDetId::kSubdet2_3,
    1<<SubDetId::kInvalidSubdetector,
    1<<SubDetId::kSubdet2     | SubDetId::MakeMask(SubDetId::kSubdet2_3,       SubDetId::kSubdet2_1),
    1<<SubDetId::kSubdet1     | SubDetId::MakeMask(SubDetId::kSubdet1_2,       SubDetId::kSubdet1_1),
    1<<SubDetId::kInvalid
};

void SubDetId::SetDetectorUsed(unsigned long &BitField, SubDetId::SubDetEnum det){
    BitField = BitField | 1<<det;
}

bool SubDetId::GetDetectorUsed(unsigned long BitField, SubDetId::SubDetEnum det){
    return BitField & DetMask[det];
}

bool SubDetId::GetDetectorUsed(unsigned long BitField, SubDetId_h det){
    return BitField & DetMask[det];
}


bool SubDetId::GetDetectorArrayUsed(unsigned long BitField, SubDetId::SubDetEnum dets[], int nDet){

    bool passed = 1;

    for(int i = 0; i < nDet; ++i){
        if(!(BitField & DetMask[dets[i]])){
            passed = 0;
        }
    }
    return passed;
}

bool SubDetId::GetDetectorArrayUsed(unsigned long BitField, SubDetId_h dets[], int nDet){

    bool passed = 1;

    for(int i = 0; i < nDet; ++i){
        if(!(BitField & DetMask[dets[i]])){
            passed = 0;
        }
    }
    return passed;
}


int SubDetId::GetSubdet2(unsigned long BitField){
    if      (BitField & DetMask[SubDetId::kSubdet2_2]) return 2;
    else if (BitField & DetMask[SubDetId::kSubdet2_3]) return 3;
    else if (BitField & DetMask[SubDetId::kSubdet2_1]) return 1;
    else return -1;
}

void SubDetId::SetDetectorSystemFields(unsigned long &BitField){
    for(int i = SubDetId::kSubdet1_1; i < SubDetId::kInvalidSubdetector; ++i){ //loop through sub-detectors list
        if (GetDetectorUsed(BitField, static_cast<SubDetId::SubDetEnum>(i))){
            // Subdet1
            if (i == SubDetId::kSubdet1_1 || i == SubDetId::kSubdet1_2)
              SetDetectorUsed(BitField, SubDetId::kSubdet1);
            // Subdet2
            else if (i == SubDetId::kSubdet2_1 || i == SubDetId::kSubdet2_2 || i == SubDetId::kSubdet2_3)
              SetDetectorUsed(BitField, SubDetId::kSubdet2);
        }
    }
}
int SubDetId::NumberOfSetBits(unsigned long v){
    int c; // c accumulates the total bits set in v
    for (c = 0; v; c++){
        v &= v - 1; // clear the least significant bit set
    }
    return c;
}

int SubDetId::GetNSegmentsInDet(unsigned long BitField, SubDetId::SubDetEnum det){
    return NumberOfSetBits(BitField & DetMask[det]);
}

bool SubDetId::TrackUsesOnlyDet(unsigned long BitField, SubDetId::SubDetEnum det){
    BitField = BitField & MakeMask(SubDetId::kSubdet2_3, SubDetId::kSubdet1_1);
    return !(BitField & ~DetMask[det]);
}

bool SubDetId::IsSubdet2Detector(SubDetId::SubDetEnum det){
    return (det <= SubDetId::kSubdet2_3 && det >= SubDetId::kSubdet2_1);
}

bool SubDetId::IsSubdet1Detector(SubDetId::SubDetEnum det){
    return (det <= SubDetId::kSubdet1_2 && det >= SubDetId::kSubdet1_1);
}
SubDetId::SubDetEnum SubDetId::GetSubdetectorEnum(unsigned long BitField){
 
    BitField = BitField & MakeMask(SubDetId::kSubdet2_3, SubDetId::kSubdet1_1);
    int nBits = NumberOfSetBits(BitField);
    if(nBits!=1){
        std::cout << " Error: Track contains multiple subdetectors, cannot find a single subdetector enum to return." << std::endl;
        return SubDetId::kInvalid;
    }
    BitField = BitField | (BitField-1);
    nBits = NumberOfSetBits(BitField);
    return static_cast<SubDetId::SubDetEnum>(nBits-1);
}

