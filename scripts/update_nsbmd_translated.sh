#!/bin/bash

mkdir -p ./tmp
mkdir -p ./tmp/nsbmd_translated

for i in ./nsbmd_image_translated/* ; do
    if [ $(basename "$i") == "LICENSE" ]; then
        continue;
    fi

    name=$(basename "$i");
    eval "mkdir -p " "./tmp/"$name

    for j in $i/* ; do
        name2=$name/$(basename "$j");
        eval "./png_merge ./nsbmd_image_original/"$name2 "./nsbmd_image_translated/"$name2 "./tmp/"$name2;
    done

    eval "./image_tool -i3 ./tmp/nsbmd_original/"$name".nsbmd" "./tmp/"$name "./tmp/nsbmd_translated/"$name".nsbmd";
done
