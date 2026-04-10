#include "EventDisplayBase.hxx"
#include "OutputManager.hxx"
#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <TEveManager.h>
#include <TEveElement.h>
#include <TEveLine.h>
#include <TEvePointSet.h>
#include <TEveViewer.h>
#include <TEveScene.h>
#include <TEveWindow.h>
#include <TEveWindowManager.h>
#include <TEveBrowser.h>
#include <TGTab.h>
#include <TGString.h>
#include <TGeoManager.h>
#include <TEveGeoShape.h>
#include <TEveStraightLineSet.h>
#include <TEveProjectionManager.h>
#include <TEveProjectionAxes.h>
#include <TGLViewer.h>
#include <TGLWidget.h>
#include <TGLCamera.h>
#include <TGLPhysicalShape.h>
#include <TGLUtil.h>
#include <TGLEventHandler.h>
#include <TGLRnrCtx.h>
#include <TSystem.h>
#include <TVirtualX.h>
#include <TEveSelection.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TMarker.h>
#include <TLine.h>
#include <TLatex.h>
#include <TEveText.h>
#include <TVector3.h>
#include <KeySymbols.h>
#include <TLeaf.h>
#include <TString.h>
#include <TTreeFormula.h>
#include <TEveSelection.h>
#include <cmath>
#include <limits>
#include <unordered_map>
#include <unordered_set>

namespace {
    constexpr Color_t kRulerColor = kYellow;
}

using MeasurementAnchor = EventDisplayBase::MeasurementAnchor;

class TRulerTool {
public:
    TRulerTool(EventDisplayBase* owner, TGLViewer* viewer)
        : fOwner(owner),
          fViewer(viewer),
          fActive(false),
          fHasFirst(false),
          fAnchors(nullptr) {}

    void SetViewer(TGLViewer* viewer) {
        fViewer = viewer;
        fHasFirst = false;
    }

    void SetOwner(EventDisplayBase* owner) { fOwner = owner; }

    void SetAnchors(const std::vector<MeasurementAnchor>* anchors) {
        fAnchors = anchors;
    }

    bool IsActive() const { return fActive; }

    void ToggleActive() {
        if (fActive) {
            Deactivate();
        } else {
            Activate();
        }
    }

    void Activate() {
        if (fActive) return;
        fActive = true;
        fHasFirst = false;
        PrintHelp();
    }

    void Deactivate() {
        if (!fActive) return;
        fActive = false;
        fHasFirst = false;
        std::cout << "[Ruler] Tool disabled." << std::endl;
    }

    void ResetMeasurement() { fHasFirst = false; }

    void ClearFirstPoint() {
        fHasFirst = false;
    }

    void HandleClick(Int_t px, Int_t py) {
        if (!fActive || !fViewer) return;

        TEveElement* snapped = nullptr;
        TVector3 worldPoint;
        if (!TryGetPoint(px, py, worldPoint, &snapped)) {
            std::cout << "[Ruler] Unable to determine 3D position. Please click on a visible object." << std::endl;
            return;
        }

        // Highlight the snapped element in TEve
        if (snapped && gEve) {
            gEve->GetSelection()->UserPickedElement(snapped);
        }

        if (!fHasFirst) {
            fFirstPoint = worldPoint;
            fHasFirst = true;
            TString coords;
            coords.Form("(%.2f, %.2f, %.2f)", worldPoint.X(), worldPoint.Y(), worldPoint.Z());
            std::cout << "[Ruler] First point fixed at " << coords << " cm." << std::endl;
        } else {
            FinalizeMeasurement(fFirstPoint, worldPoint);
            fHasFirst = false;
        }

        RequestStableRedraw();
    }

    void RequestStableRedraw() {
        if (gEve) {
            gEve->Redraw3D(kFALSE);
        }
        if (fViewer) {
            fViewer->RequestDraw();
        }
    }

private:
    const MeasurementAnchor* FindAnchorForElement(const TEveElement* element) const {
        if (!fAnchors || !element) {
            return nullptr;
        }
        for (const auto& anchor : *fAnchors) {
            if (anchor.owner == element) {
                return &anchor;
            }
        }
        return nullptr;
    }

    const MeasurementAnchor* FindAnchorForHierarchy(const TEveElement* element,
                                                    std::unordered_set<const TEveElement*>& visited) const {
        if (!element || visited.count(element) > 0) {
            return nullptr;
        }
        visited.insert(element);

        if (const MeasurementAnchor* anchor = FindAnchorForElement(element)) {
            return anchor;
        }

        for (TEveElement::List_ci it = element->BeginParents(); it != element->EndParents(); ++it) {
            if (const MeasurementAnchor* ancestorAnchor = FindAnchorForHierarchy(*it, visited)) {
                return ancestorAnchor;
            }
        }

        return nullptr;
    }

    const MeasurementAnchor* FindAnchorForHierarchy(const TEveElement* element) const {
        if (!element) {
            return nullptr;
        }

        std::unordered_set<const TEveElement*> visited;
        return FindAnchorForHierarchy(element, visited);
    }

