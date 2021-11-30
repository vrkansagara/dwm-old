# xinitrc runs automatically when you run startx.

# There are some small but important commands that need to be run when we start
# the graphical environment. There is a link to this file in ~/.xprofile
# because that file is run automatically if someone uses a display manager
# (login screen) and so they are needed there. To prevent doubling up commands,
# I source them here with the line below.

if [ -f "${XDG_CONFIG_HOME:-$HOME/.config}/x11/xprofile" ]; then
	. "${XDG_CONFIG_HOME:-$HOME/.config}/x11/xprofile"
else
	. "$HOME/.xprofile"
fi

# invoke global X session script
# if [ -d /etc/X11/xinit/xinitrc.d ]; then
  # for f in /etc/X11/xinit/xinitrc.d/*; do
    # [ -x "$f" ] && . "$f"
  # done
  # unset f
# fi

# set baground
WALLPAPERS="$HOME/git/vrkansagara/dwm/images"
while true; do
  find ${WALLPAPERS} -type f -iname "*.jpg" | shuf -n 1 | xargs feh --bg-fill
sleep  15m
done&

# Laptop/Desktop specific start up
~/.vim/src/Sh/MacBookAir/FunctionKey.sh
~/.vim/src/Sh/System/battery.sh &

# start program(s)

# st , dwm transperent
/usr/bin/compton &
# ~/Hubstaff/HubstaffClient.bin.x86_64 &
/usr/local/bin/st &
~/Applications/workcomposer-linux-installer.AppImage &
/usr/local/bin/st -c vrkansagara-ide -n vrkansagara-ide -e vim $HOME/www &
/usr/local/bin/st -c firle-explorer -n firle-explorer -e ranger $HOME/www &
keepassxc &
ringcentral &
remmina &
anydesk &
skype &
teams &
~/Applications/Postman/Postman &
/usr/bin/firefox &

# DWM top bar using conky
# (sh ~/git/vrkansagara/dwm/conky/conky.sh | while read LINE; do xsetroot -name "$LINE"; done) &
while true; do
	(conky | while read LINE; do xsetroot -name "$LINE"; done)
	sleep 1
done&

#evaluate the SSH agent
export GPGKEY=8BA6E7ABD8112B3E
killall gpg-agent ssh-agent
eval $(ssh-agent)
eval $(gpg-agent)

# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=747465
/usr/lib/notification-daemon/notification-daemon &

# X window lock start ( compile slock if not)
xautolock -time 2 -locker slock &

# zsh/demnu clean cache
mkdir -p $HOME/old
mv .bash* .profile $HOME/old
mv $HOME/.cache/dmenu_run $HOME/old
mv .zcompdump-* $HOME/old

# Start Window Manager (Alwayse last line)
# Log stderror to a file
# Insted of quiting dwm , make it hot reload(Will not disturbe your stream while hot reload)
while true; do
	/usr/local/bin/dwm 2>> /tmp/dwm.log
done
