#!/bin/bash

mkdir -p ./tmp
mkdir -p ./tmp/bmg_translated
mkdir -p ./tmp/trans
mkdir -p ./tmp/conv

for i in ./text_translated/* ; do
	if [ -d "$i" ]; then
		continue;
	fi

	eval "./txt_merge text_original/$(basename "${i:0:-6}".txt) text_translated/$(basename "${i:0:-6}".trans) ./tmp/trans/$(basename "${i:0:-6}".trans)"
	iconv --from-code=UTF-8 --to-code=SHIFT-JIS "./tmp/trans/"$(basename "${i:0:-6}".trans) -o "./tmp/conv/"$(basename "${i:0:-6}".out);
	./text_tool -i "./tmp/conv/"$(basename "${i:0:-6}".out) "./tmp/bmg_translated/"$(basename "${i:0:-6}".bmg);
done
