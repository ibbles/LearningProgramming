#!/usr/bin/env fish

set source $argv[1]
set dark_mode $argv[2]

if test -z "$source"
	echo "No file given."
	exit 1
end

if test ! -f "$source"
	echo "File '$source' does not exist."
	exit 1
end

set name (basename "$source" ".dot")
set target "$name".svg
dot -Tsvg "$source" -o "$target"

if test "$dark_mode" = "dark-mode"
	sed -i 's/<polygon fill="white"/<polygon fill-opacity="0" fill="white"/g' "$target"
	sed -i 's/stroke="black"/stroke="white"/g' "$target"
	sed -i 's/<text /<text fill="white" /g' "$target"
end
