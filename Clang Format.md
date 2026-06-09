# Rider

Settings > Tools > External Tools > `+` button:
- Program: File system to your `clang-format` binary.
- Arguments: `-i -style=file $FilePath$`
- Working directory: `$PRojectFileDir$
- Synchronize file after execution: `On`
- Open console for tool output: `Off`

Close and re-open Settings to synchronize from External Tools to Keymap.

Settings > Keymap:
- Externat Tools > External Tools > Clang Format: `Alt Shift F`


