# Building Debian packages

I was expecting to get something working by similarity with existing packages. Not that easy

[Intro to debian packaging](https://wiki.debian.org/Packaging/Intro?action=show&redirect=IntroDebianPackaging)
[doc](https://www.debian.org/doc/manuals/maint-guide/)
debian package `packaging-tutorial`
Kitware [dh-cmake](https://gitlab.kitware.com/debian/dh-cmake)

Existing packages I found useful: [libzip](https://salsa.debian.org/debian/libzip), [openshot](https://salsa.debian.org/multimedia-team/libopenshot/-/tree/master/), [openmpi](https://salsa.debian.org/hpc-team/openmpi)


during investigation the deb package lintian check complained that the lib was not using symbolic link nor versioning. Fair enough. Found a Very consise anser to versioning the versioned library building with cmake in [this SO post](https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake).

## Notes

```sh
sudo apt install dh-make equivs
#https://gitlab.kitware.com/debian/dh-cmake
sudo apt install dh-cmake
```

```sh
FN=moirai-1.0
DEST=~/tmp/moirai/${FN}
mkdir -p ${DEST}
cd ~/src/github_jm/moirai
cp -R * ${DEST}/
cd ${DEST}
ls -a
```

### Creating template debian folder:

_One off_ (not on a regular basis):

```sh
cd ${DEST}
# if the debian subfolder does not exist
dh_make
# library, and yes
```

```text
Skipping creating ../moirai_1.0.orig.tar.gz because it already exists
Currently there is not top level Makefile. This may require additional tuning
Done. Please edit the files in the debian/ subdirectory now.


Make sure you edit debian/control and change the Package: lines from
moiraiBROKEN to something else, such as moirai1
```

debian/rules:Very consise anser to versioning the versioned library building with cmake in [this SO post](https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake)


```makefile
%:
	dh $@ --buildsystem=cmake
```

### Creating the source package

```sh
cd ${DEST}/..
tar -zcvf moirai_1.0.orig.tar.gz moirai-1.0
```

### Creating the source package

```sh
cd ${DEST}
debuild -us -uc 
```



