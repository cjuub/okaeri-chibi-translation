#!/bin/bash

mkdir -p ./tmp

echo -n "Extracting ROM files..."
mkdir -p ./tmp/out
./tools/ndstool -x rom.nds -9 ./tmp/out/arm9.bin -7 ./tmp/out/arm7.bin -y9 ./tmp/out/y9.bin -y7 ./tmp/out/y7.bin -d ./tmp/out/data -y ./tmp/out/overlay -t ./tmp/out/banner.bin -h ./tmp/out/header.bin > /dev/null
mkdir -p ./tmp/bmg_original
cp ./tmp/out/data/*.bmg ./tmp/bmg_original
mkdir -p ./tmp/nxxx_original
cp ./tmp/out/data/2d/*.NCER ./tmp/nxxx_original
cp ./tmp/out/data/2d/*.NCGR ./tmp/nxxx_original
cp ./tmp/out/data/2d/*.NSCR ./tmp/nxxx_original
rm ./tmp/nxxx_original/SandBGDef.*
rm ./tmp/nxxx_original/SandPan.*
echo " Done!"

echo -n "Extracting text into folder text_original..."
./scripts/extract_text_original.sh
echo " Done!"

echo -n "Extracting images into folders ncer_image_original and nscr_image_original..."
./scripts/extract_image_original.sh
echo " Done!"

rm -rf ./tmp