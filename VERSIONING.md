# Versioning and reproducibility

Use annotated tags for production-ready reconstruction states.

## Tag format
- `reco-vMAJOR.MINOR.PATCH`
- Example: `reco-v1.0.0`

## Rules
- Tag only commits already merged to `master`.
- For each `highland` tag, define the paired `highlandPD` tag used in production.
- Record run metadata in `RUN_MANIFEST.md` using the shared spec in `/Users/jcapo/cernbox/DUNE-IFIC/Software/HIGHLAND_NEW/RUN_FRAMEWORK.md`.

## Create and publish a tag
```bash
git checkout master
git pull --ff-only origin master
git tag -a reco-vX.Y.Z -m "Reconstruction release vX.Y.Z"
git push origin reco-vX.Y.Z
```
