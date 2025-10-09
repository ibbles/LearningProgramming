#!/usr/bin/env fish

set file $argv[1]
if test -z "$file"
	echo "No file given."
	exit 1
end

if test ! -f "$file"
	echo "File '$file' does not exist."
	exit 1
end

set name (basename "$file" ".dot")

set target_directory "../working_copy/images/taskflow/"
if test ! -d "$target_directory"
	echo "Could not find target directory '$target_directory"
	exit 1
end

dot -Tjpeg "$name".dot -o "$target_directory/$name".jpg
