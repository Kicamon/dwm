#! /bin/bash

Path=~/.config/wallpaper/
Path_tmp=~/.local/state/dwm/wallpaper
files=($(ls ${Path} | grep -E 'png|jpg'))
len=${#files[*]}
index=($(cat ${Path_tmp}))
connected_screens=$(xrandr --listmonitors | grep 'Monitors:' | awk '{print $2}')

while true
do
  for ((i=0;i<${connected_screens};i++));do
    index[${i}]=$((RANDOM % len))
  done
  str=""
  for idx in "${index[@]}";do
    str+="${Path}${files[${idx}]} "
  done
  feh --bg-fill ${str}
  str=""
  for idx in "${index[@]}";do
    str+="${idx}\n"
  done
  echo -e "$str" > ${Path_tmp}
  sleep 300
done

