#!/bin/bash

source ./pascal_case.sh

content="namespace Game.Scripts.Tool;public sealed class SceneHelper{"$'\n'

while IFS= read -r file; do
	key="${file#../scenes/}"
	key="${key%.tscn}"
	content+="public static readonly SceneHelper $(to_pascal_case "$key")=new(){Key=\"res://scenes/$key.tscn\"};"$'\n'
done < <(find ../scenes -type f)

content+="public string Key{get;private init;}}"
echo "$content" > ../scripts/tool/SceneHelper.cs
