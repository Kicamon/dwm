#! /bin/bash

Path=~/.config/wallpaper/
files=($(ls ${Path} | grep -E 'png|jpg'))
len=${#files[*]}
index=($(cat ${Path}index.txt))
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
  echo -e "$str" > ${Path}/index.txt
  sleep 300
done

