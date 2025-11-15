Git is a version control system.
This means that it tracks how the contents of a set of files in a directory tree is changed over time.
It is not a continuous tracking of every change make to the files, instead we make explicit named snapshots.
The creation of such a snapshot is called to make a commit.
The directory tree with files is called a working copy.
The files Git needs to work, often stored in a directory  named `.git` is called a repository.

Git is optimized for text files with line semantics.
By "line semantics" I mean that each line is its own thing and we don't do line rewrapping/reflow like one would do when editing a long paragraph of natural language text.
Git has its origin in software development and is mostly used to store source code.

The core of the Git version control system is the `git` executable.
`git` is a CLI tool, meaning that we type `git` commands in a terminal.
There are also may GUI Git tools, many of which invoke the `git` executable to perform actions.
`git` is typically invoked as `git COMMAND` where `COMMAND` is some Git command that performs some action.
This note describes many such Git commands.

# Repository / Working Copy

A repository is a directory where the files and directories that Git needs, including the history of the files tracked by the repository, are stored.
A working copy is a directory that holds the files being tracked by a Git repository, possibly with the content from an earlier commit and/or with local changes.
A working copy always holds its repository in a `.git` directory in the root of the working copy.

To create a new empty repository, also called a bare repository, that working copies can be created from but that cannot itself be used as a working copy:
```shell
git init --bare .
```
This will create the internal Git file system structure directly in the current directory.
Can only be done in an empty directory.

We can also create a combined repository / working copy from a possibly non-empty directory:
```shell
git init .
```
This will create the internal Git file system structure in a subdirectory named `.git`.
Files and directories that already exists in the current working directory will not be modified and can be tracked by adding them to a commit.

Both ways to initialize a repository support giving a custom name to the initial branch:
```shell
git init [--bare] --initial-branch=main .
```

To create a working copy from a previously created repository:
```shell
git clone URL [PATH]
```

`URL` can be a file system path on the same machine.
The optional path is the directory where we want the working copy to be created.
If we don't specify a path then a directory named after the repository will be created in the current working directory.
The target directory must be empty.


## Repository / Working Copy Commands

```shell
# Create a new repository + working copy of the current working directory.
git init .

# Create a new bare repository in the current working directory.
git int --bare .

# Create a new working copy from a previously created repository.
git clone URL [PATH]
```


# Commits

A commit records the changes that was made to the tracked files since the previous commit.
Such a change is sometimes called a delta.
The sequence of all commits from the one where the file(s) was added to the current one describes the history of the file and when the changes described by those commits are applied one after the other we will arrive at the current version of the file.

The previous commit is called the current commit's parent.
The chain of parents going back to the first commit is called the current commit's ancestors.
The first commit in a Git repository is often called the initial commit.
The commit we are currently on is called `HEAD`.
The commits form a directed asyclic graph rooted at the initial commit.
The graph may have multiple leaf commits where each such commit typically represent the head of some development effort.
`HEAD`, the current commit, is often one of the leaf commits.
The contents of the files in the working copy reflects the state represented by the `HEAD` commit and all its ancestors, plus any local changes we may have made on top of that state.

A commit stores a bunch of information:
- Which files are being changed by the commit.
- What changes are made to each of the changed files.
- Which commit the changes builds upon, called the parent commit.
	- A commit can have multiple parents, see _Merging_.
- The author of the change.
- The date and time the commit was made.
- A hash that identifies the commit.
	- The parent commit is stored as the parent commit's hash.

A commit is created with:
```shell
git commit
```
This form will open a text editor where a commit message is be written.

To supply the commit message directly use the `-m` flag:
```shell
git commit -m"MESSAGE"
```

A working copy is always on a particular commit, the current commit.
This commit is called `HEAD` because it is the head of the history, the sequence of commits, that leads to the current state of the working copy.


## Commit Commands

```shell
# Create a new commit. Opens a text editor for the commit message.
git commit

# Create a new commit with the given commit message.
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
This means that `HEAD` points to the same commit as that branch.
When we make a new commit in in this state both `HEAD` and the current branch is updated to point to that new commit.

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


## Moving A Branch

A branch can be moved, meaning that we can make a branch point to a different commit.
```shell
git branch -f BRANCH COMMIT
```

As an example, consider a case where we commit something bad to a branch named `feature/some-feature` and we want to take a step back.
```shell
git branch -f feature/some-feature feature/some-feature^
```

The `-f` (force) bit is required because normally `git branch` is used to create new branches, not modify existing branches.


`git branch` cannot be used to move the current branch.
For this you need `git reset`.


## Branch Commands

```shell
# List local branches.
git branch

# List local and remote branches.
git branch --all

# Show the name of the current branch.
git branch --show-current

# Create a new branch in the background.
git branch BRANCH_NAME

