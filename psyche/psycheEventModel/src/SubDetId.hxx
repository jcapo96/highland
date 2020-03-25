#ifndef SubDetId_hxx_seen
#define SubDetId_hxx_seen

#include "BasicTypes.hxx"

class SubDetId {

    private:
        /// Bit mask array to select different subdetectors and systems
        const static unsigned long DetMask[28];

        /// Method to make detector bit masks
        static unsigned long MakeMask(int msb, int lsb) {return (((1<<(((msb)-(lsb))+1))-1)<<(lsb));}

        /// Method to count the number of set bits in a given bit field
        static int NumberOfSetBits(unsigned long v);


    public:

        SubDetId(){};
        ~SubDetId(){};

        /// Enumeration of all detector systems and subdetectors (example of one subdetector with 2 modules and another one with 3
        enum SubDetEnum {
            kSubdet1_1 = 0,
            kSubdet1_2,
            kSubdet2_1,
            kSubdet2_2,
            kSubdet2_3,
            kInvalidSubdetector,
            kSubdet1,
            kSubdet2,
            kInvalid
        };

        /// Method to set a certain subdetector or subdetector system to used used
        static void SetDetectorUsed(unsigned long &BitField, SubDetId::SubDetEnum det);

        /// Method to see if a certain subdetector or subdetector system is used
        static bool GetDetectorUsed(unsigned long BitField, SubDetId::SubDetEnum det);

        static bool GetDetectorArrayUsed(unsigned long BitField, SubDetId::SubDetEnum dets[], int nDet);

        static bool GetDetectorUsed(unsigned long BitField, SubDetId_h det);
        static bool GetDetectorArrayUsed(unsigned long BitField, SubDetId_h dets[], int nDet);


        /// Returns the Subdet2 number (Subdet1_2 = 1, Subdet2_2 = 2 etc.) of the track
        /// Does not check whether track passed through more than one Subdet2 module, just returns the first Subdet2 it finds (checking Subdet2_2, Subdet2_3 then Subdet2_1)
        static int GetSubdet2(unsigned long BitField);

        /// Method to set the detector system bits using the individual subdetector bits.
        /// e.g. If SetDetectorUsed(BitField, SubDetId::kTPC1) had been called, this function
        /// would call SetDetectorUsed(BitField, SubDetId::kTPC)
        static void SetDetectorSystemFields(unsigned long &BitField);

        /// Method to count the number of Segments a given track has in a detector system
        /// The DetMask bitfield masks are used to select the specified detector system
        static int GetNSegmentsInDet(unsigned long BitFIeld, SubDetId::SubDetEnum det);

        /// Check whether a track only uses a specified subdetector or detector system
        static bool TrackUsesOnlyDet(unsigned long BitFIeld, SubDetId::SubDetEnum det);

        /// Check if a detector enumeration refers to a TPC or not
        static bool IsSubdet1Detector(SubDetId::SubDetEnum det);

        /// Check if a detector enumeration refers to a FGD or not
        static bool IsSubdet2Detector(SubDetId::SubDetEnum det);
        /// Get the single subdetector that this track is from
        static SubDetId::SubDetEnum GetSubdetectorEnum(unsigned long BitField);

};
#endif