    bool TryHighlightedAnchor(TVector3& outPoint, TEveElement** snappedElement = nullptr) {
        if (!gEve || !fAnchors || fAnchors->empty()) {
            return false;
        }

        TEveSelection* highlight = gEve->GetHighlight();
        if (!highlight || highlight->NumChildren() <= 0) {
            return false;
        }

        const TEveSelection* highlightView = highlight;
        for (TEveElement::List_ci it = highlightView->BeginChildren(); it != highlightView->EndChildren(); ++it) {
            const TEveElement* hovered = *it;
            if (!hovered) continue;

            if (const MeasurementAnchor* anchor = FindAnchorForHierarchy(hovered)) {
                outPoint.SetXYZ(anchor->position.X(), anchor->position.Y(), anchor->position.Z());
                if (snappedElement && anchor->owner) {
                    *snappedElement = anchor->owner;
                }
                return true;
            }
        }

        return false;
    }

    struct CameraBookmark {
        TGLMatrix base;
        TGLMatrix trans;
        Double_t center[3]{0.0, 0.0, 0.0};
        Bool_t hasCenter = kFALSE;
        Bool_t valid = kFALSE;
    };

    bool TryGetPoint(Int_t px, Int_t py, TVector3& outPoint, TEveElement** snappedElement = nullptr) {
        if (!fViewer) return false;

        if (snappedElement) *snappedElement = nullptr;

        if (TryHighlightedAnchor(outPoint, snappedElement)) {
            return true;
        }

        if (fAnchors && !fAnchors->empty() && TrySnapToAnchors(px, py, outPoint, snappedElement)) {
            return true;
        }

        TGLCamera& camera = fViewer->CurrentCamera();
        TPoint window(px, py);
        camera.WindowToViewport(window);
        TGLLine3 ray = camera.ViewportToWorld(window.GetX(), window.GetY());
        TGLPlane plane(camera.EyeDirection(), camera.FrustumCenter());
        auto hit = Intersection(plane, ray, kTRUE);
        if (hit.first) {
            TGLVertex3 p = hit.second;
            outPoint.SetXYZ(p.X(), p.Y(), p.Z());
            return true;
        }

        return false;
    }

    bool TrySnapToAnchors(Int_t px, Int_t py, TVector3& outPoint, TEveElement** snappedElement = nullptr) {
        if (!fViewer || !fAnchors || fAnchors->empty()) {
            return false;
        }

        TGLCamera& camera = fViewer->CurrentCamera();
        TPoint window(px, py);
        camera.WindowToViewport(window);
        TGLLine3 ray = camera.ViewportToWorld(window.GetX(), window.GetY());

        TVector3 origin(ray.Start().X(), ray.Start().Y(), ray.Start().Z());
        const TGLVector3& lineVec = ray.Vector();
        TVector3 direction(lineVec.X(), lineVec.Y(), lineVec.Z());
        if (direction.Mag() == 0) {
            return false;
        }
        direction = direction.Unit();

        const MeasurementAnchor* best = nullptr;
        double bestPerp = std::numeric_limits<double>::max();
        double bestCamDist = std::numeric_limits<double>::max();

        for (const MeasurementAnchor& anchor : *fAnchors) {
            if (anchor.owner && !anchor.owner->GetRnrSelf()) {
                continue;
            }

            TVector3 toPoint = anchor.position - origin;
            double t = toPoint.Dot(direction);
            if (t < 0) {
                continue;
            }
            TVector3 closest = origin + direction * t;
            double perp = (anchor.position - closest).Mag();
            double camDist = toPoint.Mag();

            if (perp < bestPerp || (std::abs(perp - bestPerp) < 1e-3 && camDist < bestCamDist)) {
                best = &anchor;
                bestPerp = perp;
                bestCamDist = camDist;
            }
        }

        if (!best) {
            return false;
        }

        double threshold = std::max(2.0, 0.02 * bestCamDist);
        if (bestPerp <= threshold) {
            outPoint = best->position;
            if (snappedElement && best->owner) {
                *snappedElement = best->owner;
            }
            return true;
        }

        return false;
    }

    CameraBookmark CaptureCameraState() const {
        CameraBookmark bookmark;
        if (!fViewer) {
            return bookmark;
        }

        TGLCamera& cam = fViewer->CurrentCamera();
        bookmark.base = cam.GetCamBase();
        bookmark.trans = cam.GetCamTrans();
        if (Double_t* center = cam.GetCenterVec()) {
            bookmark.center[0] = center[0];
            bookmark.center[1] = center[1];
            bookmark.center[2] = center[2];
            bookmark.hasCenter = kTRUE;
        }
        bookmark.valid = kTRUE;
        return bookmark;
    }

    void RestoreCameraState(const CameraBookmark& bookmark) const {
        if (!bookmark.valid || !fViewer) {
            return;
        }

        TGLCamera& cam = fViewer->CurrentCamera();
        cam.RefCamBase() = bookmark.base;
        cam.RefCamTrans() = bookmark.trans;
        if (bookmark.hasCenter) {
            if (Double_t* center = cam.GetCenterVec()) {
                center[0] = bookmark.center[0];
                center[1] = bookmark.center[1];
                center[2] = bookmark.center[2];
            }
        }
        cam.IncTimeStamp();
    }

    void FinalizeMeasurement(const TVector3& p1, const TVector3& p2) {
        if (!fOwner) {
            std::cout << "[Ruler] Unable to store measurement (no owner)." << std::endl;
            return;
        }

        double distance = fOwner->StoreMeasurement(p1, p2);
        if (distance < 0) {
            std::cout << "[Ruler] Unable to store measurement." << std::endl;
            return;
        }

        TString distMsg;
        distMsg.Form("%.2f", distance);
        std::cout << "[Ruler] Distance = " << distMsg << " cm." << std::endl;
    }

