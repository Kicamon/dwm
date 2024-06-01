#! /bin/bash
#set -x

Path=~/.config/wallpaper/
files=($(ls ${Path} | grep -E 'png|jpg'))
len=${#files[*]}
index=`head --lines=1 ~/.config/wallpaper/index.txt`

if [ $1 == 'start' ]
then
  index=0
  feh --bg-fill ${Path}${files[$index]}
elif [ $1 == 'prev' ]
then
  index=`expr ${index} - 1`
  if [ ${index} -lt 0 ]
  then
    index=`expr ${len} - 1`
  fi
  feh --bg-fill ${Path}${files[$index]}
elif [ $1 == 'next' ]
then
  index=`expr ${index} + 1`
  if [ ${index} -ge ${len} ]
  then
    index=0
  fi
  feh --bg-fill ${Path}${files[${index}]}
elif [ $1 == 'rechange' ]
then
  RCHANGE=$(ps -ef | grep rechange_wallpaper.sh | grep -v grep)
  if [ "$RCHANGE" == "" ]
  then
    ~/.config/dwm/scripts/rechange_wallpaper.sh
  else
    killall rechange_wallpaper.sh
  fi
fi

echo $index > ~/.config/wallpaper/index.txt

