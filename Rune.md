https://rune.build

# Philosophy 

Rune is a terminal-like code editor.
It uses a command-based interface.
Type `:` to start a command.

A Rune session is hierarchical:
- Workspace: An open filesystem directory.
- Window: A tile on the screen. Shows one tab.
- Tab: A container for content, which can be a file, a terminal, or task output.

The windows and tabs are separate things, a window can show any tab.

Rune have 9 workspace slots, meaning 9 workspaces can be opened per Rune process.
Switch to a workspace with `<meta-#> where `#` is `[0-9]`.

Windows can be split, creating two windows side-by-side or one on top of the other.

A tab holds a file, a terminal, or task output.


# Super Key Problems

The Rune keybinds make frequent use of the `<meta>` key, which by default is the Super physical key.
This is a problem because the Super key is often used by the desktop environment or window manager, meaning many of Runes key bindings never reach Rune.
See https://github.com/unstablebuild/rune/issues/92 for details.
I'm not sure how to handle this.
Wait for the problem to be resolved upstream?
I can't really use Rune until then since so many fundamental features are unavailable as keyboard shortcuts.
Reconfigure the key bindings?
Doable, but then I create a mismatch in my editor from the defaults, which I would prefer to avoid.
It would make this note more difficult to write, should I write the default binding or my custom one?
I can do a rebind-light, not trying to remap everything but instead add temporary bindings for only the things I need until the probelm has been solved upstream.
In these notes I can have diffrent notation for the default vs my tempoary custom bindings.
Let's try that.
- Default key bindings are marked with a `-`.
* Temporary custom key bindings are marked with a `*`.
+ Permanent custom key bindings are marked with a `+`.



# Workspace

A workspace is a project.
A workspace has its own window layout tabs, and root directory.
Each Rune window can have nine workspaces open.
Open a directory as a workspace with `:workspaceopen PATH`.

- `<ctrl-meta-#>` where `#` is `[0-9]`: Focus workspace on slot `#`.
    - Does not work with GNOME.
* `<shift-meta-#>` where `#` is `[0-9]`: Focuse workspace in slot `#`.
- `<shift-meta-#>` where `#` is `[0-9]`: Move workspace to slot `#`.
    - Disabled while the Super key problem is fixed.
* `<ctrl-shift-meta-#>` where `#` is `[0-9]`: Move current workspace to slot `#`.
- `<meta-`>`: Search workspace.


- `:workspaceopen`: Open a workspace in the first free slot.


# Navigation

Use Vim-style navigation, meaning we have the following directions:
- `H`: Left.
- `J`: Down.
- `K`: Up.
- `L`: Right.

- `<meta-D>` where `D` is a direction: Move focus between windows.
* `<ctrl-meta-D>`: Move focus beween windows.
- `<shift-meta-D>` where `D` is a direction: Swap location with the window in the given direction.
- `<alt-D>` where `D` is `H` or `L`: Move between tabs.
- `<shift-meta-D>` where `D` is `H` or `L`: Move the current tab.


To move focus between buttons on a pop-up dialog:
- `<ctrl-h>`: Move to the button to the left.
- `<ctrl-l>`: Move to the button on the right.
There might be up (`k`) and down (`j`) variants as well, I have not yet seen a dialog with multiple rows of buttons.


# Window Management

Windows are typically controlled with the `meta` modifier, which in Rune is the Super physical key.
This is a problem since my desktop environment / window manager use that key for several of its own bindings and the desktop environment / window manager takes precedence, making those bindings unusable in Rune.
I have added a few custom key bindings to work around this problem, marked with `*` instead of `-` below.
Typically I add the Ctrl modifier, but that is not always possible.

## Creation And Closing

- `<meta-n>`: Open a new window.
* `<ctrl-meta-n>`: Open a new window.
- `:windownew [left|down|up|right]`: Create a new window, either at the default location or in the given direction.
- `<meta-w>`: Close the current window.

* `<ctrl-alt-meta-h>`: Make new windows appear below the current one, i.e. horizontal split.
- `:windownew [right|left|up|down]`: Open  new window in the current tab.

- `:terminalneworsplit`: Create a new window with a terminal.
* `<ctrl-alt-meta-v>`: Make new windows appear to the right of the current one, i.e. vertical split.
- `windowdefaultsplit (horizontal|vertical)`: Set the default split direction.

## Navigation

* `<ctrl-meta-D>` where `D` is a direction: Move focus in direction `D`.
- `<shift-meta-D>` where `D` is a direction: Move the current window in direction `D`.

## Resizing

- `<alt-meta-h>`: Make the current window narrower.
- `<alt-meta-j>`: Make the current window shorter.
- `<alt-meta-k>`: Make the current window taller.
- `<alt-meta-l>`: Make the current window wider.
The resizing tries to keep the window center at the same position on the screen.
It possible, it will alternate between expanding / contrating on the left/up and right/down side.

- `<shift-meta-f>`: Fullscreen the current window.
  - Un-fullscreen with the same chord, or by moving focus to another window with e.g. `<ctrl-meta-D>`.


## Layouts

You can use command aliases to programmatically create window layouts.
An example that creates a layout three windows by splitting twice.
```yaml
command:
    aliases:
        - windowcloseall
        - windownew right
        - windownew right
