# Used for the phpStorm program
export _JAVA_AWT_WM_NONREPARENTING=1
export AWT_TOOLKIT=MToolkit
wmname LG3D
# export DISPLAY=:0

# set baground
feh --bg-fill ~/git/vrkansagara/dwm/images

# Laptop/Desktop specific start up
~/.vim/src/Sh/MacBookAir/FunctionKey.sh
~/.vim/src/Sh/System/battery.sh &

# start program(s)

# st , dwm transperent
/usr/bin/cputool -c 5 -- /usr/bin/compton &

 /usr/bin/cputool -c 5 -- /usr/local/bin/st -e alsamixer &
 /usr/bin/cputool -c 5 -- /usr/local/bin/st &
 /usr/bin/cputool -c 5 -- /usr/local/bin/st -e htop -u $USER -d 120 &
#  /usr/bin/cputool -c 5 -- guake &
 /usr/bin/cputool -c 5 -- ~/Hubstaff/HubstaffClient.bin.x86_64 &

/usr/bin/cputool -c 20 -- /usr/local/bin/st -c vrkansagara-ide -n vrkansagara-ide -e vim $HOME/www &
/usr/bin/cputool -c 20 -- thunar &
/usr/bin/cputool -c 20 -- keepassxc &
/usr/bin/cputool -c 20 -- ringcentral &
/usr/bin/cputool -c 20 -- remmina &
/usr/bin/cputool -c 20 -- anydesk &

/usr/bin/cputool -c 40 -- skype &
/usr/bin/cputool -c 40 -- teams &
/usr/bin/cputool -c 40 -- ~/Applications/Postman/Postman &

# /usr/bin/cputool -c 60 ~/Applications/phpStorm/bin/phpstorm.sh &

/usr/bin/cputool -c 200 -- firefox &
# /usr/bin/cputool -c 200 -- google-chrome &

# DWM top bar using conky
# (sh ~/git/vrkansagara/dwm/conky/conky.sh | while read LINE; do xsetroot -name "$LINE"; done) &
while true; do
	(conky | while read LINE; do xsetroot -name "$LINE"; done)
sleep 1
done&

#evaluate the SSH agent
export GPGKEY=8BA6E7ABD8112B3E
killall gpg-agent
eval $(gpg-agent)

# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=747465
/usr/lib/notification-daemon/notification-daemon &

# X window lock start
xautolock -time 1 -locker slock &

# Start Window Manager (Alwayse last line)
# Log stderror to a file
# Insted of quiting dwm , make it hot reload(Will not disturbe your stream while hot reload)
 while :; do
	/usr/local/bin/dwm 2> /tmp/dwm.log
done
