#!/bin/bash

# Remove the bad virtualbox guest utils package
pacman -R virtualbox-guest-utils-nox --noconfirm

# Update the system
pacman -Syu --noconfirm

# Install packages
pacman -S virtualbox-guest-utils xorg-server plasma cmake automoc4 kdelibs libkdegames4 --noconfirm

# Install the knapsen
cp -rf /mnt/knapsen/ /usr/local/src/knapsen/
mkdir /usr/local/src/knapsen/build/
cd /usr/local/src/knapsen/build

cmake ..
make
make install

# Start plasma
systemctl enable sddm
systemctl start sddm