    void PrintHelp() const {
        std::cout << "[Ruler] Tool enabled. Left-click two objects to measure distance. "
                  << "Right-click clears measurement. Press 'm' to toggle, 'Esc' to exit ruler mode." << std::endl;
    }

    EventDisplayBase* fOwner;
    TGLViewer* fViewer;
    bool fActive;
    bool fHasFirst;
    TVector3 fFirstPoint;
    const std::vector<MeasurementAnchor>* fAnchors;
};

class TGLRulerEventHandler : public TGLEventHandler {
public:
    TGLRulerEventHandler(TGLViewer* viewer, TRulerTool* tool)
        : TGLEventHandler(viewer ? viewer->GetGLWidget() : nullptr, viewer),
          fViewer(viewer), fTool(tool), fRetinaScale(1) {}

    void SetTool(TRulerTool* tool) { fTool = tool; }
    void SetRetinaScale(Int_t s) { fRetinaScale = s; }

    void SetViewer(TGLViewer* viewer) {
        fViewer = viewer;
        fGLViewer = viewer;
    }

    Bool_t HandleConfigureNotify(Event_t* event) override {
        if (fRetinaScale > 1 && event) {
            event->fWidth  *= fRetinaScale;
            event->fHeight *= fRetinaScale;
        }
        return TGLEventHandler::HandleConfigureNotify(event);
    }

    Bool_t HandleMotion(Event_t* event) override {
        if (fRetinaScale > 1 && event) {
            event->fX *= fRetinaScale;
            event->fY *= fRetinaScale;
        }
        return TGLEventHandler::HandleMotion(event);
    }

    Bool_t HandleButton(Event_t* event) override {
        if (fRetinaScale > 1 && event) {
            event->fX *= fRetinaScale;
            event->fY *= fRetinaScale;
        }
        if (!fTool || !fViewer) {
            return TGLEventHandler::HandleButton(event);
        }

        if (fTool->IsActive()) {
            if (event->fType == kButtonPress && event->fCode == kButton1) {
                fTool->HandleClick(event->fX, event->fY);
                return kTRUE;
            }

            if (event->fType == kButtonPress && event->fCode == kButton3) {
                fTool->ResetMeasurement();
                fTool->ClearFirstPoint();
                fTool->RequestStableRedraw();
                return kTRUE;
            }

            if (event->fType == kButtonRelease &&
                (event->fCode == kButton1 || event->fCode == kButton3)) {
                return kTRUE;
            }
        }

        return TGLEventHandler::HandleButton(event);
    }

    Bool_t HandleKey(Event_t* event) override {
        if (!fTool) {
            return TGLEventHandler::HandleKey(event);
        }

        if (event->fType == kGKeyPress) {
            if (event->fCode == kKey_m) {
                fTool->ToggleActive();
                fTool->RequestStableRedraw();
                return kTRUE;
            }
            if (event->fCode == kKey_Escape && fTool->IsActive()) {
                fTool->Deactivate();
                fTool->RequestStableRedraw();
                return kTRUE;
            }
        }

        return TGLEventHandler::HandleKey(event);
    }

private:
    TGLViewer* fViewer;
    TRulerTool* fTool;
    Int_t fRetinaScale;
};
#include <TEveText.h>
#include <TVector3.h>
#include <KeySymbols.h>
#include <TString.h>

//********************************************************************
EventDisplayBase::EventDisplayBase() {
//********************************************************************
    _teveInitialized = false;
    _scene3D = NULL;
    _projRPhi = NULL;
    _projRhoZ = NULL;

    // Initialize four-tab layout members
    _viewer3D = NULL;
    _viewerXY = NULL;
    _viewerXZ = NULL;
    _viewerYZ = NULL;
    _projXY = NULL;
    _projXZ = NULL;
    _projYZ = NULL;
    _sceneXY = NULL;
    _sceneXZ = NULL;
    _sceneYZ = NULL;

    // Initialize TCanvas members
    _canvasXY = NULL;
    _canvasXZ = NULL;
    _canvasYZ = NULL;

    _treeName = "EventDisplayData";
    _rulerTool = nullptr;
    _rulerEventHandler = nullptr;
    _measurementList = nullptr;
    _measurementVisible = kTRUE;
    _nextMeasurementId = 1;
}

//********************************************************************
EventDisplayBase::~EventDisplayBase() {
//********************************************************************
    // Clean up TCanvas objects (not owned by TEve)
    if (_canvasXY) {
        _canvasXY->Close();
        delete _canvasXY;
        _canvasXY = NULL;
    }
    if (_canvasXZ) {
        _canvasXZ->Close();
        delete _canvasXZ;
        _canvasXZ = NULL;
    }
    if (_canvasYZ) {
        _canvasYZ->Close();
        delete _canvasYZ;
        _canvasYZ = NULL;
    }

    if (_rulerTool) {
        delete _rulerTool;
        _rulerTool = nullptr;
    }

    ClearAllMeasurements();
    if (_measurementList) {
        _measurementList->Destroy();
        _measurementList = nullptr;
    }

    // TEve manager owns the scenes and viewers, so we don't delete them
    // Don't terminate TEveManager here as it can cause crashes
    // ROOT will clean up TEveManager when the application exits
}

