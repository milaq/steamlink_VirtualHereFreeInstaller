#!/bin/sh

bail () {
  pkill vhf_installer_screen
  exit 1
}

./vhf_installer_screen &

echo "Downloading free VirtualHere server binary..."
mkdir -p /mnt/config/overlay/usr/local/bin/
wget https://virtualhere.com/sites/default/files/usbserver/vhusbdarm -O /mnt/config/overlay/usr/local/bin/vhusbdarm
if [ $? -ne 0 ]; then
  echo "ERROR: Download failed."
  bail
fi
chmod 755 /mnt/config/overlay/usr/local/bin/vhusbdarm

echo "Deploying VirtualHereFree server startup script..."
mkdir -p /mnt/config/overlay/etc/init.d/startup/
cp res/S98virtualherefree /mnt/config/overlay/etc/init.d/startup/S98virtualherefree

echo "Deploying VirtualHereFree server watchdog..."
mkdir -p /mnt/config/overlay/usr/local/bin/
cp res/vhfwatchdog /mnt/config/overlay/usr/local/bin/vhfwatchdog

if [ -f /mnt/config/system/virtualherefree_config.ini ]; then
  echo "VirtualHereFree server config already exists. Not overwriting."
else
  echo "Deploying default VirtualHereFree server config..."
  cp res/virtualherefree_config.ini /mnt/config/system/virtualherefree_config.ini
fi

echo "Enabling VirtualHereFree server..."
touch /mnt/config/system/virtualherefree_enabled.txt

echo "Rebooting device..."
/etc/init.d/reboot.sh &
