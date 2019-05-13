
Debian
====================
This directory contains files used to package kahshd/kahsh-qt
for Debian-based Linux systems. If you compile kahshd/kahsh-qt yourself, there are some useful files here.

## kahsh: URI support ##


kahsh-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install kahsh-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your kahshqt binary to `/usr/bin`
and the `../../share/pixmaps/kahsh128.png` to `/usr/share/pixmaps`

kahsh-qt.protocol (KDE)

