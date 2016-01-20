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

echo -n "Generating BMG from translated texts...";
./scripts/update_bmg_translated.sh
cp ./tmp/bmg_translated/* ./tmp/out/data/
echo " Done!";

echo -n "Generating NCGR/NCER from translated images..."
./scripts/update_ncgr_translated.sh
cp ./tmp/ncgr_translated/* ./tmp/out/data/2d/
cp ./tmp/ncer_translated/* ./tmp/out/data/2d/
echo " Done!";

echo -n "Repacking files into ROM..."
./tools/ndstool -c chibi_mod.nds -9 ./tmp/out/arm9.bin -7 ./tmp/out/arm7.bin -y9 ./tmp/out/y9.bin -y7 ./tmp/out/y7.bin -d ./tmp/out/data -y ./tmp/out/overlay -t ./tmp/out/banner.bin -h ./tmp/out/header.bin > /dev/null
echo " Done!"

rm -rf ./tmp