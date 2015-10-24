#!/bin/bash

mkdir -p ./text_original
mkdir -p ./tmp/bmg_original/tmp

for i in ./tmp/bmg_original/* ; do
	if [ -d "$i" ]; then
		continue;
	fi
	
	./text_tool -e "./tmp/bmg_original/"$(basename "$i") "./tmp/bmg_original/tmp/"$(basename "$i".out);
	iconv --from-code=SHIFT-JIS --to-code=UTF-8 "./tmp/bmg_original/tmp/"$(basename "$i".out) -o "./text_original/"$(basename "${i:0:-4}".txt);
done
