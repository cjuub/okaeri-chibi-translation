#!/bin/bash

./blz -d ./tmp/out/overlay/overlay_0011.bin > /dev/null
./blz -d ./tmp/out/overlay/overlay_0028.bin > /dev/null
./blz -d ./tmp/out/overlay/overlay_0035.bin > /dev/null

# "Apprentice"
printf '\x41\x70\x70\x72\x65\x6E\x74\x69\x63\x65\x00\x00' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=52264 count=12 conv=notrunc 2> /dev/null
# "Rank 5"
printf '\x52\x61\x6E\x6B\x20\x35\x00\x00' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=52276 count=8 conv=notrunc 2> /dev/null
# "Rank 4"
printf '\x52\x61\x6E\x6B\x20\x34\x00\x00' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=52284 count=8 conv=notrunc 2> /dev/null
# "Rank 3"
printf '\x52\x61\x6E\x6B\x20\x33\x00\x00' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=52292 count=8 conv=notrunc 2> /dev/null
# "Rank 2"
printf '\x52\x61\x6E\x6B\x20\x32\x00\x00' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=52300 count=8 conv=notrunc 2> /dev/null
# "Rank 1"
printf '\x52\x61\x6E\x6B\x20\x31\x00\x00' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=52308 count=8 conv=notrunc 2> /dev/null
# "Vice Squad Leader"
printf '\x56\x69\x63\x65\x20\x53\x71\x75\x61\x64\x20\x4C\x65\x61\x64\x65\x72\x00\x00\x00' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=52316 count=20 conv=notrunc 2> /dev/null
# "Squad Leader"
printf '\xB0\x70\x26\x02' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=29988 count=4 conv=notrunc 2> /dev/null
printf '\x53\x71\x75\x61\x64\x20\x4C\x65\x61\x64\x65\x72\x00\x00' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=52336 count=14 conv=notrunc 2> /dev/null
# "Vice Officer"
printf '\xBE\x70\x26\x02' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=29992 count=4 conv=notrunc 2> /dev/null
printf '\x56\x69\x63\x65\x20\x4F\x66\x66\x69\x63\x65\x72\x00\x00' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=52350 count=14 conv=notrunc 2> /dev/null
# "Officer"
printf '\xCC\x70\x26\x02' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=29996 count=4 conv=notrunc 2> /dev/null
printf '\x4F\x66\x66\x69\x63\x65\x72\x00\x00\x00' | dd of=./tmp/out/overlay/overlay_0011.bin bs=1 seek=52364 count=10 conv=notrunc 2> /dev/null

# "What's this?"
printf '\x57\x68\x61\x74\x27\x73\x20\x74\x68\x69\x73\x3f\x00\x00\x00\x00' | dd of=./tmp/out/overlay/overlay_0028.bin bs=1 seek=70232 count=16 conv=notrunc 2> /dev/null
# "Details"
printf '\x44\x65\x74\x61\x69\x6c\x73\x00\x00' | dd of=./tmp/out/overlay/overlay_0028.bin bs=1 seek=70248 count=8 conv=notrunc 2> /dev/null
# "More details"
printf '\x4d\x6f\x72\x65\x20\x64\x65\x74\x61\x69\x6c\x73\x00\x00\x00\x00\x00\x00\x00' | dd of=./tmp/out/overlay/overlay_0028.bin bs=1 seek=70260 count=18 conv=notrunc 2> /dev/null
# "Records"
printf '\x52\x65\x63\x6f\x72\x64\x73\x00' | dd of=./tmp/out/overlay/overlay_0028.bin bs=1 seek=70276 count=8 conv=notrunc 2> /dev/null
# "Back"
printf '\x42\x61\x63\x6b\x00\x00\x00\x00' | dd of=./tmp/out/overlay/overlay_0028.bin bs=1 seek=70284 count=8 conv=notrunc 2> /dev/null

# "*Click*!"
printf '\x2A\x43\x6C\x69\x63\x6B\x2A\x21\x00\x00\x00\x00' | dd of=./tmp/out/overlay/overlay_0035.bin bs=1 seek=71732 count=12 conv=notrunc 2> /dev/null

./blz -en ./tmp/out/overlay/overlay_0011.bin > /dev/null
./blz -en ./tmp/out/overlay/overlay_0028.bin > /dev/null
./blz -en ./tmp/out/overlay/overlay_0035.bin > /dev/null


FILE_SIZE=$(stat --printf "%s" ./tmp/out/overlay/overlay_0011.bin)
FILE_SIZE=$(printf "%x\n" $FILE_SIZE)
echo -n -e "\\x"${FILE_SIZE:2:2}"\\x"${FILE_SIZE:0:2} | dd of=./tmp/out/y9.bin bs=1 seek=380 count=2 conv=notrunc 2> /dev/null

FILE_SIZE=$(stat --printf "%s" ./tmp/out/overlay/overlay_0028.bin)
FILE_SIZE=$(printf "%x\n" $FILE_SIZE)
echo -n -e "\\x"${FILE_SIZE:2:2}"\\x"${FILE_SIZE:0:2} | dd of=./tmp/out/y9.bin bs=1 seek=924 count=2 conv=notrunc 2> /dev/null