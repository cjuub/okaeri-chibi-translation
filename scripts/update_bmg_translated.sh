#!/bin/bash

mkdir -p ./tmp
mkdir -p ./text_translated/tmp
mkdir -p ./tmp/bmg_translated

for i in ./text_translated/* ; do
	if [ -d "$i" ]; then
		continue;
	fi

	iconv --from-code=UTF-8 --to-code=SHIFT-JIS "$i" -o "./text_translated/tmp/"$(basename "$i".out);
	./text_tool -i "./text_translated/tmp/"$(basename "$i".out) "./tmp/bmg_translated/"$(basename "${i:0:-6}".bmg);
done

rm -r ./text_translated/tmp
