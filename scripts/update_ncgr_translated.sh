#!/bin/bash

mkdir -p ./tmp
mkdir -p ./tmp/ncgr_translated
mkdir -p ./tmp/ncer_translated

for i in ./ncer_image_translated/* ; do
    if [ $(basename "$i") == "LICENSE" ]; then
        continue;
    fi

    name=$(basename "$i");
    eval "mkdir -p " "./tmp/"$name
    eval "cp ./ncer_image_translated/"$name"/*.meta" "./tmp/"$name" 2> /dev/null";
    eval "cp ./ncer_image_translated/"$name"/*_*.png" "./tmp/"$name" 2> /dev/null";

    for j in $i/* ; do
        if [ "${j: -5}" == ".meta" ] || [ "${j: -6:1}" == "_" ]; then
          continue;
        fi

        name2=$name/$(basename "$j");
        eval "./png_merge ./ncer_image_original/"$name2 "./ncer_image_translated/"$name2 "./tmp/"$name2;
    done

    eval "./image_tool -i ./tmp/nxxx_original/"$(basename "$name")".NCGR" "./tmp/nxxx_original/"$(basename "$name")".NCER" "./tmp/"$(basename "$name") "./tmp/ncgr_translated/"$(basename "$name")".NCGR" "./tmp/ncer_translated/"$(basename "$name")".NCER";
done

for i in ./nscr_image_translated/* ; do
    if [ $(basename "$i") == "LICENSE" ]; then
        continue;
    fi

    name=$(basename "$i");
    eval "./png_merge ./nscr_image_original/"$name "./nscr_image_translated/"$name "./tmp/"$name;
    eval "./image_tool -i2 ./tmp/nxxx_original/"${name:0:-4}".NCGR" "./tmp/nxxx_original/"${name:0:-4}".NSCR" "./tmp/"$(basename "$name") "./tmp/ncgr_translated/"${name:0:-4}".NCGR";
done