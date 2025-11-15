Git is a version control system.
That means that it tracks how the contents of a set of files is changed over time.
It is not a continuous tracking of every change make to the files.
Instead we make explicit named snapshots.
The creation of such a snapshot is called to make a commit.

Git is optimized for text files with line semantics.
By "line semantics" I mean that each line is its own thing and we don't do line rewrapping/reflow like one would with a long paragraph of natural language text.
Git has its origin in software development and is mostly used to store source code.

The core of the Git version control system is the `git` executable.
`git` is a CLI tool, meaning that we type `git` commands at a terminal.
There are also may GUI Git tools, many of which invoke the `git` executable to perform actions.
`git` is typically invoked as `git COMMAND` where `COMMAND` is some Git command that performs some action.
This note describes many such Git commands.

# Repository / Working Copy

To create a new empty repository that working copies can be created from but that cannot itself be used as a working copy:
```shell
git init --bare --initial-branch=main .
```
This will create the internal Git file system structure directly in the current directory.
Can only be done in an empty directory.

To create a new repository in the current working directory use the `init` command:
```shell
git init --initial-branch=main .
```
This will create the internal Git file system structure in a subdirectory named `.git`.
Files and directories that already exists in the current working directory will not be modified.

To create a working copy from a previously created directory:
```shell
git clone URL [PATH]
```
`URL` can be a file system path on the same machine.


# Commit

A commit records the changes that was made to the file since the previous commit.
Such a change is sometimes called a delta.
The sequence of all commits from the first when the file was created to the current one describes the history of the file and when the changes described by all those commits are applied one after the other we will arrive at the current version of the file.
The previous commit is called the current commit's parent.
The chain of parents going back to the initial commit is called the current commit's ancestors.

The first commit in a Git repository is sometimes called the initial commit.

A commit stores a bunch of information:
- Which files are being changed by the commit.
- What changes are made to each of the changed files.
- Which commit the changes builds upon, called the parent commit.
	- A commit can have multiple parents, see _Merging_.
- The author of the change.
- The date and time the commit was made.
- A hash that identifies the commit.
	- The parent commit is stored as the parent commit's hash.

A commit is created with the `git commit` command.

## Commit Commands

Create a new commit
```shell
git commit

git commit -m"MESSAGE"
```
# Branch

A branch is a way to name sequences of commits.
A branch is a named pointer to a specific commit.
All ancestors if that commit are also considered part of that branch.
Branches are used to logically divide up work.
A working copy is always on a specific branch, unless the working copy is in a detach head state.
You can see which branch the working copy is on with
```shell
git branch --show-current
```
The branch pointer is updated to the new commit when we make a new commit while being on that branch.
Multiple branches can point to the same commit.
When a commit is made only the current branch is advanced to the new commit.
Other branches on the parent commit remain on the parent commit.
List the local branches with
```shell
git branch
```
The current branch is marked with a `*`.

To also list branches on remotes use the `--all` flag:
```shell
git branch --all
```

A repository starts of with a single default branch,  often named `main` or `master`.

A new branch is created with one of:
```shell
git branch BRANCH_NAME

git checkout -b BRANCH_NAME

git switch -c BRANCH_NAME
```
All three forms will create a new branch that points to the current commit, that is, `HEAD` is unchanged.
With the first form, `git branch`, the new branch  will be created "in the background", i.e. it will not be made the current branch.
The second form, `git switch -c`, is new, and experimental with Git 2.23, and is a way to manage branch switching that will replace `git checkout` for that purpose.
The third form, `git checkout -b`, both creates the new branch and checks it out, i.e. makes it current.
`git checkout` has multiple different use-cases and `git switch` was introduced as a safer option to `git checkout` so that one doesn't accidentally perform one of `git checkout`'s other functions when a branch switch is intended.


## Branch Commands

# Merge

A merge can be seen as the inverse of making a branch.
A branch produces a situation where two (or more) separate commits have the same parent, a diverging fork.
A merge is a commit that has two (or more) parents, a converging fork, a joining.
Remember that a commit describes the state of the repository that is created by applying all the commit's ancestor commits from the initial commit up until the current commit.
By having two parent commits a merge commit combines to parallel sequences of commits together.
The merge commit says "I want to include all the work from this parent over here and this one over here.".
If there are conflicting changes in the parallel commit paths that the creation of the merge will have a merge conflict that must be resolved before the merge commit can be created.
So a marge commit describe not only which branches are being merged but also the additional changes necessary to resolve any merge conflicts.

