#! /bin/bash

_thisdir=~/.config/dwm

settings() {
  xset -b
  xbacklight -set 100
}

daemons() {
  sleep 0.5
  xrandr --output HDMI-0 --primary --auto --above DP-4
  $_thisdir/statusbar/statusbar.sh cron &
  $_thisdir/scripts/wallpaper.sh start
  picom --experimental-backends &
  fcitx5 &
  flameshot &
  dunst &
}

setting &
daemons &
