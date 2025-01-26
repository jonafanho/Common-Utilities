#!/bin/bash

to_pascal_case() {
	echo "$1" | awk '
		BEGIN {
			FS="[[:space:][:punct:]]+";
			OFS="";
		}
		{
			for (i = 1; i <= NF; i++) {
				# Convert each word to PascalCase
				if (length($i) > 0) {
					printf "%s", toupper(substr($i,1,1)) tolower(substr($i,2))
				}
			}
			printf "\n"
		}'
}
