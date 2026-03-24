#ifndef EventDisplayBase_h
#define EventDisplayBase_h

#include <string>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>

// Forward declarations for TEve classes
class TEveManager;
class TEveScene;
class TEveViewer;
class TEveProjectionManager;
class TEveWindowSlot;
class TGLViewer;
class TEveElement;
class TEveElementList;
class TEveLine;
class TEveText;

// Forward declarations for ROOT GUI classes
class TCanvas;

// Forward declarations for OutputManager (will be included in derived classes)
class OutputManager;
class AnaEventB;

// Forward declarations for ruler tool helper classes
class TRulerTool;
class TGLRulerEventHandler;

/// Base class for event display functionality in highland framework
/// Provides common infrastructure for TEve-based visualization with multi-view support
/// and tree management for storing event display data
///
/// Design:
/// - Base class handles: TEve window creation, multi-view layout, tree management
/// - Derived classes provide: experiment-specific variables, geometry, and data filling
///
/// Usage:
/// 1. Derive from this class for experiment-specific implementation
/// 2. Override virtual methods to provide experiment data and geometry
/// 3. Pass instance to DrawingTools via SetEventDisplay()
/// 4. Use DrawingTools::EvtDisplay() to generate visualizations
class EventDisplayBase {
public:
    struct MeasurementAnchor {
        TVector3 position;
        TEveElement* owner;
    };

    struct SavedMeasurement {
        TEveElementList* container = nullptr;
        TEveLine* line = nullptr;
        TEveText* label = nullptr;
        TVector3 start;
        TVector3 end;
        Double_t length = 0.0;
        UInt_t entryId = 0;
    };

    EventDisplayBase();
    virtual ~EventDisplayBase();

    // ========== Tree Management (Analysis-time) ==========

    /// Initialize the EventDisplayData tree with common and experiment-specific variables
    /// This should be called from DefineMicroTrees() in the analysis
    /// @param output OutputManager instance to add tree and variables to
    virtual void InitializeTree(OutputManager& output);

    /// Fill the tree with event data (common + experiment-specific)
    /// This should be called from FillMicroTrees() in the analysis for selected events
    /// @param output OutputManager instance
    /// @param event The event to fill data from
    /// @param box Optional analysis-specific box (can be NULL if not needed)
    virtual void FillTree(OutputManager& output, const AnaEventB& event, void* box = NULL);

    // ========== Visualization (Post-analysis) ==========

    /// Generate event display with 3D and 2D views in tabbed interface
    /// Reads data from EventDisplayData tree and creates TEve visualization
    /// @param filename Path to ROOT file containing EventDisplayData tree
    /// @param run Run number to display
    /// @param subrun Subrun number to display
    /// @param event Event number to display
    /// @param outputFile Optional path to save screenshot (empty = interactive)
    virtual void GenerateDisplay(const std::string& filename, Int_t run, Int_t subrun, Int_t event,
                                 const std::string& outputFile = "");

    /// Generate an event display by zero-based index within the EventDisplayData tree
    /// @param filename Path to ROOT file containing EventDisplayData tree
    /// @param index Zero-based index over stored displayable events (skipping placeholders where ED_run==0)
    /// @param outputFile Optional path to save screenshot (empty = interactive)
    virtual bool GenerateDisplayByIndex(const std::string& filename, Long64_t index,
                                        const std::string& outputFile = "");

    /// List all events available in the EventDisplayData tree
    /// @param filename Path to ROOT file containing EventDisplayData tree
    /// @param category Optional branch/category name to print alongside each entry
    virtual void ListAvailableEvents(const std::string& filename, const std::string& category = "");

    /// Toggle visibility of stored measurements
    void SetMeasurementVisibility(Bool_t visible);
    /// Query visibility of stored measurements
    Bool_t GetMeasurementVisibility() const { return _measurementVisible; }
    /// Access the TEve element list containing saved measurements
    TEveElementList* GetMeasurementList();
    /// Store a measurement and get its length in cm
    Double_t StoreMeasurement(const TVector3& p1, const TVector3& p2);

protected:
    // ========== Pure Virtual Methods (MUST be implemented by derived classes) ==========

