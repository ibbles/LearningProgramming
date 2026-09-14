# Settings

Assign a value to a setting with `:set NAME=VALUE`.
For example: `:set shiftwidth=2`
Show a setting's current value with `:echo &NAME`
For example: `:echo &shiftwidth`


# Fundamentals

Neovim is based on commands and motions.
A command is something that does something to something. (duh)
A motion is something that moves the cursor.
The two are often combined.
The idea is that the command controls _what_ should be done and the motion control _where_ it should be done.
For example there is a command to copy, which Neovim calls to yank.
The copy command does to specify what to copy.
There is no copy-line command (Well..., there is, but for now pretend there isn't.), or copy-between-parenthesis command.
Instead we combine the copy command with a motion that makes the cursor move over the characters we want to copy.
For example, consider the follow line of code

```cpp
int main(int argc, char* argv[]);
    ^
```

where the cursor is placed on the marked location, the `m` in `main`.
The copy command is bound to `y`, hitting `y` will start a copy but nothing is copied immediately.
Instead Neovim waits for the user to give a motion to control what should be copied.
There is a motion that moves the cursor forward to the next occurrence of a character.
This motion is triggered with `f`, for forward, followed by which character to move to.
I call that character a _parameter_ of the forward motion, not sure if that is the official name.
So, for example, `f(` moves the cursor forward to the next `(`.
Try it on the line of code above, place the cursor anywhere before the `(` and then type `f(`.
The cursor will move to the `(`.
Now combine it with the yank (copy) command `y`, i.e. place the cursor on the marked `m` and type `yf(`.
You will now have `main(` in the yank buffer.
There is also a motion that moves to just before a character: `t`.
Place the cursor on the `m` again and use `t` instead of `f`: `yt(`.
You will now have `main`, without the `(`, in the yank buffer.
So:
- `y`: Start a yank.
- `t`: Move to, but not over, a character.
- `(`: The character to move to.
Or, the previous sequence:
- `y`: Start a yank.
- `t`: Move to, including, a character.
- `(`: The character to move to.

The command for deleting, which include a yank, is `d`.
So we can type a similar sequnce of keys as above, but using `d` instead of `y` to delete-and-yank characters intead of just copying them.
- `d`: Start a delte.
- `t`: Move to, but not over, a character.
- `(`: The character to move to.

If starting on the `m` in `main`, this will delete and yank `main`.

This system is very flexible.
With only a few building blocks, i.e. commands and motions, we can build a large library of higher-level operations.
Many Neovim tutorials, cheat-sheets, and forum posts list cryptic sequences of keystrokes for these higher-level operations without explaining their constituent parts.
I encourage you to break them down, to identify what the commands, motions, and parameters are.
This note lists a large number of commands, motions, and higher-level operations.
I will try to explain them as best as I can.

Some operations, such as yanking or deleting a line, is so common that there is a specific command for it that doesn't follow the basic command + motion pattern.
The command to delete (and yank) a line is `dd` and the command to yank a line is `yy`.
The second `d` in `dd` is not a motion, and the same goes for the second `y` in `yy`.
We cannot do `dy` and get `d` for delete and `y` as the motion for "one line", as we might have though from learning `yy` from a cheat-sheet.

Many commands can be repeated.
A repeat count is given before the command.
So `4dd` will delete and yank four lines.
And `4f(` will move four `(`s forward.
Test it on this line of code:
```cpp
sum(map(getAuditCallback(), filter(getData(), (d)->d.amount > 7)));
```

Some commands operate on "things".
A "thing" can be a buffer, a line, a window, etc.
By default such commands operate on the current "thing", like the buffer that has focus or the line the cursor is on.
You can prepend the command with `%` to operate on all "things".
For example, `:bd`, short for Buffer Delete, deletes the current buffer.
To delete all buffers do `:%bd`.
When running a shell command with `:!` `%` instead expands to the relative path to the current file.


# Cursor Navigation - Motions

Single-step motions:
- `h`: Left one character.
- `j`: Down one line.
- `k`: Up one line.
- `l`: Right one character.

Group-step motions:
- `w`: Forward one word.
- `b`: Backward one word.
- `W`: Forward one white-space separated word.
- `B`: Backward one white-space separated word.

Within-line motions:
- `0`: To start of line.
- `^`: To first character on line.
- `$`: To end or line.
- ` `: To last character on line. (TODO What is the key for this?)
- `fX` where `X` is any character: Move forward to the closest `X` on the same line.
- `FX` where `X` is any character: Move backward to the closes `X` on the same line.
- `tX` where `X` is any character: Move forward to before the closest `X` on the same line.
- `TX` where `X` is any character: Move backward to after the closest `X` on the same line. After as in higher column value.
- `;`: Repeat the prior move to closest character.

Between-line motions:
- `#gg` where `#` is a number: Go to line.
- `:#` where `#` is a number: Also go to line number.
- `gg`: Go to first line.
- `G`: Go to last line.
- `H`: Go to top of screen.
- `M`: Go to middle of screen.
- `L`: Go to bottom of screen.
- `CTRL+d`: Half-page down.
- `CTRL+u`: Half-page up.

Viewport:
- `zt`/`zz`/`zb`: Move viewport so that the current line is at the top, middle, or bottom of the screen.
- `%`: Jump between matching brackets.
- `{`/`}`: Move between paragraphs, i.e. empty lines.

Marks:
-`ma` where `a` is any lower-case character: Create a file local mark named `a` at the current location.
    - Not sure what names are valid. Only single-character? Any printable character?
- `'a` / `\``: Jump to mark, I assume.
    - Only within the current file.
    - What is `\``?
- `mA`: Create a global mark named `A` at the current location.
- `'A` / `\``: Jump to global mark.

- `''`: Jump to the previous cursor row.
    - Edit location, or any location?
- `\`\``: Jump to the previous cursor location.
- `g;`/`g,`: Jump to previous edit location.


Commands used with an LSP:
- `gd`: Go to definition.
- `gr`: Go to reference.
- `K`: Show documentation for word under cursor.


# Jump List

[(10)](https://neovim.io/doc/user/motion/#jump-motions)

- `:jumps`: Show the jump list.
- `CTRL+o`: Move to the next older entry in the jump list.
- `CTRL+i`: Move to the next newer entry in the jump list.


# Text Modifications

Enter insert mode: `i` (insert), `a` (insert after)
- `I`: go to start of line and enter insert mode.
- `A`: Go to end of line and enter insert mode.
- `c`/`C`/`cc`: Like `d`/`D`/cc` but also enter insert mode.
- `o`/`O`: Create a new line below or above and enter insert mode.

- `x`: Delete (cut) character under cursor.
    - Can be combined with `p`, paste, to swap / transpose two characters.

- `dd`: Delete the current line.
- `D`: Delete to end of line.
- `daw`: Delete a word.
- `caw`: Change a word, i.e. delete it and enter insert mode.

Delete lines with `#dd` where `#` is the number of lines to delete.
If `#` is not given then it defaults to 1.

Use `yy` instead of `dd` to copy lines instead of deleting.


- `u`: Undo.
- `Ctrl+r`: Redo.

- `r`: Reaplce the current character with the next charater you type.
- `R`: Enter replace (overwrite) mode.

-`gwip` or `gwap`: Reflow the paragraph to the current text width.


# Copying (Yanking) Text

- `y`: Copy selected text. See _Visual Mode_ (`V`).
- `Y`: Copy from the cursor to the end of the current line.
- `y$`: Copy from the cursor to the end of the current line.
- `yy`: Copy the current line.
- `#yy` where `#` is the number of lines to copy.
- `"+y`: Copy to system clipboard.
    - I don't know what `"+` actually means. Investigate.
    - Does the `"+` prefix work with the other copy commands as well?

- `yiW`: Yank a non-whitespace word.
- `p`: Paste the yanked text.

To copy a range of complete lines with visual mode:
- Shift+V to enter visual mode and start a selection on the current line.
- Navigate to the end of the range.
- `"y`


- `~`/`g~`/`gu`/`gU`: Change upper / lowercase.

- `J`: Join lines, moving the line below up to the end of the current line.
    - Basically delete the next newline. Or rather, replace the newline with a space.
- `gJ`: Join lines without adding a space between them.



# Visual Mode / Selecting Text

Visual mode is used to select text, for example to copy it.
- `v`: Enter character-wise visual mode, starting a new selection at the current cursor location.
- `V`: Enter line-wise visual mode, starting a new selection at the current cursor location.
- `Ctrl+v`: Enter block-wise visual mode, selecting a rectangular block of characters.

Use regular cursor navigation to position the end of the selection.

- `viw`: Select the current word.
- `viW`: Select the current non-whitespace word.

Selecting between brackets (`(){}[]<>` I assume all of these work.):
- `viX` where `X` is a bracket: Select everything between the brackets.
- `vaX` where `X` is a bracket: Select everything between the brackets and the brackets themselves.
    - If you are not between a pair of brackets the next bracket on the line will be used.
    - The same as `fXvaX` where `X` is a bracket.
    - This can be combined with a yank:
        - `yaX` where `X` is a brace:
            - `y`: Start a yank command.
            - `a`: Start a motion to a brace on the same line.
            - `X` where `X` is a brace: Move to the brace, select everything to, and including, the matching brace.


# Paragraph Management

- `gqq`: Wrap a long line.
- `vipJ`: Unwrap a paragraph to a single line.

Run these two after each other to rewrap a paragram.
Binding this to a keyboard shortcut is a good idea if you write a lot of text.


# Buffers

Close a buffer with `:bd`, for Buffer Delete.
Close all buffers with `:%bd`, for All Buffer Delete.
This has the unfortunate side-effect of also closing all windows except for one, no idea why.
The [documentation](https://neo.vimhelp.org/windows.txt.html) is clear that this is an intended defect:

>  Any windows for this buffer are closed

Open a file in a buffer in a new tab with `:tabe <PATH>`.
Tabcompletion works.

Switch between buffers with `gt` and `gT`.
I haven't had much success with this, typing `gt` does nothing.
(
I opened three files with `neovim file1 file2 file3`.
Made a split with `:split`.
Switched to the other window with `CTRL+w`.
Typed `gt` to switch from `file1`.
Nothing happened.
)

List the buffers with `:buffers`.
Each line is:
- Buffer ID
- Flags
    - `%`: The current buffer.
    - `#`: The alternative buffer, i.e. the most recent buffer.
- Modified state
    - `+`: Has unsaved changes.
- Path to the file that backs the buffer.
- Line the cursor is on.

The following two moved in `:buffers` list order, not recently used order.
- `[b`: Switch to previous (or next) buffer.
- `]b`: Switch to next (or previous) buffer.

Switch to the alternative buffer, i.e. the most recent buffer, with `C-^` or `:e #`.

Swith to a specific buffer with `:buffers` to list the buffers and then `:b #` where `#` is a buffer ID.

The Jump List can also move between buffers.


# Splitting Windows

- `:#split {FILE}` where `#` is an optional number and `{FILE}` is an optional file path: Split the current window into top and a bottom windows.
  - If given, the number is the number of lines to allocate to the new window.
- `:#vsplit {FILE}` where `#` is an optional number and `{FILE}` is an optional file path: Split the current window into left and right windows.
- `:new`: Create a new window with a new empty file above the current window.
  - Is this really an empty file, or an empty file-less buffer?
- `:vnew`: Create a new window with a new empty file to the left of the current window.

- `:close`: Close a window.
- `:only`: Close all other windows.


# Moving Between Windows

- `CTRL-W w`: Move between windows.
- `CTRL-W h`: Move left.
- `CTRL-W j`: Move down.
- `CTRL-W k`: Move up.
- `CTRL-W l`: Move right.
- `CTRL-W t`: Move to the top window.
- `CTRL-W b`: Move to the bottom window.


# Moving Windows

- `CTRL-W H`: Move window to the far left.
- `CTRL-W J`: Move window to the bottom.
- `CTRL-W K`: Move window to the top.
- `CTRL-W L`: Move window to the far right.


# Resizing Windows

- Click and drag the status line.
- `# CTRL-W +` where `#` is an optional number of lines: Increase the size of a window.
- `# CTRL-W -` where `#` is an optional number of lines: Decrease the size of a window.
- `# CTRL-W _` where `#` is an optional number of lines: Set number of lines. If no number then make window as large as it can be.


# Commands

- `.`: Repeat the last command.
    - This is useful in conjunction with a search to repeat a command for a bunch of search hits.
    - Use `/` to start a search, run a command on the first hit, repeatedly press `n.` to go to the next hit and repeat the command.
    - You can press `n` multiple times, without `.`, to skip over some hits.

`.` does not repeat editor commands, i.e. `:`-prefixed ones.
I don't yet know how to repeat those.


# Shell Commands

A `:` command that starts with `:!` is a shell command.
For example, `:!pwd` will print the current working directory, which kinda like Neovim's project root.

In a shell command `%` exapnds to the relative path to the current file.
It should often be quoted to ensure the path is sent as a single command line argument: `"%"`.
For example, `:!ls -l "%"` prints some information about the current file.
For example, `:!clang-format -i -style=file "%"` runs clang-format on the current file.

## Reading Shell Command Output Into Buffer

By default the shell commmand's output is displayed in a temporary window and is then lost.
Alternatively, you can read the output into the current buffer.
This is done by prepending `r`, for Read, to the command.
- `:r !SHELL_COMMAND`: Run `SHELL_COMMAND` as an external process and read the command's output into the buffer.

For example: `:r !pwd`.
This will read the output of `pwd` into the current buffer.
No output is displayed until the shell command completes, which can be a problem.
If you want the output in a new buffer, i.e. not read into e.g. a file's buffer, use `:enew` first to create a new buffer.


## Writing Buffer Into Shell Command Input

Use `:w !SHELL_COMMAND` to write the current buffer's contents into the stdin of `SHELL_COMMAND`.


# Indentation

The `expandtab` setting controls whether indentation is done with tabs or spaces.
When `expandtab` is true indentation is done with spaces.

The `shiftwidth` setting control how wide an indentation is.
If `shiftwidth` is 0 then `tabstop` is used instead.

A literal tab character (`\t`) is rendered `tabstop`  wide.

`~/.config/vim/init.vim`
```
set tabsop=8

set softtabstop=4
```


- `>>`/`<<`: Indendt and unindent the current line.
- `==`: Auto-indent the current line.
- `#==`: Where `#` is a number, auto-indent the next `#` lines.


# Search

Start a search in the current file by typing `/` followed by a pattern to search for and end with Enter [(2)](https://neovim.io/doc/user/usr_03.html#03.8).
Use `n` and `N` top step between matches.
Use `:noh` to end the search.

To search up instead of down, use `?` instead of `/`.

- `*`/`#`: Start a search for the word the cursor is currently on.


- `f`/`F`/`t`/`T`: Search for a single character on the current line.
    - `fX` where `X` is any character: Move forward to the next `X`.
    - `FX` where `X` is any character: Move backward to the next `X`.
    - `;`: Repeat the last search
    - Zap to char, basically.
    - These are useful in combination with commands that operate on locations.
    - For example, `dfw` would delete everything up to and including the letter `w`.

Search in all files with `:grep -rni PATTERN .`.
This will display a list of search results.
The list is non-interactive, i.e. we can see the results but we can't go to them.
To open an interactive list use `:copen`.
Navigate to an interesting search result and press Enter to open it.

We can also search with `:!grep`, i.e. the external `grep` binary.
This may provide more powerful search facilities, but the results won't be shown in `:copen`.
(
I may be confusing `:grep` and `:vimgrep`.
Perhaps `:grep` is the same as `:!grep`, but with `:copen` support and `:vimgrep` is the less powerful fallback.
If so, there is no need to ever run `:!grep` as far as I can tell.
Well, if you want to do a search that doesn't clear the current contents of `:copen`.
)

# Replace

Replace a word:
- `*`: Search for the word under the cursor.
- `SHIFT-n`: Go back to the previous match, i.e. where we started the search.
- `cgn`: Operate on this match.
- Type the new word.
- `ESC`: Finish typing the world.
- `n`: Go to next match.
- `.`: Redo the replace command.
- Repean `n.` for every match to replace.


To search/replace in a `sed`-like fashion on the current line:
```
:s/OLD/NEW/
```
This will replace the first occurrence of `OLD` with `NEW` on the current line.
You can use other characters, such as `,`, instead of `/` it you want.

`OLD` is a regular expression, so if you want to search for `.` you should write `\.`.

To replace all occurences of `OLD` on the line add a `g` (For Global?) at the end of the command:
```
:s/OLD/NEW/g
```

To manually accept or reject each replace add a `c` (For Confirm?) to the end of the command:
```
:s/OLD/NEW/gc
```

To search replace the entire buffer and not just the current line, add the all-things prefix `%`:
```
:%s/OLD/NEW/gc
```

Let'c summarize:
- `:`: Start a (name?) command
- `%`: Operate on all things, in this case lines in the buffer.
- `s`: Start a `sed`-like search/replace.
- `/`: Select a separator character, does not need to be `/`.
- `OLD`: String to search for.
- `/`: Separator character, marks the end of the search string.
- `NEW`: String to insert.
- `/`: Separator character, marks the end of the insert string.
- `g`: Global, i.e. find all matches on each line.
- `c`: Confirm, i.e. ask Yes/No for each match.



# Files

- `:wall` or `:wa`: Save all.
- `:Explore`: Open the directory holding the current file.
- `:Lexplore`: Open a file tree on the left side.


## `netrw`

`netrw` is not very well regarded, with frequent complaints about bugs.

`netrw` is Neovim's built-in filesystem browser.
It is opened with `:Explore` or `Lexplore`.
It has a few shortcuts of it's own:
- `%`: Createa new file.
- `d`: Create a new directory.


# Auto-Completion Complete

[(7)](https://neovim.io/doc/user/usr_24/#_completion)

- `CTRL-P`: Match previous.
- `CTRL-N`: Match next. (I think.)


# Windows / Panes / Tabs

[_VIM USER MANUAL_ > _Splitting windows_ by  Bram Moolenaar @ neovim.io](https://neovim.io/doc/user/usr_08.html#usr_08.txt).

A window is split with `:split`, which will split horizontally, i.e. one window above the other.
Pass a parametero to `:spit` to open that file, e.g. `:split my_file`.
To open a window without any file use `:new`.
Pass a prefix number argument to set the height of the new window, e.g, `:50split my_file` to create a window 50 lines tall.

Switch between windows with `CTRL+w w` or `CTRL+w CTRL+w`.
Close a window with `:close`.
Close all other windows with `:only`.

Change the size of a window either by dragging its status line with with `CTRL+w +` (CTRL+SHIFT+= on a US keyboard) and `CTRL+w -`.
Prefix with a number to change by that many lines, e.g. `50CTRL+w +` to increase the window by 50 lines.
Set a specific number of lines with `#CTRL+w _` where `#` is a number.


# Macros

[(9)](https://www.geeksforgeeks.org/linux-unix/macros-in-vi-editor/)

A macro is a recording of a sequence of actions.
A macro is first recorded and then replayed.
When recorded, the macro is samed to a register.
This means that we can have multiple recorded macros.
Registers are identified by letters.
- `qX` where `X` is a letter: Start recording of a macro into `X`.
    - For example: `qa`: Start recording into macro `a`.
While the macro is being recorded, perform a sequnce of commands.
Conclude the macro recording with `q`.
- `qX...q` where `X` is a letter and `...` is a sequence of commands: Record a macro performing `...` and store it in register `X`.

To replay a macro, use `@` followed by the macro's register letter.
- `@X` where `X` is a letter: Play back the macro stored in register `X`.
    - For example: `@a`: Play back the macro stored in register `a`.

I had though that typing `.` after playing back a macro would run the macro again, but it does not.
We can run a macro multiple times by proving a count.
- `#qX` where `#` is a number and `X` is a macro register name.


# Text Objects

`ci[` / `da(` / `yi"`
I'll investigate this another time.


# Setting Options Temporarily

`:set {OPTION}={VALUE}`

For example: `set shiftwidth=2`

Not sure if this is per-window or for the entire session.

Skip the `={VALUE}` part to print the current value.
For example: `set shiftwidth`

# References

- 1: [_Setting autoindentation to spaces in Neowin_ by Maëlan @ stackoverflow.com](https://stackoverflow.com/a/51995699)
- 2: [_Neovim docs_ > _Simple Searches_ @ neviom.io](https://neovim.io/doc/user/usr_03.html#03.8)
- 3: [_How to copy in vim_ @ learnvim.com](https://www.learnvim.com/guides/how-to-copy-in-vim)
- 4: [_Vim Command Tier List_ by AdamFrencen @ youtube.com 2025](https://www.youtube.com/watch?v=tAn-7ozV_r0)
- 5: [_Wrap and unwrap a line in Vim_ by Dave @ davetang.org 2023](https://davetang.org/muse/2023/07/02/wrap-and-unwrap-a-line-in-vim/)
- 6: [_Splitting windows_ by Bram Noolenaar @ neovim.io](https://neovim.io/doc/user/usr_08/)
- 7: [_Completion_ by Bram Moolenaar @ neovim.io](https://neovim.io/doc/user/usr_24/#_completion)
- 8: [_Vim As Your Editor_  by ThePrimeagen @ youtube.com 2022](https://www.youtube.com/playlist?list=PLm323Lc7iSW_wuxqmKx_xxNtJC_hJbQ7R)
- 9: [_Macros in Vi editor_](https://www.geeksforgeeks.org/linux-unix/macros-in-vi-editor/)
- 10: [_Neovim docs_ > _Motion_ > _Jump_ @ neovim.io](https://neovim.io/doc/user/motion/#jump-motions)

