#ifndef EventDisplayBase_h
#define EventDisplayBase_h

#include <string>
#include <TTree.h>
#include <TFile.h>

// Forward declarations for TEve classes
class TEveManager;
class TEveScene;
class TEveViewer;
class TEveProjectionManager;
class TEveWindowSlot;

// Forward declarations for OutputManager (will be included in derived classes)
class OutputManager;
class AnaEventB;

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

    /// List all events available in the EventDisplayData tree
    /// @param filename Path to ROOT file containing EventDisplayData tree
    virtual void ListAvailableEvents(const std::string& filename);

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

    /// Draw coordinate axes in 3D scene
    /// @param scene TEve scene to add axes to
    void DrawCoordinateAxes(TEveScene* scene);

    /// Configure 3D camera view
    /// @param viewer TEve 3D viewer
    void Configure3DCamera(TEveViewer* viewer);

    // ========== Data Members ==========

    /// Flag to track if TEve has been initialized
    bool _teveInitialized;

    /// Current TEve 3D scene
    TEveScene* _scene3D;

    /// Current TEve RPhi projection manager
    TEveProjectionManager* _projRPhi;

    /// Current TEve RhoZ projection manager
    TEveProjectionManager* _projRhoZ;

    /// Name of the EventDisplayData tree
    std::string _treeName;

    /// Event display class name (for auto-detection)
    std::string _eventDisplayClassName;

    // Variable indices for EventDisplayData tree
    enum enumEventDisplayBaseVars {
        edClassName,
        edbasemaxvars
    };
};

#endif

