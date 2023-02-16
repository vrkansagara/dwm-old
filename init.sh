#!/usr/bin/env bash
# set -eou pipefail
#set -e # This setting is telling the script to exit on a command error.
if [[ "$1" == "-v" ]]; then
  set -x # You refer to a noisy script.(Used to debugging)
fi

export DEBIAN_FRONTEND=noninteractive
CURRENT_DATE=$(date "+%Y%m%d%H%M%S")
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
DWM_DIR="$SCRIPT_DIR/vendor/dwm"
DWMBLOCKS_DIR="$SCRIPT_DIR/vendor/dwmblocks"
ST_DIR="$SCRIPT_DIR/vendor/st"
SLOCK_DIR="$SCRIPT_DIR/vendor/slock"
SCROLL_DIR="$SCRIPT_DIR/vendor/scroll"

if [ "$(whoami)" != "root" ]; then
  SUDO=sudo
fi

GREEN=$'\e[0;32m'
RED=$'\e[0;31m'
NC=$'\e[0m'
echo "$GREEN Script running in this directory [$SCRIPT_DIR]  $NC"

apply_permission() {
  echo "$RED Current directory is [$(pwd)]  $NC"
  # Give current user permission to work with source
  ${SUDO} chown $USER -Rf .
  ${SUDO} chgrp $USER -Rf .
}
apply_patche(){
  FILES="$(pwd)/patches/*.diff"
  for f in $FILES; do
      if [ -f "$f" ]; then
          dos2unix $f
          echo "$RED Applying path for the [ $f ] $NC"
          patch --merge=diff3 -i $f
          retCode=$$?; \
          [[ $$retCode -gt 1 ]] && exit $$retCode; \
          sleep 1
      fi
  done
}
apply_git_clean(){
  ${SUDO} git reset --hard HEAD
  ${SUDO} git clean -fd
  apply_permission
  git checkout master
}
# I would make sure , I am in to current directory
cd $SCRIPT_DIR
apply_permission
# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
#  Maintainer :- Vallabh Kansagara<vrkansagara@gmail.com> — @vrkansagara
#  Note       :- DWM Window manager
# """""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

#git submodule update --init --recursive --jobs 4  --remote --rebase

## DWMBlock Specific
#cd $ST_DIR
#apply_git_clean
#cp -R $SCRIPT_DIR/st/* $ST_DIR
#apply_patche
#${SUDO} make clean
#${SUDO} make
#${SUDO} make uninstall
#${SUDO} make install

# DWM Specific
cd $DWM_DIR
apply_git_clean
cp -R $SCRIPT_DIR/dwm/* $DWM_DIR
apply_patche
${SUDO} make clean
${SUDO} make
${SUDO} make uninstall
${SUDO} make install

# DWMBlock Specific
cd $DWMBLOCKS_DIR
apply_git_clean
cp -R $SCRIPT_DIR/dwmblocks/* $DWMBLOCKS_DIR
${SUDO} make clean
${SUDO} make
${SUDO} make uninstall
${SUDO} make install
#Lets kill all process which is executed for the dwmblocks
ps -ef | grep "dwmblocks" | grep -v grep | awk "{print \$2}" | xargs --no-run-if-empty sudo kill 9
# reset statusbar
xsetroot -name ""
/usr/local/bin/dwmblocks&

## SLOCK Specific
#cd $SLOCK_DIR
#apply_git_clean
#cp -R $SCRIPT_DIR/slock/* $SLOCK_DIR
#${SUDO} make clean
#${SUDO} make
#${SUDO} make uninstall
#${SUDO} make install

## SCROLL_DIR Specific
#cd $SCROLL_DIR
#apply_git_clean
#cp -R $SCRIPT_DIR/slock/* $SCROLL_DIR
#${SUDO} make clean
#${SUDO} make
#${SUDO} make uninstall
#${SUDO} make install


echo "$GREEN Your simple window manager is configured and ready to use.........[DONE]. $NC"
exit 0