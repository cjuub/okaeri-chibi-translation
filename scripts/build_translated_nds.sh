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
mkdir -p ./tmp/nsbmd_original
cp ./tmp/out/data/mdl/*.nsbmd ./tmp/nsbmd_original
echo " Done!"

echo -n "Generating BMG from translated texts...";
./scripts/update_bmg_translated.sh
cp ./tmp/bmg_translated/* ./tmp/out/data/
echo " Done!";

echo -n "Generating NCGR/NCER/NSCR from translated images..."
./scripts/update_ncgr_translated.sh
cp ./tmp/ncgr_translated/* ./tmp/out/data/2d/
cp ./tmp/ncer_translated/* ./tmp/out/data/2d/
cp ./tmp/nscr_translated/* ./tmp/out/data/2d/
echo " Done!";

echo -n "Generating nsbmd from translated textures..."
./scripts/update_nsbmd_translated.sh
cp ./tmp/nsbmd_translated/* ./tmp/out/data/mdl/
echo " Done!";

echo -n "Patching banner.bin..."
printf '\x59\x98' | dd of=./tmp/out/banner.bin bs=1 seek=2 count=2 conv=notrunc 2> /dev/null
for (( i = 576; i < 576 + 8 * 256; i += 256)); do
	printf '\x43\x00\x68\x00\x69\x00\x62\x00\x69\x00\x2d\x00\x52\x00\x6f\x00\x62\x00\x6f\x00\x3a\x00\x20\x00\x43\x00\x6c\x00\x65\x00\x61\x00\x6e\x00\x20\x00\x53\x00\x77\x00\x65\x00\x65\x00\x70\x00\x21\x00\x0a\x00\x4e\x00\x69\x00\x6e\x00\x74\x00\x65\x00\x6e\x00\x64\x00\x6f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00' | dd of=./tmp/out/banner.bin bs=1 seek=$i count=128 conv=notrunc 2> /dev/null
done
echo " Done!";

echo -n "Patching hardcoded text..."
./scripts/patch_hardcoded_text.sh
echo " Done!";

echo -n "Repacking files into ROM..."
./tools/ndstool -c chibi_mod.nds -9 ./tmp/out/arm9.bin -7 ./tmp/out/arm7.bin -y9 ./tmp/out/y9.bin -y7 ./tmp/out/y7.bin -d ./tmp/out/data -y ./tmp/out/overlay -t ./tmp/out/banner.bin -h ./tmp/out/header.bin > /dev/null
echo " Done!"

rm -rf ./tmp
