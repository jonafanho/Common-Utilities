#!/bin/bash

source ./pascal_case.sh

content="using Godot;namespace Game.Scripts.Tool;public sealed class TranslationHelper{"$'\n'

while IFS=, read -r key rest; do
	if [ ${#key} -ge 2 ]; then
		content+="public static readonly TranslationHelper $(to_pascal_case "$key")=new(){Key=\"$key\"};"$'\n'
	fi
done < <(tail -n +2 ../translations/translations.csv)

content+="public string Key{get;private init;}public string Translate(params string[] arguments){string text=TranslationServer.Translate(Key).ToString();for(int i=0;i<arguments.Length;i++)text=text.Replace(\"{{\"+i+\"}}\",arguments[i]);return text;}}"
echo "$content" > ../scripts/tool/TranslationHelper.cs
