# okaeri-chibi-translation

## Build
The process of building the translated ROM is detailed below. Be sure to do the steps in order. Only Linux is supported for now.

### Getting the necessary files
* In order to extract the original files, the original ROM in .nds-format is needed. Place this file in the root of the repository and rename it to *rom.nds*.
* Build the binary *ndstool* from [here](https://github.com/devkitPro/ndstool) and place it in a folder *tools* in the repository root.

### Building the translation tools
In the repository root, simply run:

`make`

### Extracting the original text and images
In the repository root, run:

`./scripts/extract_original_files.sh`

This will produce the folders *text_original*, *ncer_image_original* and *nscr_image_original*, containing the original text and images in normal text- or png-form.

### Building the translated ROM
In the repository root, run:

`./scripts/build_translated_nds.sh`

This will output the translated ROM as *chibi_mod.nds*, ready to be used.

## Licenses
Creative Commons Attribution-NonCommercial 4.0 International applies for all graphics and text in the folders and subfolders where that license exist. The New BSD license specified in LICENSE in the project root applies to all other files.

The name Chibi-Robo! is a trademark of Nintendo and skip Ltd.