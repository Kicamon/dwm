#!/bin/bash

lock_dir="$HOME/.local/share/lock"
wallpaper="$lock_dir/bg.png"
userpic="$lock_dir/pp.png"
final_bg="$lock_dir/final-bg.png"

fullname=$(getent passwd $USER | cut -d ":" -f 5 | cut -d "," -f 1)
full_alias="${fullname} (${USER})"
if [[ -n $fullname ]]; then
	full_alias="${fullname} (${USER})"
else
	full_alias=$USER
fi

width=$(xrandr --query | grep ' connected' | grep -o '[0-9][0-9]*x[0-9][0-9]*[^ ]*' | sed -r 's/^[^0-9]*([0-9]+x[0-9]+).*$/\1/' | cut -d "x" -f 1 | head -n1)
height=$(xrandr --query | grep ' connected' | grep -o '[0-9][0-9]*x[0-9][0-9]*[^ ]*' | sed -r 's/^[^0-9]*([0-9]+x[0-9]+).*$/\1/' | cut -d "x" -f 2 | head -n1)

fg_color=fefefeff
wrong_color=f82a11aa
highlight_color=39393999
verif_color=fefefe66

process_images() {
	local cropuser="$lock_dir/user-crop.png"
	convert "$userpic" -resize 100x100 -gravity Center \( \
		-size 100x100 xc:Black \
		-fill White \
		-draw "circle 50 50 50 1" \
		-alpha Copy \) -compose CopyOpacity -composite -trim "$cropuser"

	local bg_blur="$lock_dir/bg-blur.png"
	convert "$wallpaper" -resize ${width}x -gravity center -crop ${width}x${height}+0+0 +repage \
		-filter Gaussian -blur 0x8 "$bg_blur"

	local final_bg="$lock_dir/final-bg.png"
	convert "$bg_blur" \
		\( "$cropuser" -resize 120x140 \) \
		-gravity center -geometry +0-100 -compose over -composite "$final_bg"
}

if [[ ! -d ${lock_dir} ]]; then
  mkdir -p ${lock_dir}
  echo "将头像(pp.png)和背景图片(bg.png)存入到${lock_dir}中"
  return
fi

if [[ ! -f ${final_bg} ]]; then
  process_images
fi

i3lock -n --force-clock -i "$final_bg" \
	--ind-pos="w/2:h/2+40" \
	--greeter-pos="w/2:h/2+120" \
	--time-pos="w-100:h-70" \
	--date-pos="w-115:h-40" \
	--insidever-color=$fg_color --insidewrong-color=$wrong_color --inside-color=fefefe00 \
	--ringver-color=$verif_color --ringwrong-color=$wrong_color --ring-color=$fg_color \
	--keyhl-color=$highlight_color --bshl-color=$highlight_color --separator-color=00000000 \
	--date-color=$fg_color --time-color=$fg_color --greeter-color=$fg_color \
	--time-str="%H:%M" --time-size=50 \
	--date-str="%a, %b %d" --date-size=30 \
	--greeter-text="$full_alias" --greeter-size=25 --line-uses-ring \
	--radius 38 --ring-width 3 --indicator \
	--verif-text="" --wrong-text="" --noinput-text="" \
	--clock --date-font="SF Pro Display" --time-font="SF Pro Display"
