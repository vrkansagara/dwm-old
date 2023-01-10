#!/usr/bin/env bash
set -eou pipefail

export DEBIAN_FRONTEND=noninteractive
CURRENT_DATE=$(date "+%Y%m%d%H%M%S")
SCRIPT=$(readlink -f "$0")
SCRIPTDIR=$(dirname "$SCRIPT")

if [ "$(whoami)" != "root" ]; then
  SUDO=sudo
fi

# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#  Maintainer :- Vallabh Kansagara<vrkansagara@gmail.com> — @vrkansagara
#  Note       :- DWM Window manager
# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

#${SUDO} sudo apt-get install --yes -q --no-install-recommends \
#  patch build-essential libpoppler-cpp-dev pkg-config python3-dev \
#  libx11-dev x11proto-core-dev libxft-dev libharfbuzz-dev libxinerama-dev libxinerama1 libio-socket-ssl-perl libcpanel-json-xs-perl libjson-xs-perl libxml-dumper-perl xdotool \
#  feh conky cpulimit cputool screenkey htop zsh tree vlc git nmap elinks vim gimp keepassxc arandr suckless-tools xautolock ranger \
#  mesa-utils pulseaudio thunar alsa-utils network-manager xhk wmctrl pavucontrol iputils-ping net-tools lsof whois \
#  inxi lshw hddtemp net-tools ipmitool freeipmi-tools ipvsadm lvm2 mdadm lm-sensors smartmontools notification-daemon notify-osd libnotify-dev libnotify-bin hardinfo \
#  linux-headers-$(uname -r) linux-image-$(uname -r) lsb-release \
#  linux-image-$(uname -r | sed 's,[^-]*-[^-]*-,,') linux-headers-$(uname -r | sed 's,[^-]*-[^-]*-,,') at-spi2-core firmware-linux-nonfree \
#  broadcom-sta-dkms hardinfo vlc cpulimit at-spi2-core

# pulseaudio --start --log-target=syslogk

# scrot = screen capture tool
# compton = dwm/st tranparency with x11
# xdotool = programmetly call keybindings
# inxi = forensic tool for hardware information
# xautolock = monitor binary for x time

# slock = suckless login manger
# suckless-tools = suckless built in library

# Check if compositor is running or not
# inxi -Gxx | grep compositor

# xautolock -time 1 -locker slock
# ${SUDO} cp -R hooks .git/

${SUDO} apt-get install --yes --no-install-recommends \
  xcb libxcb-xkb-dev \
  x11-xkb-utils libx11-xcb-dev \
  libxkbcommon-x11-dev libxcb-res0-dev

# Ranger dependencies
#${SUDO} pip install pdftotext

FILES="patches/*.diff"
for f in $FILES; do
  if [ -f "$f" ]; then
    echo "Applying path for the [ $f ]"
    dos2unix $f
    patch --merge=diff3 -i $f
    sleep 1
  fi
done

# Give current user permission to work with source
${SUDO} chown $USER -Rf .
${SUDO} chgrp $USER -Rf .

make clean
make
${SUDO} rm -rf /usr/share/bin/dwm
${SUDO} make install
${SUDO} make clean

${SUDO} rm -rf /usr/share/xsessions/vallabh.desktop
${SUDO} ln -s $(pwd)/dwm.desktop /usr/share/xsessions/vallabh.desktop
${SUDO} rm -rf $HOME/.xinitrc $HOME/.xprofile
${SUDO} ln -s $(pwd)/x11/xinitrc $HOME/.xinitrc
${SUDO} ln -s $(pwd)/x11/xprofile $HOME/.xprofile
${SUDO} chmod 744 $HOME/.xinitrc
${SUDO} chmod u+s /usr/bin/xinit

# Copy conky configuration to home folder
${SUDO} rm -rf $HOME/.config/conky
${SUDO} cp -R $(pwd)/conky $HOME/.config

${SUDO} rm -rf $HOME/.config/ranger
${SUDO} ln -s $HOME/git/vrkansagara/dwm/ranger $HOME/.config/

${SUDO} chown $USER -Rf $HOME/.config
${SUDO} chgrp $USER -Rf $HOME/.config

${SUDO} chsh -s $(which zsh) $USER

# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=747465
# echo '[D-BUS Service]
# Name=org.freedesktop.Notifications
# Exec=/usr/lib/notification-daemon/notification-daemon'| ${SUDO} tee /usr/share/dbus-1/services/org.gnome.Notifications.service > /dev/null

# Command line fuzzy finder called fzf
if [ ! -d "$HOME/.fzf" ]; then
  cd $HOME
  git clone https://github.com/junegunn/fzf.git --depth=1 -b master .fzf
  cd .fzf
  ${SUDO} git stash
  git reset --hard HEAD
  git clean -fd
fi

echo "Your simple window manager is configured and ready to use.........[DONE]."

exit 0