#!/bin/bash

download() {
	version=$(curl -s "https://api.github.com/repos/godotengine/godot-builds/releases" | grep -oPm 1 "\"name\":\s*\"\K[^\"]+")
	echo "Downloading Godot $version for $1"

	rm -rf "../godot/$1/"
	mkdir "../godot/$1/"

	curl -Lo godot.zip "https://github.com/godotengine/godot-builds/releases/download/${version}/Godot_v${version}_mono_$2.zip"
	unzip -q godot.zip -d "temp"
	mv temp/*/* "../godot/$1"

	for file in "../godot/$1/"Godot_v*_mono_*; do
		new_name=${file//"v${version}_mono_"/''}
		mv "$file" "$new_name"
	done

	rm -r temp
	rm godot.zip
}

mkdir "../godot/"
download linux linux_x86_64
download windows win64
