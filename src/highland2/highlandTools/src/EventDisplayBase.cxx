#include "EventDisplayBase.hxx"
#include "OutputManager.hxx"
#include <iostream>
#include <iomanip>
#include <TEveManager.h>
#include <TEveLine.h>
#include <TEvePointSet.h>
#include <TEveViewer.h>
#include <TEveScene.h>
#include <TEveWindow.h>
#include <TGeoManager.h>
#include <TEveGeoShape.h>
#include <TEveStraightLineSet.h>
#include <TEveProjectionManager.h>
#include <TEveProjectionAxes.h>
#include <TGLViewer.h>

//********************************************************************
EventDisplayBase::EventDisplayBase() {
//********************************************************************
    _teveInitialized = false;
    _scene3D = NULL;
    _projRPhi = NULL;
    _projRhoZ = NULL;
    _treeName = "EventDisplayData";
}

//********************************************************************
EventDisplayBase::~EventDisplayBase() {
//********************************************************************
    // TEve manager owns the scenes and viewers, so we don't delete them
}

//********************************************************************
void EventDisplayBase::InitializeTree(OutputManager& output) {
//********************************************************************
    std::cout << "EventDisplayBase::InitializeTree() - Creating EventDisplayData tree" << std::endl;

    // Use a regular tree index (not a special tree) to avoid issues with OutputManager internal vectors
    // We use index 45 which is well above NMAXSPECIALTREES (20) but comfortably within NMAXTREES (50)
    // This leaves room for configuration trees which start at index 20
    const Int_t eventDisplayTreeIndex = 45;

    // Create the EventDisplayData tree
    output.AddTreeWithName(eventDisplayTreeIndex, _treeName.c_str());

    // Add event display class name variable (for auto-detection)
    output.AddVar(eventDisplayTreeIndex, edClassName, "ED_ClassName", "C", "Event display class name");

    // Call derived class to add experiment-specific variables
    AddExperimentVariables(output);

    std::cout << "EventDisplayBase::InitializeTree() - Tree initialized successfully" << std::endl;
}

//********************************************************************
void EventDisplayBase::FillTree(OutputManager& output, const AnaEventB& event, void* box) {
//********************************************************************
    // Use the same tree index as in InitializeTree
    const Int_t eventDisplayTreeIndex = 45;

    // Save current tree
    Int_t previousTree = output.GetCurrentTree();

    // Set EventDisplayData tree as current
    output.SetCurrentTree(eventDisplayTreeIndex);

    // Initialize tree (resets all variables for this entry)
    output.InitializeTree(eventDisplayTreeIndex);

    // Fill common variables
    // Event display class name (for auto-detection)
    output.FillVar(edClassName, _eventDisplayClassName.c_str());

    // Run, subrun, event are filled by derived class (they know the event structure)

    // Call derived class to fill experiment-specific data
    FillExperimentData(output, event, box);

    // Actually write the entry to the EventDisplayData tree
    output.FillTree(eventDisplayTreeIndex);

    // Restore previous tree
    output.SetCurrentTree(previousTree);
}

