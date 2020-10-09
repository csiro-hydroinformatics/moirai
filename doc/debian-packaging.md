
```sh
sudo apt install dh-make
#https://gitlab.kitware.com/debian/dh-cmake
sudo apt install dh-cmake
sudo apt install equivs
```

```
cp -R moirai/ /tmp/
cd /tmp/
mv moirai/ moirai-1.0/
mkdir moirai
mv moirai-1.0 moirai/
cd /tmp/moirai
```

```
tar -zcvf moirai_1.0.orig.tar.gz moirai-1.0
```

```
cd /tmp/moirai/moirai-1.0
dh_make
# librarry, and yes
```

```
Skipping creating ../moirai_1.0.orig.tar.gz because it already exists
Currently there is not top level Makefile. This may require additional tuning
Done. Please edit the files in the debian/ subdirectory now.


Make sure you edit debian/control and change the Package: lines from
moiraiBROKEN to something else, such as moirai1
```

debian/rules:

```makefile
%:
	dh $@ --buildsystem=cmake
```

ls debian/
more 
more *
more debian/*
ls
rm -rf debian/
ls
dh_make
history



2010  dh_cmake_install 
 2011  dh_make
 2012  ls
 2013  cd ..
 2014  ls
 2015  cd ..
 2016  ls
 2017  mkdir debtut
 2018  cd debtut/
 2019  s
 2020  ls
 2021  dget http://snapshot.debian.org/archive/debian-archive/
 2022  20090802T004153Z/debian/dists/bo/main/source/web/
 2023  dget http://snapshot.debian.org/archive/debian-archive/20090802T004153Z/debian/dists/bo/main/source/web/wget_1.4.4-6.dsc
 2024  ls
 2025  debcheckout --help
 2026  debcheckout wget_1.4.4-6.dsc
 2027  ls
 2028  dpkg-source -x wget_1.4.4-6.dsc
 2029  ls
 2030  ls wget-1.4.4/
 2031  history

dget http://snapshot.debian.org/archive/debian-archive/20090802T004153Z/debian/dists/bo/main/source/web/wget_1.4.4-6.dsc




Using as a template: 
https://salsa.debian.org/debian/libzip
https://salsa.debian.org/multimedia-team/libopenshot/-/tree/master/

Still bamboozled by this all. 
Back to more basic

https://wiki.debian.org/Packaging/Intro?action=show&redirect=IntroDebianPackaging


https://www.debian.org/doc/manuals/maint-guide/