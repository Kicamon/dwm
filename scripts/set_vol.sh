#! /bin/bash

tempfile=~/.config/dwm/statusbar/temp

case $1 in
  up) pactl set-sink-volume @DEFAULT_SINK@ +10% ;;
  down) pactl set-sink-volume @DEFAULT_SINK@ -10% ;;
esac

touch $tempfile