//********************************************************************
void EventDisplayBase::GenerateDisplay(const std::string& filename, Int_t run, Int_t subrun, Int_t event,
                                       const std::string& outputFile) {
//********************************************************************
    std::cout << "\n=== Event Display Generation ===" << std::endl;
    std::cout << "Event: Run " << run << ", Subrun " << subrun << ", Event " << event << std::endl;

    if (!outputFile.empty()) {
        std::cout << "Output file: " << outputFile << std::endl;
    }

    // Open the file and get the tree
    TFile* file = TFile::Open(filename.c_str());
    if (!file || file->IsZombie()) {
        std::cout << "ERROR: Could not open file: " << filename << std::endl;
        return;
    }

    TTree* tree = (TTree*)file->Get(_treeName.c_str());
    if (!tree) {
        std::cout << "ERROR: No '" << _treeName << "' tree found in the file!" << std::endl;
        std::cout << "Make sure the analysis was run with event display data saving enabled." << std::endl;
        file->Close();
        return;
    }

    // Read event data from tree
    if (!ReadEventData(tree, run, subrun, event)) {
        std::cout << "ERROR: Could not find or read event data!" << std::endl;
        file->Close();
        return;
    }

    // Initialize TEve with multi-view layout
    InitializeTEve();

    // Create/get 3D scene and clear previous content
    if (!gEve->GetEventScene()) {
        _scene3D = gEve->SpawnNewScene("Event Scene", "Scene holding event data");
    } else {
        _scene3D = gEve->GetEventScene();
        _scene3D->DestroyElements(); // Clear previous event
    }

    // Draw coordinate axes in 3D view
    DrawCoordinateAxes(_scene3D);

    // Draw detector geometry (experiment-specific)
    DrawDetectorGeometry(_scene3D);

    // Draw particles and hits in 3D
    DrawParticles3D(_scene3D);

    // NOTE: 2D projection views disabled for clean single 3D viewport
    // Uncomment below to enable 2D tabs (RPhi, RhoZ projections)
    /*
    // Create 2D projection views
    Create2DViews(_projRPhi, _projRhoZ);

    // Import 3D elements into 2D projections (automatic projection of 3D scene)
    if (_projRPhi && _scene3D) {
        _projRPhi->ImportElements(_scene3D);
    }
    if (_projRhoZ && _scene3D) {
        _projRhoZ->ImportElements(_scene3D);
    }

    // Draw detector geometry in 2D projections (if needed - optional)
    if (_projRPhi) DrawDetectorGeometry2D(_projRPhi, "RPhi");
    if (_projRhoZ) DrawDetectorGeometry2D(_projRhoZ, "RhoZ");

    // Draw analysis-specific particles in 2D projections (if needed - optional)
    if (_projRPhi) DrawParticles2D(_projRPhi, "RPhi");
    if (_projRhoZ) DrawParticles2D(_projRhoZ, "RhoZ");
    */

    // Set scene name
    std::string sceneTitle = "Event Display: Run " + std::to_string(run) +
                            ", Subrun " + std::to_string(subrun) +
                            ", Event " + std::to_string(event);
    _scene3D->SetElementName(sceneTitle.c_str());

    // Redraw everything
    gEve->Redraw3D(kTRUE);

    // Configure 3D camera
    TEveViewer* viewer3D = gEve->GetDefaultViewer();
    if (viewer3D) {
        Configure3DCamera(viewer3D);
    }

    std::cout << "\n✓ Event Display READY!" << std::endl;
    std::cout << "  Event: " << sceneTitle << std::endl;
    std::cout << "\n=== INTERACTION ===" << std::endl;
    std::cout << "  - Scroll to zoom, drag to rotate/pan" << std::endl;
    std::cout << "  - Shift+Click: Select objects for info" << std::endl;

    if (!outputFile.empty()) {
        gEve->GetDefaultGLViewer()->SavePicture(outputFile.c_str());
        std::cout << "\n✓ Saved to: " << outputFile << std::endl;
    } else {
        std::cout << "\n✓ Interactive 3D display ready!" << std::endl;
    }

    file->Close();
}

//********************************************************************
void EventDisplayBase::ListAvailableEvents(const std::string& filename) {
//********************************************************************
    std::cout << "\n=== Available Event Displays ===" << std::endl;

    // Open the file
    TFile* file = TFile::Open(filename.c_str());
    if (!file || file->IsZombie()) {
        std::cout << "ERROR: Could not open file: " << filename << std::endl;
        return;
    }

    TTree* tree = (TTree*)file->Get(_treeName.c_str());
    if (!tree) {
        std::cout << "ERROR: No '" << _treeName << "' tree found in the file!" << std::endl;
        std::cout << "Make sure the analysis was run with event display data saving enabled." << std::endl;
        file->Close();
        return;
    }

    // Check if event display branches exist
    if (!tree->GetBranch("ED_run")) {
        std::cout << "ERROR: EventDisplayData branches not found in tree." << std::endl;
        std::cout << "Make sure the analysis was run with event display data saving enabled." << std::endl;
        file->Close();
        return;
    }

    // Set up to read the data
    Int_t ed_run, ed_subrun, ed_event;
    tree->SetBranchAddress("ED_run", &ed_run);
    tree->SetBranchAddress("ED_subrun", &ed_subrun);
    tree->SetBranchAddress("ED_event", &ed_event);

    // Print header
    std::cout << "\nStored events with display data:\n" << std::endl;
    std::cout << "Run      Subrun   Event" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    // Loop through entries
    Long64_t nEntries = tree->GetEntries();
    int displayCount = 0;

    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);

        // Only show entries that have event display data (where ED_run != 0)
        if (ed_run > 0) {
            std::cout << std::setw(8) << ed_run << " "
                     << std::setw(8) << ed_subrun << " "
                     << std::setw(8) << ed_event << std::endl;
            displayCount++;
        }
    }

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Total: " << displayCount << " event(s) with display data" << std::endl;
    std::cout << "\nUsage: draw.EvtDisplay(run, subrun, event)" << std::endl;
    std::cout << std::endl;

    file->Close();
}

//********************************************************************
bool EventDisplayBase::ReadEventData(TTree* tree, Int_t run, Int_t subrun, Int_t event) {
//********************************************************************
    // Default implementation reads common variables
    // Derived classes should override this to read experiment-specific data

    Int_t ed_run, ed_subrun, ed_event;
    tree->SetBranchAddress("ED_run", &ed_run);
    tree->SetBranchAddress("ED_subrun", &ed_subrun);
    tree->SetBranchAddress("ED_event", &ed_event);

    // Find the event
    Long64_t nEntries = tree->GetEntries();
    bool found = false;

    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);

        if (ed_run == run && ed_subrun == subrun && ed_event == event) {
            found = true;
            std::cout << "Found event data at entry " << i << std::endl;
            break;
        }
    }

    return found;
}

