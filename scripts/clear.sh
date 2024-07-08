#!/bin/bash

current_time=$(date +%s)
time_interval=$((7*24*60*60))

delete_old_files() {
  local dir="$1"
  for file in "$dir"/{*,.*}; do
    if [[ "$file" == "$dir/." || "$file" == "$dir/.." ]]; then
      continue
    fi

    if [ -e "$file" ]; then
      file_mod_time=$(stat -c %Y "$file")
      file_age=$((current_time - file_mod_time))
      if [ "$file_age" -gt "$time_interval" ]; then
        rm -rf "$file"
        echo "Deleted: $file"
      fi
    fi
  done
}

delete_old_files "~/.local/share/Trash/info"
delete_old_files "~/.local/share/Trash/files"
