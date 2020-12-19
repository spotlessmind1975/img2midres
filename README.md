# img2midres
Utility to convert images into midres pictures

## USAGE

<pre>img2midres.exe [options]</pre>

By omitting the options, you get online help.

## OPTIONS (mandatory)

`-i <filename>` input filename

With this option you can specify the image file to be processed. The supported input formats are as follows:

 * JPEG (12 bpc not supported)
 * PNG 1...16 bpc
 * TGA
 * BMP >1bpp, non-RLE
 * PSD (composited view only, no extra channels, 8/16  bpc)
 * GIF
 * HDR (rgbE)
 * PIC (Softimage)
 * PNM (PPM / PGM binary)

`-o <filename>` output filename

With this option you can indicate the name of the file where the image converted to midres format will be written. By convention, the following extensions should be used:
 * .mpic - for compressed images (option `-c`);
 * .pic - for uncompressed images (option `-u`).


`-O <filename>` additional output filename

With this option you can indicate the name of the additional file where the color map of midres converted image will be written. This option is required only if you choose not to compress the midres picture and you choose a platform that allow to use colors (like Commodore). By convention, the following extension should be used:
 * .col - for uncompressed images (option `-u`).

`-A <filename>` add image to a slide show file

This option allows you to indicate any slideshow file to be updated. If the file does not exist, it is created. If the file exists, it is updated. The image (compressed or uncompressed) is added to the list of images to show. The file name will be exactly as indicated by the output options, limited to the file name only and omitting the path.

## OPTIONS

`-64`           use output format for Commodore 64

This option is a shortcut for the following options: 
`-w 40 -h 25 -l 32 -c -p 16 -S 0 -s 0 -r 0 -B 0 -R cbm`

`-20`           use output format for Commodore VIC 20

This option is a shortcut for the following options: 
`-w 22 -h 23 -l 32 -c -p 8 -S 0 -s 1 -r 0 -B 0 -R cbm`

`-16`           use output format for Commodore 16-PLUS/4

This option is a shortcut for the following options: 
`-w 40 -h 25 -l 32 -c -p 16 -S 0 -s 0 -r 0 -B 96 -R cbm`

`-a`            use output format for Atari 800 (luminance only)

This option is a shortcut for the following options: 
`-w 40 -h 24 -l 32 -u -p 2 -S 0 -s 0 -r 0 -B 0 -R atari`

`-b`            image is just black/white

This option is a shortcut for `-p 2`

`-B <level>`    brightness correction

Valid for Commodore 16 and for uncompressed images only:it allows to indicate the base brightness level (0..7) for the color map.

`-c`            use compressed format

The output file with the midres image will be one, and will contain both the information on the mixels (lower nibble) and the information on the color (higher nibble), which will be stored without any kind of brightness correction (see option `-B `). This image will not be directly viewable, but it will require a preventive decompression operation on an auxiliary screen. If you choose to generate a slideshow, the program will be able to do this transparently.

`-h <height>`   height of generated image

It is possible to indicate the height of the image to be generated, in terms of characters (equivalent to double the mixels). For example, to generate an image 50 mixels high, 25 characters must be entered (50/2 = 25).

`-l <lum>`      threshold luminance

It is possible to indicate the luminance threshold, above which the mixel is considered and below which the mixel is ignored. This parameter is important to ensure image quality in presence of many shades. A value of zero implies that all mixels will be drawn and the image will depend solely on the color component. Conversely, too high a value of this parameter will result in a completely black image.

`-p <size>`     size of palette

 This parameter allows to size the number of colors used for the adaptive palette. In general, this parameter should be identical to the palette size for the hardware under consideration (8 colors for the VIC 20, 16 colors for the other platforms). However, it is possible to indicate a narrower range, where certain effects are desired.

`-q`            quiet execution

This option disables any type of output, making the program suitable for running in a batch or makefile context.

`-r <index>`    replace black (0) with another color

This option allows you to indicate which color of the palette should be used instead of black (0). This option can be useful in those cases where the image is particularly low in contrast.

`-r <platform>` use midres tiles for specific platform

This option allows you to specify which set of tiles will be used to implement the MIDRES graphics. You can choose `cbm` to indicate the standard (built-in) tiles of the PETSCII character set or `atari` to indicate the set **[ztiles.bin](https://github.com/spotlessmind1975/midres/blob/master/data/ztiles.bin)** loaded on departure from the library for this platform (8 tiles). Finally, you can choose `vanilla` in order to choose the complete 16 tiles, stored in **[mtiles.bin](https://github.com/spotlessmind1975/midres/blob/master/data/ztiles.bin)**.

`-s <index>`    skip first palette entries

This option allows not to consider the first colors of the palette in the calculation of color distances; the remaining colors will, however, be used. For example, if the palette is 16 colors and you are asked to skip the first 3 colors, the palette that will be used will have 13 colors.

`-S <index>`    start from palette entry

This option allows to start from a certain index in the palette, instead of the first color. This option differs from `-s` in that it is a mere offset from the list of stored colors. If the palette has 8 colors and it is indicated to start from color 3, 8 colors will still be considered.

`-u`            use uncompressed format

The output files with the midres image will be two, and they will contain the first the information on the mixels and the second thhe information on the color, which will be stored with brightness correction (see option `-B `). This image will be directly viewable by loading it into text and color memory. If you choose to generate a slideshow, the program will be able to do this transparently, and in sequence.

`-v`            make execution verbose

Activates the display of all essential information, as well as an ASCII representation of the processed image.

`-w <width>`    width of generated image

It is possible to indicate the width of the image to be generated, in terms of characters (equivalent to double the mixels). For example, to generate an image 80 mixels width, 40 characters must be entered (80/2 = 40).

## LICENSE
Copyright 2020 Marco Spedaletti

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.