#! /bin/bash

tempfile=$DWM/statusbar/temp
_thisdir=$DWM

case $1 in
  up) pactl set-sink-volume @DEFAULT_SINK@ +5% ;;
  down) pactl set-sink-volume @DEFAULT_SINK@ -5% ;;
esac

touch $tempfile
bash $_thisdir/statusbar/packages/vol.sh notify
