#ifndef ProductionUtils_h
#define ProductionUtils_h

#include "VersionManager.hxx"
          
  /// Enum for the different productions suported
namespace ProdId{
  enum ProdEnum{
    unknown=UNKNOWN_PRODUCTION,
    MCC5,
    MCC6,
    MCC7,
    PDSPProd2
  };
}

/// This namespace contains useful functions for analyses. 
/// This is in addition to those defined in psycheUtils
namespace anaUtils{

    /// Get Production Id from nd280AnalysisTools
    ProdId_h GetProductionIdFromoaAnalysisReader();

    /// Add the standard production versions
    void AddStandardProductions();
}
#endif


