#ifndef VERSIONING_HXX_SEEN
#define VERSIONING_HXX_SEEN
//#include "AnalysisTools/libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"

/// This file contains pre-processor directives related to the
/// oaAnalysis file format that was used to compile nd280AnalysisTools.
/// Depending on the file format, different bits of highland need to
/// be conditionally compiled.
///
/// nd280AnalysisTools advertises a few low-level directives. This file
/// defines higher-level directives (related to specific features that
/// are enabled/disabled in each version), to make using the directives
/// easier and more transparent.
///
/// If you are editing this file, you should make sure that all definitions
/// check for the existence of ANATOOLS_FILE_VERSION as the first check.
/// This ensures that backwards-compatibility is kept for people with older
/// versions of nd280AnalysisTools.
///
/// The file version information was only introduced in nd280AnalysisTools
/// v1r1. For backwards-compatibility we assume that anyone using an older
/// version of nd280AnalysisTools is using a Production 5 file. We also mock
/// up some of the definitions we rely on.

#if !defined ANATOOLS_FILE_VERSION
/// This is just a mockup so that things compile. The real BEFORE_ANATOOLS_FILE
/// is found in libReadoaAnalysisProjectHeaders.h in nd280AnalysisTools. 
/// Having this mockup defined as 1 means that old versions of nd280AnalysisTools
/// are assumed to be processing Production 5 files.
#define BEFORE_ANATOOLS_FILE(v,r,p) 1

/// This is just a mockup so that things compile. The real ANATOOLS_FILE_PROD5
/// is found in libReadoaAnalysisProjectHeaders.h in nd280AnalysisTools.
/// Having this mockup defined as 1 means that old versions of nd280AnalysisTools
/// are assumed to be processing Production 5 files.
#define ANATOOLS_FILE_PROD5 1
#endif

/// For production 6 pre-production files, we need to disable all corrections.
#define VERSION_DISABLE_CORRECTIONS VERSION_PROD6PRE

/// nd280 version for production 5E is v10r11p17 to v10r11p18
#define VERSION_PROD5E (!defined ANATOOLS_FILE_VERSION || ANATOOLS_FILE_PROD5 || (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(10,11,17) && BEFORE_ANATOOLS_FILE(10,11,18)))

/// nd280 version for production 5F goes from v10r11p19 to v10r11p23
#define VERSION_PROD5F (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(10,11,19) && BEFORE_ANATOOLS_FILE(10,11,27))

/// nd280 version for production 5G is v10r11p27
#define VERSION_PROD5G (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(10,11,27) && BEFORE_ANATOOLS_FILE(10,11,29))

/// nd280 version for pre production 6 goes from v11r17 to v11r27
#define VERSION_PROD6PRE (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,17,0) && BEFORE_ANATOOLS_FILE(11,29,0))

/// nd280 version for Production 6A is v11r29
#define VERSION_PROD6A (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,29,0) && BEFORE_ANATOOLS_FILE(11,31,0))

/// nd280 version for Production 6B is v11r31
#define VERSION_PROD6B (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,31,0) && BEFORE_ANATOOLS_FILE(11,33,0))

/// For production 6 pre-production or production 6A files, we need to disable all production dependent corrections
#define VERSION_DISABLE_PROD_CORRECTIONS (VERSION_PROD6PRE || VERSION_PROD6A)

/// The B-field distortion refit info changed interface between P5 and P6.
/// In P5F, we had Charge, Momentum, Position and Direction saved
#define VERSION_HAS_BFIELD_REFIT_FULL VERSION_PROD5F

/// In P6, we have only the B-field refit Momentum (and it changed name).
#define VERSION_HAS_BFIELD_REFIT_BASIC (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,17,0))

/// Whether there is any B-field information. Although the momentum name is
/// different in P5/P6 oaAnalysis files, we kept the same name in highland.
/// Note that there is a gap between P5 and P6 pre-production files where
/// no B-field info was saved...
#define VERSION_HAS_BFIELD_REFIT (VERSION_HAS_BFIELD_REFIT_FULL || VERSION_HAS_BFIELD_REFIT_BASIC )

/// TPC E-field refit information was added in v11r17.
#define VERSION_HAS_EFIELD_REFIT (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,17,0))

/// P5 files didn't store the ECal LLR (PID) variables directly, and they had
/// to be calculated using a tool in nd280AnalysisTools. In P6, they are
/// stored directly in the oaAnalysis file.
#define VERSION_HAS_ECAL_LLR (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,0,0))

/// P5 files had CT5POT (and needed a correction to use CT4POT for some beam runs). P6 files
/// have an OfficalPOT variable.
#define VERSION_HAS_OFFICIAL_POT (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 files have refits of each global track for electron/muon/proton hypotheses
/// in both the forwards and backwards directions.
#define VERSION_HAS_REVERSED_REFITS (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 files have momentum from range estiamtates for a global track, assuming electron/muon/proton hypotheses
/// in both the forwards and backwards directions.
#define VERSION_HAS_PRANGE_ESTIMATES (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 files have node times updated with t_vs_dist fit results
/// independently for each sub-detector
#define VERSION_HAS_TIME_FITS (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 files have have both vertex (start or entrance) and end (stop or exit) activities
/// calculated for FGD tracks
#define VERSION_HAS_FGD_VERTEX_AND_END_ACTIVITY (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,17,0))

/// P6 files use the same method for main track fitting and alternates fitting
#define VERSION_HAS_EQUIVALENT_MAIN_AND_ALT_FITS (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(11,17,0))

/// For preproduction 6 the number of nodes for TPC objects was 2
#define VERSION_HAS_TWO_TPC_NODES VERSION_PROD6PRE

/// For production 6 P0D objects got hits/nodes averaged time stamps 
#define VERSION_HAS_P0D_AVERAGED_TIME (VERSION_PROD6A || VERSION_PROD6B) 

/// For production 6 FGDTimeBins objects have hit information
#define VERSION_HAS_FGDTIMEBIN_ALL_HITS (VERSION_PROD6A || VERSION_PROD6B) 

/// Production 7 development version
#define VERSION_PROD7_DEVEL (defined ANATOOLS_FILE_VERSION && !BEFORE_ANATOOLS_FILE(12,0,0))


#endif