```

# Tab Management

Tag are generally controlled with the `alt` modifier, but sometimes `meta`.

- `<meta-t>`: Create a new tab.
- `<alt-w>`: Close the current tab.
- `<alt-D>` where `D` is `[hl]`: Focus the left or right tab.
- `<shift-alt-D>` where `D` is `[hl]`: Move the current tag left or right.
- `<alt-#>` where `#` is a number: Focus tab `#`.
- `<shift-alt-#>` where `#` is a number: Move the current tab to slot `#`.
- `<alt-`>: Search tabs.

- `:windowconverttab`: Convert a window to a tab.
There does not seem to be a way to go the other way, to turn a tab into a window.


# File Management


- `:edit`: Open a file in the current window.
- `<shift-tab>`: Open the file explorer side bar.

The file explorer lists one file per line.
Modify the line to modify the file.
- Change the text: rename the file.
- Create a new line: create a new file.
- Delete a line: delete the file.
- `return`: Open the file in a new tab in the most recent window.
- Save the buffer (`:write`) to apply the changes.

Buffers not backed by any file can be created with
- `:edit memory:///NAME`


# Terminal

The terminal is also modal.
Press `esc` to switch from input mode to normal mode.

- `<meta-enter>`: Open a terminal, either in the current empty window or a new window.
* `<ctrl-meta-enter>`: Open a terminal.


# Searching

- `<meta-p>`: Search for a file by name.
    - THIS COLLIDES WITH GNOMES WRECK MULTI-MONITOR FEATURE.
    - TODO REBIND.
- `<meta-\`: Search for text in the entire workspace.
- `:jumptoast`: Search for a function within the current file.
- `<alt-f>`: Search for a function within the current file.
- `<alt-d>`: Go to definition, find the definition for the symbol under the cursor.
- `<alt-r>`: Search for references to the symbol under the cursor.
- `<alt-i>`: Search for types implementing an interface, or the interfaces a type implements.
- `<alt-t>`: Show the documentation for the symbol under the cursor.
- `alt-shift-d>`: Go to definition, find the definition for a symbol by name.
- `/`: Search for text within the current file.

Some of the above are from the `fuzzy-search` extension.
Use the `pkg install fuzzy-search` console command to install it.


# Cursor History

- `<ctrl-o>`: Jump backwards in the cursor history.
- `<ctrl-i>`: Jump forwards in the cursor history.
- `:cursorhistory jump`: Display a list of cursor history locations.
    - TODO Is this branching-awayre? Test it.


# Command Prompt

Rune is controlled with commands.
Commands are either entered by name on triggered with a key binding.
Commands are sometimes called `:` commands because the command prompt is opened with the `:` key while in Vim normal mode.
When entering a `:` command the command prompt is in Vim insert mode by default and Esc closes the dialog instead of entering Vim normal mode.
Use `shift-esc` to get access to the full editor functionality.
- `shift-esc`: Enter Vim normal mode in the command input text field.
    - Not really true [(1)](https://docs.rune.build/learn/command-prompt/#edit-the-prompt-with-shift-esc), it replaced the simplified input text field with a full editor, which uses whatever editor mode you have selected with the `editor.mode` config.

- `:workspaceopen PATH`: Open a workspace.
- `:edit PATH`: Open a file.
- `:tutorial start (basics|navigation|agents)`
- `:tutorial stop`


# Task

A task is a process that is not a shell that has been given its own.
The window is collapsed by default.
Window collor indicate status:
- Gray: running.
- Green: finished successfully.
- Red: finished with an error.
- yellow: canceled.

A task can also be put in a tab.
Do this when the output, and not just the exit code, is important.

- `<esc>`: Minimize the focused task.

- `:tasknew`: Create a new task.
- `:tasknewtab`: `:tasknew` followed by `:windowconverttab`.
- `:taskfocus`: Open and focus a task window.
- `:taskclose`: Stop and close the focused task.

# Console

The console is kinda like the command prompt, but for commands that have output.
The console is used to setup up rune, the promp to drive it.

- `:console`: Open the console.

Console commands:
- `pgk install PACKAGE`: Install a package.


# Configuration

Open the configuration file with `<meta+,>`.
By default it is in `~/.rune/config.yaml`.
You can pass `-d` flag to use a different data/config directory, such as `$USER/config/rune/`.
(
With or without the `rune` bit?
Is `rune` added automatically, or are the files directly in the given directory?
)

To reload the configuration run the `:workspacereload` command.
This will reload the non-GUI settings.
There is no way to reload the GUI settings, they are applied when the GUI window is created.
Create a new GUI window with `:guiwindownew`.


# Workarounds

## Incorrect Colors

The default color theme in Rune maps color names to colors that we don't typically associate with those names.
For example, the color blue is rendered using a red color.
The theme `sanfilippo` seems to be the one whose colors best match the color names.


# References

- 1: [_Command Prompt_ > _Edit the prompt with <shift-esc>_ @ docks.rune.build](https://docs.rune.build/learn/command-prompt/#edit-the-prompt-with-shift-esc)
- 2: [_Layout Management_ @ docs.rune.build](https://docs.rune.build/learn/layout-management/)