void EventDisplayBase::EnsureMeasurementList() {
    if (_measurementList) {
        return;
    }

    _measurementList = new TEveElementList("Measurements");
    _measurementList->SetElementTitle("Saved ruler measurements");
    _measurementList->SetMainColor(kRulerColor);
    _measurementList->SetRnrSelf(_measurementVisible);
    _measurementList->SetRnrChildren(_measurementVisible);
    AttachMeasurementListToScene();
}

void EventDisplayBase::AttachMeasurementListToScene() {
    if (!_measurementList || !_scene3D) {
        return;
    }

    bool attached = false;
    for (TEveElement::List_ci it = _measurementList->BeginParents(); it != _measurementList->EndParents(); ++it) {
        if (*it == _scene3D) {
            attached = true;
            break;
        }
    }
    if (!attached) {
        _scene3D->AddElement(_measurementList);
    }
}

TEveElementList* EventDisplayBase::GetMeasurementList() {
    EnsureMeasurementList();
    AttachMeasurementListToScene();
    return _measurementList;
}

void EventDisplayBase::NotifyMeasurementsChanged(Bool_t requestRedraw) {
    if (_suspendMeasurementCallbacks) {
        return;
    }

    OnMeasurementsChanged();
    if (requestRedraw && gEve) {
        gEve->Redraw3D(kFALSE);
    }
}

Double_t EventDisplayBase::StoreMeasurement(const TVector3& p1, const TVector3& p2) {
    EnsureMeasurementList();
    AttachMeasurementListToScene();

    if (!_measurementList) {
        return -1.0;
    }

    Double_t distance = (p2 - p1).Mag();
    TString labelText;
    labelText.Form("%.2f cm", distance);

    auto* container = new TEveElementList(Form("Measurement %u", _nextMeasurementId));
    container->SetPickable(kTRUE);

    auto* line = new TEveLine(Form("Measurement %u Line", _nextMeasurementId));
    line->SetMainColor(kRulerColor);
    line->SetLineWidth(3);
    line->SetPoint(0, p1.X(), p1.Y(), p1.Z());
    line->SetPoint(1, p2.X(), p2.Y(), p2.Z());
    container->AddElement(line);

    TVector3 mid = 0.5 * (p1 + p2);
    TVector3 dir = p2 - p1;
    TVector3 offset(0, 0, 0);
    if (dir.Mag() > 0) {
        dir = dir.Unit();
        TVector3 up(0, 0, 1);
        offset = dir.Cross(up);
        if (offset.Mag() < 1e-3) {
            offset = dir.Cross(TVector3(1, 0, 0));
        }
        if (offset.Mag() > 0) {
            offset = offset.Unit() * 5.0;
        }
    }

    auto* text = new TEveText(labelText);
    text->SetMainColor(kRulerColor);
    text->SetFontSize(18);
    text->RefMainTrans().SetPos(mid.X() + offset.X(), mid.Y() + offset.Y(), mid.Z() + offset.Z());
    container->AddElement(text);

    _measurementList->AddElement(container);

    // ROOT 6.36+: register in the browser list tree for toggle visibility
    if (gEve) {
        gEve->AddToListTree(_measurementList, kTRUE);
        gEve->AddToListTree(container, kTRUE);
    }

    if (!_measurementVisible) {
        container->SetRnrSelf(kFALSE);
        container->SetRnrChildren(kFALSE);
    }

    SavedMeasurement entry;
    entry.container = container;
    entry.line = line;
    entry.label = text;
    entry.start = p1;
    entry.end = p2;
    entry.length = distance;
    entry.entryId = _nextMeasurementId++;
    _measurements.push_back(entry);

    NotifyMeasurementsChanged(kTRUE);
    return distance;
}

void EventDisplayBase::RemoveMeasurementById(UInt_t entryId) {
    for (auto it = _measurements.begin(); it != _measurements.end(); ++it) {
        if (it->entryId != entryId) continue;

        if (it->container) {
            it->container->Destroy();
        }
        _measurements.erase(it);
        NotifyMeasurementsChanged(kTRUE);
        return;
    }
}

void EventDisplayBase::ClearAllMeasurements() {
    for (auto& entry : _measurements) {
        if (entry.container) {
            entry.container->Destroy();
        }
    }
    _measurements.clear();
    if (_measurementList) {
        _measurementList->DestroyElements();
    }
    NotifyMeasurementsChanged(kTRUE);
}

void EventDisplayBase::SetMeasurementVisibility(Bool_t visible) {
    _measurementVisible = visible;
    if (_measurementList) {
        _measurementList->SetRnrSelf(visible);
        _measurementList->SetRnrChildren(visible);
        _measurementList->ElementChanged(kTRUE, kTRUE);
    }
    for (auto& entry : _measurements) {
        if (!entry.container) continue;
        entry.container->SetRnrSelf(visible);
        entry.container->SetRnrChildren(visible);
        entry.container->ElementChanged(kTRUE, kTRUE);
    }
    OnMeasurementVisibilityChanged(visible);
    if (gEve) gEve->Redraw3D(kFALSE);
}

void EventDisplayBase::OnMeasurementsChanged() {}