# Create a new branch and switch to it.
git switch -c BRANCH_NAME  # New way.
git checkout -b BRANCH_NAME  # Old way.
```

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


# Interactive Rebase

During an interactive rebase the user can control the way the commits are applied.
The control interface is a text file that is initialized by Git to contain a list of the relevant commits with their hashes and messages.
Each line also contains a word that describe what should be done with the commit:
- Keep: The commit is applied.
- Drop: The commit is not applied.
- Squash: The change contained  in the commit is included but not as a separate commit but instead inserted into one of the neighboring commits.
	- Which one, above or below?
By editing the file we can perform the following actions:
- Keep the commit: Leave 
- Reorder commits: By reordering the lines in the file.
- Drop commits: By 

Pass the `-i` flag to start an interactive rebase.
```shell
git rebase -i COMMIT
```
The operation performed here is to take all commits in the range from the first common ancestor of `HEAD` and `COMMIT` to the current commit and apply them on top of `COMMIT`.


# Reset

Moves a branch backwards along the parent references.
It's like rewinding time for that branch and resetting it back to an earlier state.

To move the current branch backwards one commit relative to `HEAD`:
```shell
git reset HEAD~1
```

A reset will not only move the current branch to another commit, it will also update the working copy to the state of that commit.

Resetting a branch rewrites history.
This can cause issues if done with branches that exists not only on our own machine.


# Revert

Create a new commit that does the opposite of another commit.
This effectively undos the changes done by that other commit.
Behaves just like a regular commit so there are no issues with using  this with shared branches.
The other commit remains in the history but the changes it made are hidden by the changes in the created commit.

```shell
git revert HEAD~1
```


# Cherry-Pick

A cherry-pick creates a copy of some commit and applies to to your current branch.
It acts as-if a user had looked at the diff for the commit and hand-applied it to the working copy and the comiitted.

```shell
git cherry-pick COMMIT [COMMIT..]
```

Cherry-pick only picks exactly the commits you listed and not the entire history as a merge would.

# Tags


# Identifying Commits

Each commit has a hash and can be identified by that hash.
The hashes should all be unique, if two commits have the same hash, a hash collision, then Git will be having trouble.
Since they must be unique the hashes are quite long, to reduce the likelihood of hash collisions.
Here is an example hash: `fed2da64c0efc5293610bdd892f82a58e8cbc5d8`.
When identifying a commit using a hash we don't need to type out the entire hash.
It is enough to type out enough characters to uniquely identify a single commit that exists within the repository.

We can also use branches to identify commits.
Each branch points to a commit and naming that branch also names the commit the branch points to.

We can also identify commits based on their location relative to some other commit.
This is called a relative ref.
We can base a relative ref from a commit hash, a branch, a tag, or `HEAD`.

There are multiple ways to create a relative ref, here using `HEAD` as the base:
- `HEAD^` (caret operator): Move to the parent.
	- If there are multiple parents then I assume the first  parent is used.
		- TODO Verify the above.
	- Can be given multiple times: `HEAD^^^` means go to the third ancestor.
- `HEAD~#` (tilde operator): Follow `#` parent links where `#` is an integer.


#  Modifying An Ancestor Commit

Sometimes we want to modify the changes contained in an older commit.
There are multiple ways to achieve  this.

## Interactive Rebase / Amend / Interactive Rebase Method

Use an interactive rebase to move the commit we want to change to the end of the history.
```shell
git rebase -i COMMIT_TO_MODIFY^
```
Reorder the commits so that the  commit we want to change is at the top (Or is it bottom?).

Make the wanted changes to the files in the working copy.
Commit with `--amend` to insert the change into the already existing commit.
```shell
git commit --amend
```

Do another interactive rebase to reorder the now modified commit back to its original location.
```shell
git rebase -i NO_IDEA_WHAT_TO_PASS_HERE
```

I don't know how well this method handles conflicts during the rebase steps.

Since this rewrites history it is not safe to with commits that has been pushed.

# Detached `HEAD`

Normally a working copy is on a particular branch, meaning that the branch in question is current and `HEAD` points to the same commit as that branch.
Or rather, `HEAD` points to the branch, which points to the commit.
If we make a new commit then both `HEAD` and the branch is updated to point to the new commit.
Or rather, the branch is updated and since `HEAD` points to the branch `HEAD` tags along to the new commit.
Our working copy is attached to that branch through `HEAD`.

A working copy does not need to be attached to any branch.
We can chose to attach it to a commit instead.
In this case `HEAD` points directly to a commit instead of to a branch.
This state is called detached `HEAD` since the commit we are currently on, `HEAD`, is not linked to any branch.
There may still be branches that point to the same commit, but making new commits will not update any branch to point to that new commit.

`HEAD` is a symbolic name that refers to the thing we are currently on, which may be a branch or a commit.
When it is a commit then we are in a detach `HEAD` state.

To detach `HEAD` from the current branch and point it directly to a commit, switch to that commit:
```shell
git switch HASH
```


# References

- 1: [_Learn Git Branching_ @ learngitbranching.js.org](https://learngitbranching.js.org/)
- 2: [_Finding what branch a Git commit came from_ @ stackoverflow.com 2010,2018](https://stackoverflow.com/questions/2706797/finding-what-branch-a-git-commit-came-from)