# Contributing to highland

## Branching model
- `master`: stable branch used for production analyses.
- `neutralKaon/<feature>`: normal neutral kaon development work.
- `neutralKaon/hotfix/<topic>`: urgent neutral kaon fixes.
- For coordinated changes across repositories, use the same suffix in both repos (example: `neutralKaon/eventdisplay-cleanup`).

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
2. `git checkout -b neutralKaon/<feature>`
3. Develop, test, and commit in small chunks.
4. `git push -u origin neutralKaon/<feature>`
5. Open PR and merge after review/checks.
