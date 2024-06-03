#! /bin/bash

Path=~/.config/wallpaper/
files=($(ls ${Path} | grep -E 'png|jpg'))
len=${#files[*]}
index1=`sed -n "1,1p" ~/.config/wallpaper/index.txt`
index2=`sed -n "2,2p" ~/.config/wallpaper/index.txt`

while true
do
  index1=`expr ${RANDOM} % ${len}`
  index2=`expr ${RANDOM} % ${len}`
  feh --bg-fill ${Path}${files[$index1]} ${Path}${files[$index2]}
  echo "$index1"$'\n'"$index2" > ~/.config/wallpaper/index.txt
  sleep 300
done

