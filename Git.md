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
A working copy is always on a specific branch, unless it is in a detach head state.
You can see which branch the working copy is on with
```shell
git branch --show-current
```
The branch pointer is updated to the new commit when we make a new commit while being on that branch.
Multiple branches can point to the same commit.
When a commit is made only the current branch is advanced to the new commit.
Other branches on the parent commit remain on the parent commit.

A repository starts of with a single default branch,  often named `main` or `master`.


Branches are used to logically divide up work.


## Branch Commands

# References

- 1: [_Learn Git Branching_ @ learngitbranching.js.org](https://learngitbranching.js.org/)