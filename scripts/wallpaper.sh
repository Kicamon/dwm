#!/bin/bash
#set -x

Path=~/.config/wallpaper/
Path_tmp=~/.local/state/dwm/wallpaper
files=($(ls ${Path} | grep -E 'png|jpg'))
len=${#files[*]}
index=($(cat ${Path_tmp}))
connected_screens=$(xrandr --listmonitors | grep 'Monitors:' | awk '{print $2}')

change_index() {
  for ((i=1;i<=${connected_screens};i++));do
    eval "idx=\$$i"
    index[$((i - 1))]=$(((index[i - 1] + idx + len) % len))
  done
  str=""
  for idx in "${index[@]}";do
    str+="${Path}${files[${idx}]} "
  done
  feh --bg-fill ${str}
}

write() {
  str=""
  for idx in "${index[@]}";do
    str+="${idx}\n"
  done
  echo -e "$str" > ${Path_tmp}
}

start() {
  for ((i=0;i<${connected_screens};i++));do
    index[${i}]=0
  done
  feh --bg-fill ${Path}${files[0]}
  write
}

prev() {
  str=""
  for ((i=0;i<${connected_screens};i++));do
    if [ ${i} == ${1} ]
    then
      str+="-1 "
    else
      str+="0 "
    fi

  done
  change_index $str
  write
}

next() {
  str=""
  for ((i=0;i<${connected_screens};i++));do
    if [ ${i} == ${1} ];
    then
      str+="1 "
    else
      str+="0 "
    fi

  done
  change_index $str
  write
}

rechange() {
  RCHANGE=$(ps -ef | grep rechange_wallpaper.sh | grep -v grep)
  if [ "$RCHANGE" == "" ]
  then
    $DWM/scripts/rechange_wallpaper.sh &
  else
    killall rechange_wallpaper.sh
  fi
}

case "$2" in
  start) start ;;
  prev) prev $1 ;;
  next) next $1 ;;
  rechange) rechange ;;
esac
