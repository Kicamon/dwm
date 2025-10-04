#!/bin/bash

default_screen="DP-4"
output_screen="HDMI-0"

auto() {
	have_output=$(xrandr | grep ${output_screen} | awk -F ' ' '{print $2}')
	if [[ $have_output == 'connected' ]]; then
		xrandr --output ${default_screen} --off --output ${output_screen} --auto
	fi
}

case "$1" in
same) xrandr --output ${default_screen} --same-as --output ${output_screen} --auto ;;
up) xrandr --output ${output_screen} --primary --auto --above ${default_screen} ;;
*) auto ;;
esac
