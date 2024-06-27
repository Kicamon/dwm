#! /bin/bash

wp_change() {
  killall rechange_wallpaper.sh
  ~/.config/dwm/scripts/rechange_wallpaper.sh &
}

get_screen() {
  img_path=$(flameshot screen -p ~/Pictures 2>&1  | grep -oP '(?<=Capture saved as ).*')
  xclip -selection clipboard -t image/png -i "$image_path"
  ristretto $img_path
}

click() {
  case "$1" in
    L) wp_change ;;
    R) get_screen ;;
  esac
}

case "$1" in
  click) click $2 ;;
  notify) notify ;;
esac
