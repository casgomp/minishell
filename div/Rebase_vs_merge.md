### Merging vs Rebasing

[Atlassian - Merging vs Rebasing](https://www.atlassian.com/git/tutorials/merging-vs-rebasing#the-golden-rule-of-rebasing)

**Merging:** 
- Ties together the histories of the feature branch and the main branch.
- Considered safer
- Allows preserving the complete history of the project.
- Tradeoff is the extra merge commits that could "pollute" the branch history, which could be problematic when searching history for debugging purposes
  (although there are some workarounds like git log --grep=merge --invert-grep or something similar to filter out merge commits (although for git bisect
  I'm not sure there would be a workaround).
- There is no git merge alternative for cleaning up local commits with an interactive rebase.

**Rebasing:** 
- Moves the entire feature branch to begin on the tip of the main branch.
- The project's history is rewritten. i.e., rebase makes new "replayed" commits (sort of copies) from the feature branch's commits and replays them on
  top of main. These "replayed" commits are actually new commits with new ID's even though their content may be the same.
- "Interactive" rebasing is possible, i.e., defining what the branch will look like after rebasing (for example squashing several commits into one,
- editing commit messages, reordering commits, and dropping commits).
- It is possible to rebase onto the parent branch, but also onto an earlier commit in the feature branch.
- Its is also okay to rebase onto a remote branch instead of main. This can happen when collaborating on the same feature with another developer and you
- need to incorporate their changes into your repository.
- Provides a clean, linear history without additional commits as is the case when merging. This makes it easier to find bugs with tools like git log and git bisect.
- The tradeoff of this cleanliness is that some commits are rewritten.
- Can be risky. Especially if done on a public branch. (See "Golden Rule of Rebasing" section in the article).

### Rebasing workflow (from article)

- The first step (when using rebase) is to create a dedicated branch for each feature. This gives the necessary branch structure for safely using rebasing.
- One of the best ways to incorporate rebasing into your workflow is to clean up local, in-progress features. In other words, periodically performing
  interactive rebasing. This way you can make sure each commit in your feature is focused and meaningful.
- When using interactive rebasing, it is possible to rewrite only a few previous commits (e.g. using `git checkout feature` followed by `git rebase -i HEAD~3`),
  but also to rewrite the entire feature branch (e.g. using `git merge-base feature main` to get the ID of the original base commit where the feature branched
  of from main, followed by `it rebase -i <that_commit_id>`). In any of these two cases, you are not bringing any changes from main, you are just rewriting
  your own branch's commits to clean up. This use of interactive rebasing only works for private feature branches, i.e., if collaborating with other developers
  via the same feature branch, then the branch is public and you're not allowed to re-write its history.
- Rebasing into a remote branch (e.g. when working on a public branch) works exactly the same way as when integrating upstream changes from main.
- If using pull requests as part of the code review process, avoid using git rebase after creating the pull request. i.e., while pull request is open, merge
  main into feature. Don't rebase (see next point).
- Since you are not allowed to re-write commits in the main branch, you have to eventually use git merge to integrate the feature. However, by performing a
  rebase before the merge, you're assured that the merge will be fast-forwarded, resulting in a perfectly linear history. This also gives you the chance to
  squash any follow-up commits during the pull request.

### Careful with:

**git push --force:**

- This will overwrite the remote main branch. 
- One of the only times to use force-push is when performing a local cleanup (i.e., interactive rebasing) and deciding to use that cleaned-up version instead of
  another previously (untidy) pushed version. 
- Important that no one else is working off of the commits from the original version of the feature branch. That would be risky, because it rewrites history out
  from under them. It is only safe if you are the only one working on the branch.
