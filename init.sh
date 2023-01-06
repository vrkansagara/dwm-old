# set -e # This setting is telling the script to exit on a command error.
# set -x # You refer to a noisy script.(Used to debugging)

echo " "
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

# ${SUDO} sudo apt-get install --yes -q  --no-install-recommends patch build-essential libx11-dev feh conky cputool pulseaudio libxft-dev thunar x11proto-core-dev libharfbuzz-dev libxinerama-dev libxinerama1 xhk screenkey htop alsa-utils network-manager zsh firefox-esr lshw linux-headers-$(uname -r) linux-image-$(uname -r) linux-headers-generic inxi mesa-utils hddtemp net-tools ipmitool freeipmi-tools ipvsadm lvm2 mdadm lm-sensors smartmontools tree lsb-release  libio-socket-ssl-perl libcpanel-json-xs-perl  libjson-xs-perl  libxml-dumper-perl wmctrl linux-image-$(uname -r|sed 's,[^-]*-[^-]*-,,') linux-headers-$(uname -r|sed 's,[^-]*-[^-]*-,,') broadcom-sta-dkms hardinfo vlc cheese cpulimit notification-daemon libnotify-bin notify-osd libnotify-dev at-spi2-core
# ${SUDO} apt install --yes --no-install-recommends xorg git nmap elinks htop nmap vim x11proto-core-dev libxft-dev libharfbuzz-dev xhk libx11-dev libxinerama-dev libxinerama1 thunar feh conky cputool conky gimp sudo keepassxc thunar alsa-utils alsa-tools firmware-linux-nonfree arandr pavucontrol iputils-ping net-tools lsof nmap whois network-manager

${SUDO} apt install --yes --no-install-recommends libxft-dev libxinerama-dev
# pulseaudio --start --log-target=syslogk

# scrot = screen capture tool
# compton = dwm/st tranparency with x11
# xdotool = programmetly call keybindings
# inxi = forensic tool for hardware information
# xautolock = monitor binary for x time

# slock = suckless login manger
${SUDO} apt-get install --yes --no-install-recommends \
    xautolock xdotool compton inxi scrot zsh conky feh screenkey

# Check if compositor is running or not
# inxi -Gxx | grep compositor

# xautolock -time 1 -locker slock
# ${SUDO} cp -R hooks .git/

${SUDO} apt-get install --yes --no-install-recommends xcb libxcb-xkb-dev \
    x11-xkb-utils libx11-xcb-dev \
    libxkbcommon-x11-dev libxcb-res0-dev \
    ranger

# Ranger dependencies
${SUDO} apt install build-essential libpoppler-cpp-dev pkg-config python3-dev
${SUDO} pip install pdftotext

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
${SUDO} chown $USER  -Rf .
${SUDO} chgrp $USER  -Rf .

make clean
make
${SUDO} mv  /usr/share/bin/dwm /tmp
${SUDO} make install
${SUDO} make clean

${SUDO} mv  /usr/share/xsessions/vallabh.desktop /tmp
${SUDO} ln -s $(pwd)/dwm.desktop /usr/share/xsessions/vallabh.desktop
${SUDO} ln -s $HOME/git/vrkansagara/dwm/ranger  $HOME/.config/
${SUDO} mv  $HOME/.xinitrc $HOME/.xprofile /tmp
${SUDO} ln -s $(pwd)/x11/xinitrc $HOME/.xinitrc
${SUDO} ln -s $(pwd)/x11/xprofile $HOME/.xprofile
${SUDO} chmod 744 $HOME/.xinitrc
${SUDO} chmod u+s /usr/bin/xinit

# Copy conky configuration to home folder
${SUDO} rm -rf $HOME/.config/conky
${SUDO} cp -R $(pwd)/conky $HOME/.config

${SUDO} rm -rf $HOME/.config/ranger /tmp
${SUDO} cp -R $(pwd)/ranger $HOME/.config

${SUDO} chown $USER  -Rf $HOME/.config
${SUDO} chgrp $USER  -Rf $HOME/.config

${SUDO} chsh -s $(which zsh) $USER

# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=747465
# echo '[D-BUS Service]
# Name=org.freedesktop.Notifications
# Exec=/usr/lib/notification-daemon/notification-daemon'| ${SUDO} tee /usr/share/dbus-1/services/org.gnome.Notifications.service > /dev/null

# Command line fuzzy finder called fzf
if [ ! -d "$HOME/.fzf" ]; then
    cd /tmp
    git clone https://github.com/junegunn/fzf.git --depth=1 -b master
    cd fzf
    git stash
    git reset --hard HEAD
    git clean -fd
fi

echo "Your simple window manager is configured and ready to use.........[DONE]."

exit 0
