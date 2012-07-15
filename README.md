shark
=====

A super lightweight package manager for linux.



Compilation
===========

1. Invoke ./configure
2. Type make



Installation
============
Shark should not be installed via 'make install'. Shark should be placed in the /usr/bin of the root of your enviorment. In the future you will be able to run 'make pkg' which will automatically generate a shark package for you.



Packages
========
Currently, no package hosts exist (I change the package format too often for it to be possible to host)
However, if you want to bundle your own packages, here is a guide.
1. Make a new folder somewhere (eg pkg-ver)
2. Isolate all the package files into a directory (keeping the directory structure, there should probably be a pkg-ver/usr and a /pkg-ver/bin directory and so on)
3. Run shark --gen-dirs /path/to/dir
4. Run shark --gen-files /path/to/dir
5. Copy over the 'files' and 'dirs' files to the root of your package folder (they should be in the directory where you ran shark from)
6. Write a info file that looks like this:
PKG_NAME=pkg
PKG_VER=ver
7. 'tar cf pkg-ver.tar.gz /path/to/dir'
8. Install your package with shark -UI