void EventDisplayBase::OnMeasurementVisibilityChanged(Bool_t) {}

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

    // Initialize TEve
    InitializeTEve();

    // Create/get 3D scene and clear previous content
    if (!gEve->GetEventScene()) {
        _scene3D = gEve->SpawnNewScene("Event Scene", "Scene holding event data");
    } else {
        _scene3D = gEve->GetEventScene();
        BeforeSceneCleared();
        _scene3D->DestroyElements();
    }

    // Draw coordinate axes in 3D view
    DrawCoordinateAxes(_scene3D);

    // Draw detector geometry (experiment-specific)
    DrawDetectorGeometry(_scene3D);

    // Draw particles and hits in 3D
    DrawParticles3D(_scene3D);

    // ROOT 6.36+: scene->AddElement() no longer auto-registers in the
    // browser list tree.  Explicitly register all top-level scene children
    // so they appear with visibility checkboxes in the Eve panel.
    for (TEveElement::List_i it = _scene3D->BeginChildren();
         it != _scene3D->EndChildren(); ++it) {
        gEve->AddToListTree(*it, kTRUE);
    }

    // Set scene name
    std::string sceneTitle = "Event Display: Run " + std::to_string(run) +
                            ", Subrun " + std::to_string(subrun) +
                            ", Event " + std::to_string(event);
    _scene3D->SetElementName(sceneTitle.c_str());

    // Redraw 3D
    gEve->Redraw3D(kTRUE);

    // Configure 3D camera
    TEveViewer* viewer3D = gEve->GetDefaultViewer();
    if (viewer3D) {
        Configure3DCamera(viewer3D);
    }

    // Install custom event handler with permanent Retina fix.
    // The handler intercepts every HandleConfigureNotify and scales
    // the viewport dimensions so the GL rendering fills the full window.
    Int_t retinaScale = 1;
#ifdef __APPLE__
    if (TGLUtil::GetScreenScalingFactor() <= 1.0f) {
        retinaScale = 2;
    }
#endif
    TGLViewer* glv = gEve->GetDefaultGLViewer();
    if (glv) {
        InstallRulerTool(glv);
        if (_rulerEventHandler && retinaScale > 1) {
            _rulerEventHandler->SetRetinaScale(retinaScale);
        }
    }

    // Apply initial Retina viewport fix
    gSystem->ProcessEvents();
    if (glv && glv->GetGLWidget()) {
        TGLWidget* glw = glv->GetGLWidget();
        Int_t w = glw->GetWidth()  * retinaScale;
        Int_t h = glw->GetHeight() * retinaScale;
        Event_t ev;
        memset(&ev, 0, sizeof(ev));
        ev.fType   = kConfigureNotify;
        ev.fWidth  = w;
        ev.fHeight = h;
        glw->HandleConfigureNotify(&ev);
        glv->RequestDraw(TGLRnrCtx::kLODHigh);
    }

    // 2D canvas event displays are intentionally disabled for now.
    // Keep only the interactive 3D view.
    std::cout << "2D canvas event displays are disabled; showing 3D view only." << std::endl;

    UpdateWindowTitles(run, subrun, event);

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
bool EventDisplayBase::GenerateDisplayByIndex(const std::string& filename, Long64_t index,
                                              const std::string& outputFile) {
//********************************************************************
    if (index < 0) {
        std::cout << "ERROR: Event index must be non-negative." << std::endl;
        return false;
    }

    TFile* file = TFile::Open(filename.c_str());
    if (!file || file->IsZombie()) {
        std::cout << "ERROR: Could not open file: " << filename << std::endl;
        return false;
    }

    TTree* tree = (TTree*)file->Get(_treeName.c_str());
    if (!tree) {
        std::cout << "ERROR: No '" << _treeName << "' tree found in the file!" << std::endl;
        std::cout << "Make sure the analysis was run with event display data saving enabled." << std::endl;
        file->Close();
        return false;
    }

    if (!tree->GetBranch("ED_run")) {
        std::cout << "ERROR: EventDisplayData branches not found in tree." << std::endl;
        file->Close();
        return false;
    }

    Int_t ed_run = 0;
    Int_t ed_subrun = 0;
    Int_t ed_event = 0;
    tree->SetBranchAddress("ED_run", &ed_run);
    tree->SetBranchAddress("ED_subrun", &ed_subrun);
    tree->SetBranchAddress("ED_event", &ed_event);

    Long64_t nEntries = tree->GetEntries();
    Long64_t displayIdx = 0;
    Int_t targetRun = 0;
    Int_t targetSubrun = 0;
    Int_t targetEvent = 0;
    bool found = false;

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        if (ed_run <= 0) {
            continue;
        }
        if (displayIdx == index) {
            targetRun = ed_run;
            targetSubrun = ed_subrun;
            targetEvent = ed_event;
            found = true;
            break;
        }
        ++displayIdx;
    }

    file->Close();

    if (!found) {
        std::cout << "ERROR: Event index " << index << " is out of range. "
                  << "Total stored events: " << displayIdx << std::endl;
        return false;
    }

    GenerateDisplay(filename, targetRun, targetSubrun, targetEvent, outputFile);
    return true;
}

