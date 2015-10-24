#!/bin/bash

mkdir -p ./tmp

echo -n "Extracting ROM files..."
mkdir -p ./out
./tools/ndstool -x rom.nds -9 ./out/arm9.bin -7 ./out/arm7.bin -y9 ./out/y9.bin -y7 ./out/y7.bin -d ./out/data -y ./out/overlay -t ./out/banner.bin -h ./out/header.bin > /dev/null
echo " Done!"

echo -n "Generating BMG from translated texts...";
./scripts/update_bmg_translated.sh
cp ./tmp/bmg_translated/* ./out/data/
echo " Done!";

echo -n "Generating NCGR from translated images..."
./scripts/update_ncgr_translated.sh
cp ./tmp/ncgr_translated/* ./out/data/2d/
echo " Done!";

echo -n "Repacking files into ROM..."
./tools/ndstool -c chibi_mod.nds -9 ./out/arm9.bin -7 ./out/arm7.bin -y9 ./out/y9.bin -y7 ./out/y7.bin -d ./out/data -y ./out/overlay -t ./out/banner.bin -h ./out/header.bin > /dev/null
echo " Done!"

rm -rf ./out
rm -rf ./tmp