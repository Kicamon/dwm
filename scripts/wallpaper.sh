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

if [ $2 == 'start' ]
then
  for ((i=0;i<${connected_screens};i++));do
    index[${i}]=0
  done
  feh --bg-fill ${Path}${files[0]}
elif [ $2 == 'prev' ]
then
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
elif [ $2 == 'next' ]
then
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
elif [ $2 == 'rechange' ]
then
  RCHANGE=$(ps -ef | grep rechange_wallpaper.sh | grep -v grep)
  if [ "$RCHANGE" == "" ]
  then
    ~/.config/dwm/scripts/rechange_wallpaper.sh &
  else
    killall rechange_wallpaper.sh
  fi
fi

str=""
for idx in "${index[@]}";do
  str+="${idx}\n"
done
echo -e "$str" > ${Path_tmp}
