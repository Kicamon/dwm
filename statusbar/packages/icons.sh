#! /bin/bash
# ICONS 部分特殊的标记图标 这里是我自己用的，你用不上的话去掉就行

tempfile=$(cd $(dirname $0);cd ..;pwd)/temp

this=_icons
color="^c#2D1B46^^b#5555660x66^"
signal=$(echo "^s$this^" | sed 's/_//')

with_v2raya() {
  [ "$(ps aux | grep -v grep | grep 'v2raya')" ] && icons=(${icons[@]} "")
}

with_bluetooth() {
  # 此处为自用蓝牙设备的 MAC 地址，你可以自定义该部分
  [ ! "$(command -v bluetoothctl)" ] && echo command not found: bluetoothctl && return
  [ "$(bluetoothctl info 6C:D3:EE:3E:CB:5F | grep 'Connected: yes')" ] && icons=(${icons[@]} "")
}

update() {
  icons=("󰍜")
  with_v2raya
  with_bluetooth

  text=" ${icons[@]} "

  sed -i '/^export '$this'=.*$/d' $tempfile
  printf "export %s='%s%s%s'\n" $this "$signal" "$color" "$text" >> $tempfile
}

notify() {
  texts=""
  [ "$(ps aux | grep -v grep | grep 'v2raya')" ] && texts="$texts\n v2raya 已启动"
  [ "$(bluetoothctl info 6C:D3:EE:3E:CB:5F | grep 'Connected: yes')" ] && texts="$texts\n 蓝牙耳机 已链接"
  [ "$texts" != "" ] && notify-send " Info" "$texts" -r 9527
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
  *) update ;;
esac
