#! /bin/bash

tempfile=~/.config/dwm/statusbar/temp
_thisdir=~/.config/dwm

case $1 in
  up) pactl set-sink-volume @DEFAULT_SINK@ +5% ;;
  down) pactl set-sink-volume @DEFAULT_SINK@ -5% ;;
esac

touch $tempfile
bash $_thisdir/statusbar/packages/vol.sh notify