The workflow is:
- Create and switch to branch.
- Make changes and commits.
- Merge.

Just like regular commits, a merge commit is made on a particular branch.
If we are on `main` and merge another branch then the commits in that branch's  history is merged into `main`.
The other branch is left unchanged.
If we are on the other branch and merge `main` the changes made on `main` since the other branch was created or last merged is brought into that other branch.


## Fast-Forward Merge

There are two kinds of merges.
The one described above is the kind that creates a merge commit.
A merge commit is required when there are commits in both of the merged branches.
The other type of commit happens when there are new commits on the branch we are merging in but not on the current branch.
In this case the branch we are merging in represents a set of pure additions to the current branch.
There has never been any actual forking, the branch simply moved forward while the current branch was standing still.
In this case Git can "fast-forward" the current branch to the merged branch's commit.
By default `git merge` will do a fast-forward if possible and create a merge commit if required.
If we want to force the creation of a merge commit even when a fast-forward would have been possible then we can give the `--no-ff` flag:
```shell
# Merge and force the creation ofa merge commit.
git merge --no-ff
```

In some cases we may expect to be able to do a fast-forward but want to make sure that no merge commit is created unexpectedly. 
By passing `--ff-only` we tell Git to abort the merge, i.e. do nothing, if a merge would have otherwise been created.
```shell
# Only allow fast-forward merge, abort if a merge commit would have been required.
git merge --ff-only
```

Fast-forward is possible when the current branch is an ancestor of the branch being merged.
In this case the merged branch has seen everything in the current branch.
No conflicts are possible in this case since by definition there is nothing to conflict with.
The current branch has not changed since the merged branch was created.


## Merge Commands

```shell
# Bring changes from another branch into the current one.
git merge OTHER_BRANCH

# Create a merge commit even if a fast-forward would have been possible.
git merge --no-ff OTHER_BRANCH

# Only merge if a fast-forward is possible, abort otherwise.
git merge --ff-only OTHER_BRANCH
```


# Rebase

Rebase is typically used when bringing changes in `main` back to a development branch so that the branch is kept up-to-date with other work that has been done and merged to `main`.
Rebase is an alternative to merge when bringing changes in one branch into another.
A merge takes two parallel series of commits and creates a new commit that has both of them as parents.
That is, a merge merges two parallel paths into a single path.
Rebase, on the other hand, makes it appear as-if all the work on the branch had been done on top of the current commit in `main` instead of the commit from which the branch was actually created.
The file changes recorded in the commits on the branch are applied not to the original `main` commit but instead to the current `main` commit.
The result of this is a set of new commits on the branch, and the old commits are discarded.
In essence, a rebase takes the commits from their current place in the commit history and plots them down somewhere else.
The places are the "bases" that the rebase name hints at.
The branch was created from a particular base commit but now we want to move it to a new base.

If the file changes recorded in the branch's commits are not compatible with the the contents of the working copy state of the new base commit then there is a merge conflict that must be resolved.
The result of the merge conflict resolution is changes to the commits being moved so that they become compatible with the new base.
So a rebase does not just move the branch from one base commit to another, it also changes the moved commits.
Even if no changes are necessary to the file content changes stored with the moved commit the commits themselves are still changed since the parent of the first commit in the branch is changed.
Since the parent commit is an integral part of a commit changing the base parent of the branch changes the hash of the first commit on the branch.
Since the hash of the first commit changed the parent of the second commit must change to that new hash.
Since the second commit had its parent changed this changes the hash of the second commit as well so we must also update the third commit and so on for all commits.
So after a rebase ALL commits on the branch are brand new commits and the old commits are gone.

One of the advantages of rebasing over merging is that is produces a nicer looking history.
Instead of the history branching and merging in a way that reflects the realities of software development and how the work was actually performed we throw all of that information away an instead present a neatly packaged lie.

# References

- 1: [_Learn Git Branching_ @ learngitbranching.js.org](https://learngitbranching.js.org/)