    /// Add experiment-specific variables to the EventDisplayData tree
    /// Called by InitializeTree() after adding common variables (run, subrun, event)
    /// @param output OutputManager instance to add variables to
    virtual void AddExperimentVariables(OutputManager& output) = 0;

    /// Fill experiment-specific data into the tree
    /// Called by FillTree() after filling common variables
    /// @param output OutputManager instance
    /// @param event The event to fill data from
    /// @param box Optional analysis-specific box (can be NULL if not needed)
    virtual void FillExperimentData(OutputManager& output, const AnaEventB& event, void* box) = 0;

    /// Draw detector geometry in 3D scene
    /// Called when generating 3D view
    /// @param scene TEve scene to add geometry elements to
    virtual void DrawDetectorGeometry(TEveScene* scene) = 0;

    /// Draw detector geometry for 2D projections
    /// Called when generating RPhi (XY) and RhoZ (XZ) projection views
    /// @param manager Projection manager for the specific view
    /// @param projection_type "RPhi" for XY or "RhoZ" for XZ projection
    virtual void DrawDetectorGeometry2D(TEveProjectionManager* manager, const std::string& projection_type) = 0;

    /// Get color for particle based on PDG code
    /// @param pdg PDG code of the particle
    /// @return ROOT color code (e.g., kRed, kBlue, etc.)
    virtual int GetParticleColor(int pdg) = 0;

    // ========== Virtual Methods (CAN be overridden if needed) ==========

    /// Read event display data from tree for specified event
    /// Default implementation reads common variables (run, subrun, event)
    /// Override to read experiment-specific variables
    /// @param tree EventDisplayData tree
    /// @param run Run number to find
    /// @param subrun Subrun number to find
    /// @param event Event number to find
    /// @return true if event found, false otherwise
    virtual bool ReadEventData(TTree* tree, Int_t run, Int_t subrun, Int_t event);

    /// Draw particle tracks and hits in 3D view
    /// Override to customize 3D visualization
    /// @param scene TEve scene to add elements to
    virtual void DrawParticles3D(TEveScene* scene);

    /// Draw particle tracks and hits in 2D projections
    /// Override to customize 2D visualization
    /// @param manager Projection manager
    /// @param projection_type "RPhi" or "RhoZ"
    virtual void DrawParticles2D(TEveProjectionManager* manager, const std::string& projection_type);

    /// Draw detector geometry on 2D canvas
    /// Override to draw experiment-specific geometry
    /// @param canvas TCanvas to draw on
    /// @param projection_type "XY", "XZ", or "YZ"
    virtual void DrawDetectorCanvas2D(TCanvas* canvas, const std::string& projection_type);

    /// Draw particle tracks and hits on 2D canvas
    /// Override to draw experiment-specific particles
    /// @param canvas TCanvas to draw on
    /// @param projection_type "XY", "XZ", or "YZ"
    virtual void DrawParticlesCanvas2D(TCanvas* canvas, const std::string& projection_type);

    /// Draw analysis-specific content on 2D canvas (e.g., vertices, reconstructed objects)
    /// Override in analysis-specific classes to add custom overlays
    /// @param canvas TCanvas to draw on
    /// @param projection_type "XY", "XZ", or "YZ"
    virtual void DrawAnalysisContentCanvas2D(TCanvas* canvas, const std::string& projection_type);

    /// Hook invoked just before the 3D event scene is cleared.
    /// Derived classes can override to release cached TEve pointers.
    virtual void BeforeSceneCleared();

    /// Remove all persisted ruler measurements
    void ClearAllMeasurements();
    /// Remove a measurement specified by entry id
    void RemoveMeasurementById(UInt_t entryId);
    /// Access saved measurements (for derived UI)
    const std::vector<SavedMeasurement>& GetMeasurements() const { return _measurements; }
    /// Notify derived classes that measurements changed
    virtual void OnMeasurementsChanged();
    /// Notify derived classes that measurement visibility toggled
    virtual void OnMeasurementVisibilityChanged(Bool_t visible);
    /// Temporarily disable measurement callbacks (used during scene resets)
    void SetMeasurementCallbacksSuspended(Bool_t value) { _suspendMeasurementCallbacks = value; }

    // ========== Helper Methods ==========

    /// Initialize TEve manager with multi-view layout
    /// Creates main window with tabs for 3D and 2D views
    void InitializeTEve();

