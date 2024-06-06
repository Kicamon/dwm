#! /bin/bash
# backlight 脚本

tempfile=$(cd $(dirname $0);cd ..;pwd)/temp

this=_light
icon_color="^c#3B102B^^b#6873790x88^"
text_color="^c#3B102B^^b#6873790x99^"
signal=$(echo "^s$this^" | sed 's/_//')

update() {
  light_text=$(xrandr --verbose | grep Brightnes | awk '{print $2}' | head -n 1)
  light_text=$(awk "BEGIN {print $light_text * 100}")
  if   [ "$light_text" -ge 95 ]; then light_icon="󰃠";
  elif [ "$light_text" -ge 75 ]; then light_icon="󰃝";
  elif [ "$light_text" -ge 50 ]; then light_icon="󰃟";
  elif [ "$light_text" -ge 25 ]; then light_icon="󰃞";
  else light_icon="󰃜"; fi
  icon=" $light_icon "
  text=" $light_text% "

  sed -i '/^export '$this'=.*$/d' $tempfile
  printf "export %s='%s%s%s%s%s'\n" $this "$signal" "$icon_color" "$icon" "$text_color" "$text" >> $tempfile
}

case "$1" in
  *) update ;;
esac