//********************************************************************
void EventDisplayBase::ListAvailableEvents(const std::string& filename, const std::string& category) {
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

    std::string resolvedCategory;
    TBranch* categoryBranch = nullptr;
    TLeaf* categoryLeaf = nullptr;
    bool useExpressionFilter = false;
    TTree* anaTree = nullptr;
    TTreeFormula* selectionFormula = nullptr;

    auto makeEventKey = [](Int_t run, Int_t subrun, Int_t evt) -> Long64_t {
        return (static_cast<Long64_t>(run) << 40) ^
               (static_cast<Long64_t>(subrun) << 20) ^
               static_cast<Long64_t>(evt);
    };
    auto looksLikeExpression = [](const std::string& text) -> bool {
        if (text.empty()) return false;
        if (text.find(' ') != std::string::npos) return true;
        const std::string exprTokens = "=<>!&|()+*/[]$";
        return text.find_first_of(exprTokens) != std::string::npos;
    };

    if (!category.empty()) {
        useExpressionFilter = looksLikeExpression(category);

        if (useExpressionFilter) {
            anaTree = (TTree*)file->Get("ana");
            if (!anaTree) {
                std::cout << "WARNING: 'ana' tree not found. Cannot apply filter '" << category
                          << "'. Listing without filtering." << std::endl;
                useExpressionFilter = false;
            } else if (!anaTree->GetBranch("run") || !anaTree->GetBranch("subrun") || !anaTree->GetBranch("evt")) {
                std::cout << "WARNING: 'ana' tree missing run/subrun/evt branches. Cannot apply filter '"
                          << category << "'. Listing without filtering." << std::endl;
                useExpressionFilter = false;
            } else {
                selectionFormula = new TTreeFormula("evtDisplaySelection", category.c_str(), anaTree);
                if (!selectionFormula || selectionFormula->GetNdim() <= 0) {
                    std::cout << "WARNING: Invalid filter expression '" << category
                              << "'. Listing without filtering." << std::endl;
                    if (selectionFormula) delete selectionFormula;
                    selectionFormula = nullptr;
                    useExpressionFilter = false;
                }
            }
        }
    }

    if (!category.empty() && !useExpressionFilter) {
        std::vector<std::string> candidates;
        candidates.push_back(category);
        if (category.rfind("ED_", 0) != 0) {
            candidates.push_back("ED_" + category);
            candidates.push_back("ED_category_" + category);
        }

        for (const auto& candidate : candidates) {
            if (candidate.empty()) continue;
            TBranch* br = tree->GetBranch(candidate.c_str());
            if (br) {
                categoryBranch = br;
                categoryLeaf = br->GetLeaf(candidate.c_str());
                if (!categoryLeaf) {
                    categoryLeaf = br->GetLeaf(br->GetName());
                }
                resolvedCategory = candidate;
                break;
            }
        }

        if (!categoryBranch) {
            std::cout << "WARNING: Category/branch '" << category
                      << "' not found. Listing without category column." << std::endl;
        }
    }

    // Set up to read the data
    Int_t ed_run, ed_subrun, ed_event;
    tree->SetBranchAddress("ED_run", &ed_run);
    tree->SetBranchAddress("ED_subrun", &ed_subrun);
    tree->SetBranchAddress("ED_event", &ed_event);

    // Print header
    std::cout << "\nStored events with display data:\n" << std::endl;
    std::cout << std::setw(6) << "EDIdx" << "  "
              << std::setw(8) << "Run" << " "
              << std::setw(8) << "Subrun" << " "
              << std::setw(8) << "Event";
    if (useExpressionFilter) {
        std::cout << "  " << category;
    } else if (categoryBranch && categoryLeaf) {
        std::cout << "  " << resolvedCategory;
    }
    std::cout << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    // Loop through entries
    Long64_t nEntries = tree->GetEntries();
    int displayCount = 0;

    std::unordered_map<Long64_t, bool> passByEvent;
    if (useExpressionFilter && anaTree && selectionFormula) {
        Int_t anaRun = 0, anaSubrun = 0, anaEvt = 0;
        anaTree->SetBranchAddress("run", &anaRun);
        anaTree->SetBranchAddress("subrun", &anaSubrun);
        anaTree->SetBranchAddress("evt", &anaEvt);
        const Long64_t nAnaEntries = anaTree->GetEntries();
        for (Long64_t j = 0; j < nAnaEntries; ++j) {
            anaTree->GetEntry(j);
            bool pass = false;
            const Int_t nFormulaData = selectionFormula->GetNdata();
            for (Int_t iData = 0; iData < nFormulaData; ++iData) {
                if (selectionFormula->EvalInstance(iData) != 0.0) {
                    pass = true;
                    break;
                }
            }
            passByEvent[makeEventKey(anaRun, anaSubrun, anaEvt)] = pass;
        }
    }

    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);

        // Only show entries that have event display data (where ED_run != 0)
        if (ed_run > 0) {
            bool passFilter = true;
            if (useExpressionFilter) {
                auto it = passByEvent.find(makeEventKey(ed_run, ed_subrun, ed_event));
                passFilter = (it != passByEvent.end()) ? it->second : false;
            }
            if (!passFilter) continue;

            std::cout << std::setw(6) << i << "  "
                      << std::setw(8) << ed_run << " "
                      << std::setw(8) << ed_subrun << " "
                      << std::setw(8) << ed_event;
            if (useExpressionFilter) {
                std::cout << "  1";
            } else if (categoryBranch && categoryLeaf) {
                Double_t catValue = categoryLeaf->GetValue();
                std::cout << "  " << catValue;
            }
            std::cout << std::endl;
            displayCount++;
        }
    }

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Total: " << displayCount << " event(s) with display data" << std::endl;
    std::cout << "\nUsage:" << std::endl;
    std::cout << "  draw.EvtDisplay(run, subrun, event)" << std::endl;
    std::cout << "  draw.EvtDisplayByIndex(index)" << std::endl;
    std::cout << std::endl;

    if (selectionFormula) delete selectionFormula;
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
void EventDisplayBase::DrawDetectorCanvas2D(TCanvas* canvas, const std::string& projection_type) {
//********************************************************************
    // Default implementation - derived classes should override
    (void)canvas;
    (void)projection_type;
}

