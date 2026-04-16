# Settings

Assign a value to a setting with `:set NAME=VALUE`.
For example: `:set shiftwidth=2`
Show a setting's current value with `:echo &NAME`
For example: `:echo &shiftwidth`


# Cursor Navigation

Single character / line: `h`, `j`, `k`, `l`
- `w`: Forward one word.
- `b`: Backward one word.
- `W`: Forward one white-space separated word.
- `B`: Backward one white-space separated word.
- `0`: To start of line.
- `^`: To first character on line.`
- '$': To end or line.

- `fX` where `X` is any character: Move forward to the closest `X` on the same line.
- `;`: Repeat the prior `fX` where `X` is any character, i.e. step forward to the next `X` on the same line.

- `#gg` where `#` is a number: Go to line.
- `:#` where `#` is a number: Also go to line number.
- `gg`: Go to first line.
- `G`: Go to last line.
- `H`: Go to top of screen.
- `M`: Go to middle of screen.
- `L`: Go to bottom of screen.
- `zt`/`zz`/`zb`: Move viewport so that the current line is at the top, middle, or bottom of the screen.
- `%`: Jump between matching brackets.
- `{`/`}`: Move between paragraphs, i.e. empty lines.

-`ma`: Create a file local mark named `a` at the current location.
    - Not sure what names are valid. Only single-character? Any printable character?
- `'a` / `\``: Jump to mark, I assume.
    - Only within the current file.
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


# Text Modifications

Enter insert mode: `i` (insert), `a` (insert after)
- `I`: go to start of line and enter insert mode.
- `A`: Go to end of line and enter insert mode.
- `c`/`C`/`cc`: Like `d`/`D`/cc` but also enter insert mode.
- `o`/`O`: Create a new line below or above and enter insert mode.

- `x`: Delete character under cursor.

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

To copy a range of complete lines with visual mode:
- Shift+V to enter visual mode and start a selection on the current line.
- Navigate to the end of the range.
- `"y`


- `~`/`g~`/`gu`/`gU`: Change upper / lowercase.

- `J`: Join lines, moving the line below up to the end of the current line.
    - Basically delete the next newline. Or rather, replace the newline with a space.
- `gJ`: Join lines without adding a space between them.


# Search Replace

To replace all occurences of `OLD` with `NEW`:
```
:%s/OLD/NEW/g
```

The format is similar to `sed`.

To ask for confirmation for every replacement add a `c` to the end:
```
:%s/OLD/NEW/gc
```


# Visual Mode

Visual mode is used to select text, for example to copy it.
- `v`: Enter character-wise visual mode, starting a new selection at the current cursor location.
- `V`: Enter line-wise visual mode, starting a new selection at the current cursor location.
- `Ctrl+v`: Enter block-wise visual mode, selecting a rectangular block of characters.

Use regular cursor navigation to position the end of the selection.


# Paragraph Management

- `gqq`: Wrap a long line.
- `vipJ`: Unwrap a paragraph to a single line.

Run these two after each other to rewrap a paragram.
Binding this to a keyboard shortcut is a good idea if you write a lot of text.


# Buffers

Close a buffer with `:bd`, for Buffer Delete.

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
Another way to switch buffers is with `:buffers` to list the buffers and then `:b #` where `#` is a buffer number.



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
    - You can press `n` multiple times to skip over some hits.


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

There is also something with `?`, not sure what that does yet.
Search backwards, perhaps?

- `*`/`#`: Start a search for the word the cursor is currently on.


- `f`/`F`/`t`/`T`: Search for a single character on the current line.
    - Zap to char, basically.
    - These are useful in combination with commands that operate on locations.
    - For example, `dfw` would delete everything up to and including the letter `w`.


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


# Auto-Completion Complete

[(7)](https://neovim.io/doc/user/usr_24/#_completion)

- `CTRL-P`: Match previous.
- `CTRL-P`: Match next.




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
- 2: [_Simple Searches_ @ neviom.io](https://neovim.io/doc/user/usr_03.html#03.8)
- 3: [_How to copy in vim_ @ learnvim.com](https://www.learnvim.com/guides/how-to-copy-in-vim)
- 4: [_Vim Command Tier List_ by AdamFrencen @ youtube.com 2025](https://www.youtube.com/watch?v=tAn-7ozV_r0)
- 5: [_Wrap and unwrap a line in Vim_ by Dave @ davetang.org 2023](https://davetang.org/muse/2023/07/02/wrap-and-unwrap-a-line-in-vim/)
- 6: [_Splitting windows_ by Bram Noolenaar @ neovim.io](https://neovim.io/doc/user/usr_08/)
- y: [_Completion_ by Bram Moolenaar @ neovim.io](https://neovim.io/doc/user/usr_24/#_completion)

