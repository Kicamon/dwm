#! /bin/bash

screen_names=($(xrandr --listmonitors | grep -oE '[^ ]+$' | tail -n +2))
tempfile=$DWM/statusbar/temp

backlight=$(xrandr --verbose | grep Brightnes | awk '{print $2}' | head -n 1)
backlight=$(awk "BEGIN {print $backlight * 100}")

case $1 in
  up) backlight=$((${backlight} + 10));;
  down) backlight=$((${backlight} - 10));;
esac

if [ $backlight -gt 100 ]
then
  backlight=100
elif [ $backlight -lt 10 ]
then
  backlight=10;
fi
backlight=$(awk "BEGIN {printf \"%.2f\", ${backlight} / 100}")

str=""
for idx in ${screen_names[@]};do
  str+=$(printf -- "--output %s --brightness %.2f " "$idx" "$backlight")
done

xrandr $str

touch $tempfile
