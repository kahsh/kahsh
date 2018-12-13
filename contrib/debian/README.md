
Debian
====================
This directory contains files used to package dilithiumd/dilithium-qt
for Debian-based Linux systems. If you compile dilithiumd/dilithium-qt yourself, there are some useful files here.

## dilithium: URI support ##


dilithium-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install dilithium-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your dilithiumqt binary to `/usr/bin`
and the `../../share/pixmaps/dilithium128.png` to `/usr/share/pixmaps`

dilithium-qt.protocol (KDE)

