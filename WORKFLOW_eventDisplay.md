# Event display workflow (current)

Technical reference for the current event-display implementation in `highland`.

## Scope

- How display is invoked from `DrawingTools`
- `EventDisplayBase` write/read/render responsibilities
- Data contract in `EventDisplayData`
- What is generic in `highland` vs analysis-specific extensions

## Key files

- `src/highland2/highlandTools/src/DrawingTools.cxx`: public entry API (`EvtDisplay`, `EvtDisplayByIndex`, list events).
- `src/highland2/highlandTools/src/DrawingTools.hxx`: event display API declarations.
- `src/highland2/highlandTools/src/EventDisplayBase.hxx`: abstract base interface and data contract.
- `src/highland2/highlandTools/src/EventDisplayBase.cxx`: tree I/O and rendering pipeline implementation.
- `src/highland2/highlandCore/src/AnalysisLoop.cxx`: analysis lifecycle hooks where microtree content is produced.

## Invocation flow from user side

1. User opens an output ROOT file with event-display content.
2. User calls one of:
   - `draw.EvtDisplay(run, subrun, event)`
   - `draw.EvtDisplayByIndex(index)`
   - `draw.ListEvtDisplay(...)`
3. `DrawingTools` ensures an event display object exists:
   - if manually set via `SetEventDisplay(...)`, uses that,
   - otherwise auto-detects class from `ED_ClassName` in tree and instantiates via ROOT reflection.

## Data contract (`EventDisplayData`)

`EventDisplayBase` uses a dedicated tree named `EventDisplayData`.

Common expectations:
- `ED_ClassName`: class name used for auto-instantiation.
- Event-identification fields consumed for retrieval/listing:
  - `ED_run`
  - `ED_subrun`
  - `ED_event`

Write path:
- `EventDisplayBase::InitializeTree(...)` creates the tree and common variables.
- `EventDisplayBase::FillTree(...)` fills common fields and delegates experiment-specific fields to derived implementation.

## Rendering pipeline

`EventDisplayBase::GenerateDisplay(...)` performs:

1. Open ROOT file and read `EventDisplayData`.
2. Locate requested event payload.
3. Reset prior scene/measurement state.
4. Initialize TEve and scene containers.
5. Draw base coordinate axes.
6. Delegate detector/particle rendering to derived hooks.
7. Build/update 2D `TCanvas` projections (XY, XZ, YZ).
8. Update titles and redraw.
9. Optionally save image if output path provided.

Notable current features in base implementation:
- interactive ruler tool and persistent measurements,
- measurement anchors for snapping,
- category-aware listing support in event listing.

## Generic vs analysis-specific responsibilities

Generic in `highland`:
- visualization framework lifecycle,
- storage/retrieval contract,
- TEve/canvas orchestration.

Expected from analysis-specific package (e.g. `highlandPD`):
- concrete derived class implementation,
- detector-specific drawing primitives,
- event payload branch definitions,
- analysis overlays and custom semantics.

## Current branch/commit (template)

- Branch: `<fill>`
- Commit: `<fill>`
- Date checked: `<fill>`

## Known gaps/uncertainties

- This repository provides generic display infrastructure; neutral-kaon-specific visual semantics are defined in downstream analysis code.
- If runtime dictionaries/libraries for the concrete display class are not loaded, auto-instantiation will fail.
- Event listing/rendering assumes expected `ED_*` fields are filled by derived implementation.

## Next verification actions

1. Validate one end-to-end event display session using latest neutral kaon output.
2. Confirm the concrete derived class name written in `ED_ClassName` matches loadable dictionary symbols.
3. Keep display usage examples synchronized with `highlandPD` run/manifests.
