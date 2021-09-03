#ifndef baseSelection_h
#define baseSelection_h

#include "SelectionBase.hxx"
#include "Parameters.hxx"

/// This file defines steps that are common to all analyses.
/// Currently the only cut is one on event quality.


class baseSelection: public SelectionBase{
    public:
        baseSelection();
        virtual ~baseSelection(){}

        //---- These are mandatory functions
        void DefineSteps();
//        ToyBoxB* MakeToyBox(){return new ToyBoxTracker();}
        EventBoxB* MakeEventBox(){return new EventBoxB();}
//        AnaEventSummaryB* MakeEventSummary(){return new AnaEventSummaryB();}
        bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);

};


/// A cut on event quality. Requires good beam and detector data quality flags.
class EventQualityCut: public StepBase {
    public:
        using StepBase::Apply;

        EventQualityCut(){
            enableDQCut = (bool) ND::params().GetParameterI("psycheSelections.EnableDataQualityCut");
            enableBeamQualityCut = (bool) ND::params().GetParameterI("psycheSelections.EnableBeamQualityCut");
        }

        bool enableDQCut;
        bool enableBeamQualityCut;

        /// Apply the event quality cut. See EventQualityCut class documentation
        /// for details.
        bool Apply(AnaEventC& event, ToyBoxB& box) const;
        StepBase* MakeClone(){return new EventQualityCut();}
};

#endif
