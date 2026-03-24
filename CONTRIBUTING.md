# Contributing to highland

## Branching model
- `master`: stable branch used for production analyses.
- `feature/<topic>`: normal development work.
- `hotfix/<topic>`: urgent fixes that must be released quickly.

## Commit style
- Keep commits focused and atomic.
- Use clear messages describing intent, for example:
  - `feat(neutralKaon): add annihilation vertex categorization`
  - `fix(eventDisplay): correct neutral candidate filtering`
  - `refactor(pdUtils): split neutral utilities by responsibility`

## Pull request policy
- Do not push changes directly to `master`.
- Open a PR from your branch into `master`.
- Include:
  - concise summary of behavior changes
  - validation method (compile/run or analysis checks)
  - rollback impact if relevant

## Suggested local workflow
1. `git checkout master && git pull --ff-only`
2. `git checkout -b feature/<topic>`
3. Develop, test, and commit in small chunks.
4. `git push -u origin feature/<topic>`
5. Open PR and merge after review/checks.
