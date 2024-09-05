#! /bin/bash

_thisdir=$DWM

xset -b

sleep 0.5
xrandr --output HDMI-0 --primary --auto --above DP-4
dwm_statusbar cron &
$_thisdir/scripts/wallpaper.sh 0 start &
picom --experimental-backends &
xfce4-power-manager &
dunst &
fcitx5 &
Snipaste &

sleep 8
otd-daemon &
clash-verge &
python3 $_thisdir/scripts/network.py