    /// Create 3D viewer tab
    /// @return TEve viewer for 3D view
    TEveViewer* Create3DView();

    /// Create 2D projection tab with RPhi and RhoZ views
    /// @param rphiManager Output: projection manager for RPhi (XY) view
    /// @param rhozManager Output: projection manager for RhoZ (XZ) view
    void Create2DViews(TEveProjectionManager*& rphiManager, TEveProjectionManager*& rhozManager);

    /// Create four-tab layout with separate 3D and 2D projection views
    void CreateFourTabLayout();

    /// Helper to create a single projection view in a tab
    /// @param proj Output: projection manager
    /// @param viewer Output: viewer for this projection
    /// @param scene Output: scene for this projection
    /// @param type Projection type (0=RPhi/XY, 1=RhoZ/XZ or YZ)
    /// @param name Name for the tab
    void CreateProjection(TEveProjectionManager*& proj, TEveViewer*& viewer,
                          TEveScene*& scene, int type, const char* name);

    /// Create 2D canvas views for XY, XZ, YZ projections
    void Create2DCanvases();

    /// Update GUI window titles with current run/subrun/event information
    void UpdateWindowTitles(Int_t run, Int_t subrun, Int_t event);

    /// Draw coordinate axes in 3D scene
    /// @param scene TEve scene to add axes to
    void DrawCoordinateAxes(TEveScene* scene);

    /// Configure 3D camera view
    /// @param viewer TEve 3D viewer
    void Configure3DCamera(TEveViewer* viewer);

    /// Install the interactive ruler tool on the current GL viewer
    void InstallRulerTool(TGLViewer* glViewer);

    /// Clear all measurement anchors (called when a new event is drawn)
    void ResetMeasurementAnchors();

    /// Register a measurement anchor so the ruler can snap to actual objects
    void RegisterMeasurementAnchor(TEveElement* owner, const TVector3& position);
    void RegisterMeasurementAnchor(TEveElement* owner, Double_t x, Double_t y, Double_t z);

    // ========== Data Members ==========

    /// Flag to track if TEve has been initialized
    bool _teveInitialized;

    /// Current TEve 3D scene
    TEveScene* _scene3D;

    /// Current TEve RPhi projection manager
    TEveProjectionManager* _projRPhi;

    /// Current TEve RhoZ projection manager
    TEveProjectionManager* _projRhoZ;

    /// Separate viewers for four-tab layout
    TEveViewer* _viewer3D;
    TEveViewer* _viewerXY;
    TEveViewer* _viewerXZ;
    TEveViewer* _viewerYZ;

    /// Projection managers for XY, XZ, YZ views
    TEveProjectionManager* _projXY;
    TEveProjectionManager* _projXZ;
    TEveProjectionManager* _projYZ;

    /// Scenes for 2D projections
    TEveScene* _sceneXY;
    TEveScene* _sceneXZ;
    TEveScene* _sceneYZ;

    /// TCanvas-based 2D views (stable alternative to TEve projections)
    TCanvas* _canvasXY;
    TCanvas* _canvasXZ;
    TCanvas* _canvasYZ;

    /// Name of the EventDisplayData tree
    std::string _treeName;

    /// Event display class name (for auto-detection)
    std::string _eventDisplayClassName;

    /// Interactive ruler tool for measuring distances in 3D view
    TRulerTool* _rulerTool;

    /// Event handler that routes clicks/keys to the ruler tool
    TGLRulerEventHandler* _rulerEventHandler;

    /// Persisted measurement list and helpers
    TEveElementList* _measurementList = nullptr;
    std::vector<SavedMeasurement> _measurements;
    Bool_t _measurementVisible = kTRUE;
    UInt_t _nextMeasurementId = 1;
    Bool_t _suspendMeasurementCallbacks = kFALSE;
    void EnsureMeasurementList();
    void AttachMeasurementListToScene();
    void NotifyMeasurementsChanged(Bool_t requestRedraw = kTRUE);

    /// Cached measurement anchors for ruler snapping
    std::vector<MeasurementAnchor> _measurementAnchors;

    // Variable indices for EventDisplayData tree
    enum enumEventDisplayBaseVars {
        edClassName,
        edbasemaxvars
    };
};

#endif

