# Workflow for pull requests

### Working in local repo from terminal /in shell:

1. Use `git branch` to see what Git branch you are currently on in your terminal.
2. Check Fetch latest remote branches: `git fetch origin`.
3. See which branches exist on the remote: `git branch -r`.
4. You can detect which branches are ahead of main. For example: `git log origin/main..origin/<branch> --oneline`.
   You can also use `git diff --stat origin/main..origin/branchname`, which show a summary of changes between main and branch
   (how many files were added/modified). However, this alone doesn't guarantee these branches have a pull request (PR).
   The only way to know for sure which branches have a PR is to look them up directly in github.
6. Check out (i.e., switch to a different branch or commit, in github lingo) the branch from the PR:
   `git checkout -b branchname-local origin/branchname`. -b flag creates new local branch branchname-local and sets it to track
   the remote branch origin/branchname. Finally, the command checkout switches your working directory to the newly created branch.
8. If you want the newly created local branch to exist in the remote repository as a remote branch too, do:
   `git push -u origin branchname-local`. This creates a remote branch origin/branchname-local on GitHub, and links your local
   branchname-local to that remote branch (because of -u, shorthand for --set-upstream).
10. Review changes with `git diff origin/main..branchname-local`.
11. Build/run/review the code.
12. Merge PR in github.
13. Clean up your local branch: First switch back to main: `git checkout main`. Update with the merged changes: `git pull origin main`.
    Delete local branch safely: `git branch -d branchname-local`. If needed, with caution and when absolutely sure it is no longer needed,
    you can also remove the remote branch: `git push origin --delete branchname`.
15. To merge another branch, e.g. a libft branch into your newly created local branch where you're currently working on: `git merge origin/libft`.
