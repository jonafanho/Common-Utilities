#!/bin/bash

download() {
	url=$(curl -s $"https://files.openscad.org/snapshots/.snapshot_win64.js" | grep -oPm 1 "https://files.openscad.org/snapshots/OpenSCAD-\d\d\d\d\.\d\d.\d\d-x86-64.zip")
	echo "Downloading OpenSCAD for $1"

	rm -rf "../openscad/$1/"
	mkdir "../openscad/$1/"

	curl -Lo openscad.zip "$url"
	unzip -q openscad.zip -d "temp"
	mv temp/*/* "../openscad/$1"

	rm -r temp
	rm openscad.zip
}

library() {
	url=$"https://github.com/BelfrySCAD/BOSL2/archive/refs/heads/master.zip"
	echo "Downloading BOSL2"

	rm -rf "../openscad/libraries/"
	mkdir "../openscad/libraries/"

	curl -Lo bosl2.zip "$url"
	unzip -q bosl2.zip -d "temp"
	mv temp/*/* "../openscad/libraries"

	rm -r temp
	rm bosl2.zip
}

mkdir "../openscad/"
download windows
library
