#include "SampleId.hxx"


//*********************************************************
std::string SampleId::ConvertSample(SampleEnum sample){
//*********************************************************

    std::string ssample="";

    
    if      (sample == kUnassigned) ssample          = "Unassigned";
    else if (sample == kDUNE_NuMuCC) ssample         = "10 kt numuCC inclusive";
    else if (sample == kProtoDUNE_SP_Elec) ssample   = "ProtoDUNE SP single electrons";
    else if (sample == kProto35t_Muon) ssample       = "35t prototype single muons";

    else ssample      = "Unassigned";

    return ssample;
}


//*********************************************************
std::string SampleId::ConvertSampleToSelection(SampleEnum sample){
//*********************************************************

    //These should be the same as what is used to initialize selection names in 
    //AnalysisManager. Some of them are made up right now. -ACK

    std::string ssample="";

    if      (sample == kUnassigned) ssample          = "Unassigned";
    else if (sample == kDUNE_NuMuCC) ssample         = "duneExample";
    else if (sample == kProtoDUNE_SP_Elec) ssample   = "protoDuneExample";
    else if (sample == kProto35t_Muon) ssample       = "protoDUneExample";

    else ssample      = "Unassigned";

    return ssample;
}
