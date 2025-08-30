#!/usr/bin/env bash

tempfile=$DWM/statusbar/temp

microphone() {
	amixer set Capture toggle
  amixer set Capture 20%
  touch $tempfile
}

powermenu() {
	~/.config/rofi/powermenu.sh
}

click() {
	case "$1" in
	L) microphone ;;
	R) powermenu ;;
	esac
}

case "$1" in
click) click $2 ;;
notify) notify ;;
esac