//********************************************************************
void EventDisplayBase::DrawParticlesCanvas2D(TCanvas* canvas, const std::string& projection_type) {
//********************************************************************
    // Default implementation - derived classes should override
    (void)canvas;
    (void)projection_type;
}

//********************************************************************
void EventDisplayBase::DrawAnalysisContentCanvas2D(TCanvas* canvas, const std::string& projection_type) {
//********************************************************************
    // Default implementation - derived classes should override
    (void)canvas;
    (void)projection_type;
}

//********************************************************************
void EventDisplayBase::BeforeSceneCleared() {
//********************************************************************
    SetMeasurementCallbacksSuspended(kTRUE);
    ClearAllMeasurements();
    ResetMeasurementAnchors();
    SetMeasurementCallbacksSuspended(kFALSE);
    if (_measurementList) {
        _measurementList->Destroy();
        _measurementList = nullptr;
    }
}

//********************************************************************
void EventDisplayBase::InitializeTEve()
//********************************************************************
{
    if (!gEve) {
        TEveManager::Create(kTRUE);
        std::cout << "\n✓ TEve window opened!" << std::endl;
        _teveInitialized = true;
    } else {
        std::cout << "\n✓ Updating TEve display..." << std::endl;
    }
}

//********************************************************************
TEveViewer* EventDisplayBase::Create3DView()
//********************************************************************
{
    TEveViewer* viewer = gEve->GetDefaultViewer();
    if (!viewer) {
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
void EventDisplayBase::CreateProjection(TEveProjectionManager*& proj, TEveViewer*& viewer,
                                        TEveScene*& scene, int type, const char* name) {
//********************************************************************
    // Create projection manager (constructor automatically sets the projection type)
    proj = new TEveProjectionManager(static_cast<TEveProjection::EPType_e>(type));
    gEve->AddToListTree(proj, kFALSE);

    // Create scene for this projection
    scene = gEve->SpawnNewScene(Form("%s Scene", name));

    // Add axes to the projection scene
    TEveProjectionAxes* axes = new TEveProjectionAxes(proj);
    scene->AddElement(axes);

    // Add scene to viewer
    viewer->AddScene(scene);

    std::cout << "✓ Created " << name << " projection" << std::endl;
}

//********************************************************************
void EventDisplayBase::CreateFourTabLayout() {
//********************************************************************
    // Use default 3D viewer from TEve
    _viewer3D = gEve->GetDefaultViewer();
    _scene3D = gEve->GetEventScene();

    // Disable TEve projections (unstable)
    _projXY = NULL;
    _projXZ = NULL;
    _projYZ = NULL;
    _sceneXY = NULL;
    _sceneXZ = NULL;
    _sceneYZ = NULL;
    _viewerXY = NULL;
    _viewerXZ = NULL;
    _viewerYZ = NULL;

    std::cout << "✓ 3D view ready" << std::endl;
}

//********************************************************************
void EventDisplayBase::Create2DCanvases() {
//********************************************************************
    // Create separate TCanvas windows for 2D projections
    // These are more stable than TEveProjectionManager

    Int_t canvasWidth = 800;
    Int_t canvasHeight = 600;

    // XY Projection (X vs Y, looking down Z-axis/beam)
    if (!_canvasXY) {
        _canvasXY = new TCanvas("c_XY", "XY Projection (Beam View)", 100, 100, canvasWidth, canvasHeight);
        _canvasXY->SetGrid(1, 1);
        _canvasXY->SetLeftMargin(0.12);
        _canvasXY->SetBottomMargin(0.12);
        _canvasXY->SetRightMargin(0.15); // Space for legend
        _canvasXY->SetTopMargin(0.10);
        _canvasXY->SetTicks(1, 1); // Draw ticks on all sides
    } else {
        _canvasXY->Clear();
        _canvasXY->SetGrid(1, 1);
    }

    // XZ Projection (X vs Z, looking from side)
    if (!_canvasXZ) {
        _canvasXZ = new TCanvas("c_XZ", "XZ Projection (Side View)", 150, 150, canvasWidth, canvasHeight);
        _canvasXZ->SetGrid(1, 1);
        _canvasXZ->SetLeftMargin(0.12);
        _canvasXZ->SetBottomMargin(0.12);
        _canvasXZ->SetRightMargin(0.15); // Space for legend
        _canvasXZ->SetTopMargin(0.10);
        _canvasXZ->SetTicks(1, 1); // Draw ticks on all sides
    } else {
        _canvasXZ->Clear();
        _canvasXZ->SetGrid(1, 1);
    }

    // YZ Projection (Y vs Z, looking from end)
    if (!_canvasYZ) {
        _canvasYZ = new TCanvas("c_YZ", "YZ Projection (End View)", 200, 200, canvasWidth, canvasHeight);
        _canvasYZ->SetGrid(1, 1);
        _canvasYZ->SetLeftMargin(0.12);
        _canvasYZ->SetBottomMargin(0.12);
        _canvasYZ->SetRightMargin(0.15); // Space for legend
        _canvasYZ->SetTopMargin(0.10);
        _canvasYZ->SetTicks(1, 1); // Draw ticks on all sides
    } else {
        _canvasYZ->Clear();
        _canvasYZ->SetGrid(1, 1);
    }

    std::cout << "✓ Created 2D canvas views: XY, XZ, YZ" << std::endl;
}

//********************************************************************
void EventDisplayBase::UpdateWindowTitles(Int_t run, Int_t subrun, Int_t event) {
//********************************************************************
    TString suffix = Form(" - Run %d, Subrun %d, Event %d", run, subrun, event);

    TEveViewer* viewer3D = _viewer3D;
    if (!viewer3D && gEve) {
        viewer3D = gEve->GetDefaultViewer();
    }

    if (viewer3D) {
        TString viewerTitle = TString("3D View") + suffix;
        viewer3D->SetElementName(viewerTitle.Data());
        viewer3D->SetElementTitle(viewerTitle.Data());
        viewer3D->SetNameTitle(viewerTitle.Data(), viewerTitle.Data());
    }

    if (_canvasXY) {
        TString titleXY = TString("XY Projection (Beam View)") + suffix;
        _canvasXY->SetTitle(titleXY.Data());
        _canvasXY->SetName(titleXY.Data());
    }

    if (_canvasXZ) {
        TString titleXZ = TString("XZ Projection (Side View)") + suffix;
        _canvasXZ->SetTitle(titleXZ.Data());
        _canvasXZ->SetName(titleXZ.Data());
    }

    if (_canvasYZ) {
        TString titleYZ = TString("YZ Projection (End View)") + suffix;
        _canvasYZ->SetTitle(titleYZ.Data());
        _canvasYZ->SetName(titleYZ.Data());
    }
}

//********************************************************************
void EventDisplayBase::DrawCoordinateAxes(TEveScene* scene) {
//********************************************************************
    TEveElementList* axesGroup = new TEveElementList("Axes");
    scene->AddElement(axesGroup);

    // Add coordinate axes with labels
    TEveStraightLineSet* xAxis = new TEveStraightLineSet("X Axis");
    xAxis->AddLine(-360, 0, 0, 360, 0, 0);
    xAxis->SetMainColor(kRed);
    xAxis->SetLineWidth(2);
    axesGroup->AddElement(xAxis);

    TEveStraightLineSet* yAxis = new TEveStraightLineSet("Y Axis");
    yAxis->AddLine(0, 0, 0, 0, 700, 0);
    yAxis->SetMainColor(kGreen);
    yAxis->SetLineWidth(2);
    axesGroup->AddElement(yAxis);

    TEveStraightLineSet* zAxis = new TEveStraightLineSet("Z Axis");
    zAxis->AddLine(0, 0, 0, 0, 0, 700);
    zAxis->SetMainColor(kBlue);
    zAxis->SetLineWidth(2);
    axesGroup->AddElement(zAxis);
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

//********************************************************************
void EventDisplayBase::InstallRulerTool(TGLViewer* glViewer) {
//********************************************************************
    if (!glViewer) return;

    if (!_rulerTool) {
        _rulerTool = new TRulerTool(this, glViewer);
    } else {
        _rulerTool->SetOwner(this);
        _rulerTool->SetViewer(glViewer);
    }
    _rulerTool->SetAnchors(&_measurementAnchors);

    TGEventHandler* handler = glViewer->GetEventHandler();
    TGLRulerEventHandler* rulerHandler = dynamic_cast<TGLRulerEventHandler*>(handler);

    if (!rulerHandler) {
        rulerHandler = new TGLRulerEventHandler(glViewer, _rulerTool);
        glViewer->SetEventHandler(rulerHandler);
    } else {
        rulerHandler->SetTool(_rulerTool);
        rulerHandler->SetViewer(glViewer);
    }

    _rulerEventHandler = rulerHandler;
}

//********************************************************************
void EventDisplayBase::ResetMeasurementAnchors() {
//********************************************************************
    _measurementAnchors.clear();
    if (_rulerTool) {
        _rulerTool->SetAnchors(&_measurementAnchors);
        _rulerTool->ResetMeasurement();
        _rulerTool->ClearFirstPoint();
    }
}

//********************************************************************
void EventDisplayBase::RegisterMeasurementAnchor(TEveElement* owner, const TVector3& position) {
//********************************************************************
    if (!std::isfinite(position.X()) || !std::isfinite(position.Y()) || !std::isfinite(position.Z())) {
        return;
    }
    MeasurementAnchor anchor;
    anchor.position = position;
    anchor.owner = owner;
    _measurementAnchors.push_back(anchor);
}

//********************************************************************
void EventDisplayBase::RegisterMeasurementAnchor(TEveElement* owner, Double_t x, Double_t y, Double_t z) {
//********************************************************************
    RegisterMeasurementAnchor(owner, TVector3(x, y, z));
}

