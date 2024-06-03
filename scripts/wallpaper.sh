#! /bin/bash
#set -x

Path=~/.config/wallpaper/
files=($(ls ${Path} | grep -E 'png|jpg'))
len=${#files[*]}
index1=`sed -n "1,1p" ~/.config/wallpaper/index.txt`
index2=`sed -n "2,2p" ~/.config/wallpaper/index.txt`

change_index() {
  index1=$(((index1 + $1) % len))
  index2=$(((index1 + $2) % len))
}

if [ $2 == 'start' ]
then
  index1=0
  index2=0
  feh --bg-fill ${Path}${files[$index1]} ${Path}${files[$index2]}
elif [ $2 == 'prev' ]
then
  if [ $1 == 0 ]
  then
    change_index -1 0
  else
    change_index 0 -1
  fi
  feh --bg-fill ${Path}${files[$index1]} ${Path}${files[$index2]}
elif [ $2 == 'next' ]
then
  if [ $1 == 0 ]
  then
    change_index 1 0
  else
    change_index 0 1
  fi
  feh --bg-fill ${Path}${files[$index1]} ${Path}${files[$index2]}
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

echo "$index1"$'\n'"$index2" > ~/.config/wallpaper/index.txt
