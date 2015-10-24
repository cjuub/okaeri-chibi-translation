#!/bin/bash

mkdir -p ./ncer_image_original

for i in ./tmp/nxxx_original/*.NCER ; do
	if [ -d "$i" ]; then
		continue;
	fi

	name=$(basename "${i:0:-5}");
	mkdir -p ./ncer_image_original/"$name"
	eval "./image_tool -e ./tmp/nxxx_original/"$(basename "$name")".NCGR" "./tmp/nxxx_original/"$(basename "$name")".NCER" "./ncer_image_original/"$(basename "$name");

done

mkdir -p ./nscr_image_original

for i in ./tmp/nxxx_original/*.NSCR ; do
	if [ -d "$i" ]; then
		continue;
	fi
	name=$(basename "${i:0:-5}");
	eval "./image_tool -e2 ./tmp/nxxx_original/"$(basename "$name")".NCGR" "./tmp/nxxx_original/"$(basename "$name")".NSCR" "./nscr_image_original/"$(basename "$name")".png";
done
