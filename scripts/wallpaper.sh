#! /bin/bash
#set -x

Path=~/.config/wallpaper/
files=($(ls ${Path} | grep -E 'png|jpg'))
len=${#files[*]}
index=($(cat ${Path}index.txt))
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
  for ((i=1;i<=${connected_screens};i++));do
    index[$((i - 1))]=0
  done
  feh --bg-fill ${Path}${files[0]}
elif [ $2 == 'prev' ]
then
  if [ $1 == 0 ]
  then
    change_index -1 0
  else
    change_index 0 -1
  fi
elif [ $2 == 'next' ]
then
  if [ $1 == 0 ]
  then
    change_index 1 0
  else
    change_index 0 1
  fi
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
echo -e "$str" > ${Path}/index.txt
