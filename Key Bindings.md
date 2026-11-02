This is where I document how I configure my key bindings.
It doesn't really have much to do with programming exactly, but I must write it somewhere.

# System

Key bindings owned by the system, i.e. the operating system, window manager, and global shortcuts.
All these key bindings are prefixed with `os-prefix`.
`os-prefic` is `alt+ctrl+super`.

In GNOME I have configured the following in the Tiling Shell extension.

- `h`: Focus window to left.
- `j`: Focus window down.
- `k`: Focus window up.
- `l`: Focus window to right.

- `y`: Move window a tile to left.
- `u`: Move window a tile down.
- `i`: Move window a tile up.
- `o`: Move window a tile right.

- `n`: Span window a tile left.
- `m`: Span window a tile down.
- `,`: Span window a tile up.
- `.`: Span window a tile right.


In GNOME I have configured the following in Settings > Keyboard.

- `e`: Move window a workpace left.
- `r`: Move window a workspace right.
- `a`: Move window a monitor left.
- `s`: Move window a monitor right.

- `q`: Switch to workspace on the left.
- `w`: Switch to workspace on the right.


# Application Containers

These key bindings are for applications that wrap other applications.
A terminal multiplexer is an example of such a wraper.
All these key bindings are prefixed with `apps-prefix`.
`apps-prefix` is `shift+super`.

In Zellij I have configured the followning in `~/.config/zellij/config.kdl`.
- `h`: Focus panel to left.
- `j`: Focus panel down.
- `k`: Focus panel up.
- `l`: Focus panel right.