//********************************************************************
void EventDisplayBase::DrawParticles3D(TEveScene* scene) {
//********************************************************************
    // Default implementation - derived classes should override
    // to draw experiment-specific particles and hits
    std::cout << "EventDisplayBase::DrawParticles3D() - No implementation (override in derived class)" << std::endl;
}

//********************************************************************
void EventDisplayBase::DrawParticles2D(TEveProjectionManager* manager, const std::string& projection_type) {
//********************************************************************
    // Default implementation - derived classes should override
    // to draw experiment-specific particles and hits in 2D projections
    (void)manager;
    (void)projection_type;
    std::cout << "EventDisplayBase::DrawParticles2D() - No implementation (override in derived class)" << std::endl;
}

//********************************************************************
void EventDisplayBase::InitializeTEve() {
//********************************************************************
    if (!gEve) {
        TEveManager::Create(kTRUE); // Create with full browser for native zoom
        std::cout << "\n✓ TEve window opened!" << std::endl;
        std::cout << "NOTE: Multi-view display with tabs for 3D and 2D projections" << std::endl;
        _teveInitialized = true;
    } else {
        std::cout << "\n✓ Updating TEve display..." << std::endl;
    }
}

//********************************************************************
TEveViewer* EventDisplayBase::Create3DView() {
//********************************************************************
    // Get or create the default 3D viewer
    TEveViewer* viewer = gEve->GetDefaultViewer();

    if (!viewer) {
        // Create new viewer if needed
        viewer = gEve->SpawnNewViewer("3D View");
        viewer->AddScene(gEve->GetEventScene());
        viewer->AddScene(gEve->GetGlobalScene());
    }

    return viewer;
}

//********************************************************************
void EventDisplayBase::Create2DViews(TEveProjectionManager*& rphiManager, TEveProjectionManager*& rhozManager) {
//********************************************************************
    // Create projection managers if they don't exist

    // RPhi (XY) projection
    if (!_projRPhi) {
        _projRPhi = new TEveProjectionManager(TEveProjection::kPT_RPhi);
        gEve->AddToListTree(_projRPhi, kFALSE);

        TEveViewer* viewerRPhi = gEve->SpawnNewViewer("RPhi (XY) View");
        TEveScene* sceneRPhi = gEve->SpawnNewScene("RPhi Scene");
        viewerRPhi->AddScene(sceneRPhi);
        _projRPhi->SetProjection(TEveProjection::kPT_RPhi);

        TEveProjectionAxes* axesRPhi = new TEveProjectionAxes(_projRPhi);
        sceneRPhi->AddElement(axesRPhi);
    }

    // RhoZ (XZ) projection
    if (!_projRhoZ) {
        _projRhoZ = new TEveProjectionManager(TEveProjection::kPT_RhoZ);
        gEve->AddToListTree(_projRhoZ, kFALSE);

        TEveViewer* viewerRhoZ = gEve->SpawnNewViewer("RhoZ (XZ) View");
        TEveScene* sceneRhoZ = gEve->SpawnNewScene("RhoZ Scene");
        viewerRhoZ->AddScene(sceneRhoZ);
        _projRhoZ->SetProjection(TEveProjection::kPT_RhoZ);

        TEveProjectionAxes* axesRhoZ = new TEveProjectionAxes(_projRhoZ);
        sceneRhoZ->AddElement(axesRhoZ);
    }

    rphiManager = _projRPhi;
    rhozManager = _projRhoZ;
}

//********************************************************************
void EventDisplayBase::DrawCoordinateAxes(TEveScene* scene) {
//********************************************************************
    // Add coordinate axes with labels
    TEveStraightLineSet* xAxis = new TEveStraightLineSet("X Axis");
    xAxis->AddLine(-360, 0, 0, 360, 0, 0);
    xAxis->SetMainColor(kRed);
    xAxis->SetLineWidth(2);
    scene->AddElement(xAxis);

    TEveStraightLineSet* yAxis = new TEveStraightLineSet("Y Axis");
    yAxis->AddLine(0, 0, 0, 0, 700, 0);
    yAxis->SetMainColor(kGreen);
    yAxis->SetLineWidth(2);
    scene->AddElement(yAxis);

    TEveStraightLineSet* zAxis = new TEveStraightLineSet("Z Axis");
    zAxis->AddLine(0, 0, 0, 0, 0, 700);
    zAxis->SetMainColor(kBlue);
    zAxis->SetLineWidth(2);
    scene->AddElement(zAxis);
}

//********************************************************************
void EventDisplayBase::Configure3DCamera(TEveViewer* viewer) {
//********************************************************************
    TGLViewer* glv = viewer->GetGLViewer();
    if (glv) {
        glv->SetCurrentCamera(TGLViewer::kCameraPerspXOZ);
        glv->CurrentCamera().RotateRad(-0.5, 0.5);
        glv->ResetCurrentCamera();
        glv->RequestDraw();
    }
}

