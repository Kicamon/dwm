#! /bin/bash

dwmdir=~/.config/dwm

case $1 in
  up) pactl set-sink-volume @DEFAULT_SINK@ +10% ;;
  down) pactl set-sink-volume @DEFAULT_SINK@ -10% ;;
esac

bash $dwmdir/statusbar/statusbar.sh update vol
