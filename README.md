# okaeri-chibi-translation

This is a fan translation project for the game *Okaeri! Chibi-Robo! Happy Richie Ōsōji!* for the Nintendo DS. The project aims to translate the game without including original copyrighted contents.
This means that:

* All graphics in the repository only contain the custom made parts of the graphics.
* All text in the repository only contain the custom made translated strings.

## Build
The process of building the translated ROM is detailed below. Be sure to do the steps in order.

##### Getting the necessary files
* In order to extract the original files, the original ROM in .nds-format is needed. Place this file in the root of the repository and rename it to *rom.nds*.
* Build the binary *ndstool* from [here](https://github.com/devkitPro/ndstool) and place it in a folder *tools* in the repository root.

##### Building the translation tools on Linux
In the repository root, simply run:

`make`

##### Building the translation tools on Windows
Install [Cygwin](https://www.cygwin.com/). Be sure to select the complete *devel* package list in the installer. Cygwin is also required in order to run all the scripts used in this project.

Use Cygwin to navigate to the repository root and run:

`make`

##### Extracting the original text and images
In the repository root, run:

`./scripts/extract_original_files.sh`

This will produce the folders *text_original*, *ncer_image_original* and *nscr_image_original*, containing the original text and images in normal text- or png-form.

##### Building the translated ROM
In the repository root, run:

`./scripts/build_translated_nds.sh`

This will output the translated ROM as *chibi_mod.nds*, ready to be used.

## Licenses
Creative Commons Attribution-NonCommercial 4.0 International applies for all graphics and text in the folders and subfolders where that license exist. The New BSD license specified in LICENSE in the project root applies to all other files.

The name *Chibi-Robo!* is a trademark of Nintendo and skip Ltd.
