#! /bin/bash
# ICONS 部分特殊的标记图标 这里是我自己用的，你用不上的话去掉就行

with_v2raya() {
  [ "$(ps aux | grep -v grep | grep 'v2raya')" ] && icons=(${icons[@]} "")
}

with_hot() {
  [ "$(nmcli dev | grep 'ap0')" ] && icons=(${icons[@]} "󱜠")
}

update() {
  icons=("󰊠")
  with_v2raya
  with_hot

  text=" ${icons[@]} "
}

notify() {
  texts=""
  [ "$(ps aux | grep -v grep | grep 'v2raya')" ] && texts="$texts\n v2raya 已启动"
  [ "$(nmcli dev | grep 'ap0')" ] && texts="$texts\n󱜠 热点 已开启"
  [ "$texts" != "" ] && notify-send " Info" "$texts" -r 9527
}

call_menu() {
  ~/.config/rofi/powermenu.sh
}

click() {
  case "$1" in
    L) notify ;;
    R) call_menu ;;
  esac
}

case "$1" in
  click) click $2 ;;
  notify) notify ;;
esac
