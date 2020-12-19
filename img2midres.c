/*****************************************************************************
 * IMG2MIDRES - Utility to convert images into midres pictures               *
 *****************************************************************************
 * Copyright 2020 Marco Spedaletti (asimov@mclink.it)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *----------------------------------------------------------------------------
 * Concesso in licenza secondo i termini della Licenza Apache, versione 2.0
 * (la "Licenza"); è proibito usare questo file se non in conformità alla
 * Licenza. Una copia della Licenza è disponibile all'indirizzo:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Se non richiesto dalla legislazione vigente o concordato per iscritto,
 * il software distribuito nei termini della Licenza è distribuito
 * "COSÌ COM'È", SENZA GARANZIE O CONDIZIONI DI ALCUN TIPO, esplicite o
 * implicite. Consultare la Licenza per il testo specifico che regola le
 * autorizzazioni e le limitazioni previste dalla medesima.
 ****************************************************************************/

// This directive is used to deactivate the safety warning for the use 
// of fopen under Microsoft Visual Studio.
#pragma warning(disable : 4996)

/****************************************************************************
 ** INCLUDE SECTION
 ****************************************************************************/

#include "..\midres\src\midres.h"
#include "img2midres.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <ctype.h>

/****************************************************************************
 ** RESIDENT VARIABLES SECTION
 ****************************************************************************/

// Error levels.

#define ERL_WRONG_OPTIONS               1
#define ERL_MISSING_INPUT_FILENAME      2
#define ERL_MISSING_OUTPUT_FILENAME     3
#define ERL_MISSING_OUTPUT_FILENAME2    4
#define ERL_CANNOT_OPEN_INPUT           5
#define ERL_CANNOT_OPEN_OUTPUT          6
#define ERL_CANNOT_OPEN_OUTPUT2         7
#define ERL_CANNOT_OPEN_SLIDESHOW       8

// These are the default values for running the program.

Configuration configuration = {
    40 /* screen_width (in mixel) */,
    25 /* scheen_heigh (in mixel) */,
    64 /* minimum_luminance_level (0...255) */,
    1 /* compressed (0,1) */,
    16 /* palette_size (2,8,16) */,
    0, /* palette skip (0...palette_size-1)*/
    0, /* Black replacement */
    32 /* Brightness correction */
};

// This is the default palette for supported retrocomputers.
// Data taken from: 
// - https://lospec.com/palette-list/commodore64
// - https://retroshowcase.gr/index.php?p=palette (color pick)

RGB COLORS[] = {    
    // C64 and VIC20 colors
    { 0x00, 0x00, 0x00 }, // MR_COLOR_BLACK (0)    
    { 0xff, 0xff, 0xff }, // MR_COLOR_WHITE (1)
    { 0x9f, 0x4e, 0x44 }, // MR_COLOR_RED (2)
    { 0x6a, 0xbf, 0xc6 }, // MR_COLOR_CYAN (3)
    { 0xa0, 0x57, 0xa3 }, // MR_COLOR_VIOLET (4)
    { 0x5c, 0xab, 0x5e }, // MR_COLOR_GREEN (5)
    { 0x50, 0x45, 0x9b }, // MR_COLOR_BLUE (6)
    { 0xc9, 0xd4, 0x87 }, // MR_COLOR_YELLOW (7)
    { 0xa1, 0x68, 0x3c }, // MR_COLOR_ORANGE (8)
    // C64 colors only
    { 0x6d, 0x54, 0x12 }, // MR_COLOR_BROWN (9)
    { 0xcb, 0x7e, 0x75 }, // MR_COLOR_LIGHT_RED	(10)
    { 0x62, 0x62, 0x62 }, // MR_COLOR_DARK_GREY	(11)
    { 0x89, 0x89, 0x89 }, // MR_COLOR_GREY (12)
    { 0x9a, 0xe2, 0x9b }, // MR_COLOR_LIGHT_GREEN (13)
    { 0x88, 0x7e, 0xcb }, // MR_COLOR_LIGHT_BLUE (14)
    { 0xad, 0xad, 0xad },  // MR_COLOR_LIGHT_GREY (15)
    // C16 colors only
    { 0x00, 0x00, 0x00 }, // MR_COLOR_BLACK (0)    
    { 0xff, 0xff, 0xff }, // MR_COLOR_WHITE (1)
    { 0xbc, 0x68, 0x59 }, // MR_COLOR_RED (2)
    { 0x43, 0x97, 0xa6 }, // MR_COLOR_CYAN (3)
    { 0xbc, 0x52, 0xcc }, // MR_COLOR_PURPLE (4)
    { 0x43, 0xad, 0x33 }, // MR_COLOR_GREEN (5)
    { 0x80, 0x71, 0xcc }, // MR_COLOR_BLUE (6)
    { 0x80, 0x8e, 0x33 }, // MR_COLOR_YELLOW (7)
    { 0xbc, 0x6f, 0x33 }, // MR_COLOR_ORANGE (8)
    { 0x9e, 0x7f, 0x33 }, // MR_COLOR_BROWN (9)
    { 0x61, 0x9e, 0x33 }, // MR_COLOR_YELLOW_GREEN (10)
    { 0xbc, 0x61, 0x80 }, // MR_COLOR_PINK (11)
    { 0x43, 0x9e, 0x80 }, // MR_COLOR_BLUE_GREEN (12)
    { 0x43, 0x90, 0xcc }, // MR_COLOR_LIGHT_BLUE (13)
    { 0x9e, 0x61, 0xcc }, // MR_COLOR_DARK_BLUE (14)
    { 0x43, 0xa6, 0x59 } // MR_COLOR_LIGHT_GREEN (15)
};

// Pointer to the name of the file with the image to be processed.

char* filename_in = NULL;

// Pointer to the name of the file with the luminance (if not compressed)
// or the luminance/color midres image.

char* filename_out_pic = NULL;

// Pointer to the name of the file with the color map (if not compressed).

char* filename_out_col = NULL;

// Pointer to the name of the slideshow file

char* filename_slideshow = NULL;

// Starting address for mixels
int starting_address_mixels = 0;

// Starting address for colors
int starting_address_colors = 0;

// Verbose?
int verbose = 0;

// MIDRES pattern to use
unsigned char MR_RENDERED_MIXELS[16];

unsigned char MR_RENDERED_MIXELS_ATARI[16];
unsigned char MR_RENDERED_MIXELS_CBM[16];
unsigned char MR_RENDERED_MIXELS_VANILLA[16];
unsigned char MR_RENDERED_MIXELS_VDP[16];

/****************************************************************************
 ** RESIDENT FUNCTIONS SECTION
 ****************************************************************************/

// Extract the basename part of a complete file path

char* basename(char* _path) {
    char* base = strrchr(_path, '/');
    if (base == NULL) {
        base = strrchr(_path, '\\');
    }
    return base ? base + 1 : _path;
}

// Write a right-space padded string into a file

void fwrite_pad(char* _string, int _size, FILE* _file) {
    char* output = malloc(_size);
    memset(output, 32, _size);
    memcpy(output, _string, strlen(_string)+1);
    for (int i = 0; i < _size; ++i) {
        output[i] = toupper(output[i]);
    }
    fwrite(output, _size, 1, _file);
    free(output);
}

// This function calculates the color distance between two colors(_a and _b).
// By "distance" we mean the geometric distance between two points in a 
// three-dimensional space, where each dimension corresponds to one of the 
// components (red, green and blue). The returned value is normalized to 
// the nearest 8-bit value.

int calculate_distance(RGB _a, RGB _b) {

    // Extract the vector's components.
    double red = (double)_a.red - (double)_b.red;
    double green = (double)_a.green - (double)_b.green;
    double blue = (double)_a.blue - (double)_b.blue;

    // Calculate distance using Pitagora's Theorem
    return (int) sqrt(pow(red, 2) + pow(green, 2) + pow(blue, 2));

}

// This function calculates the luminance of a color. 
// By luminance we mean the modulus of the three-dimensional vector, drawn 
// in the space composed of the three components (red, green and blue).
// The returned value is normalized to the nearest 8-bit value.

int calculate_luminance(RGB _a) {

    // Extract the vector's components 
    // (each partecipate up to 1/3 of the luminance).
    double red = (double)_a.red/3;
    double green = (double)_a.green/3;
    double blue = (double)_a.blue/3;

    // Calculate luminance using Pitagora's Theorem
    return (int)sqrt(pow(red, 2) + pow(green, 2) + pow(blue, 2));

}

// This function deduces which is the most suitable color, among those 
// present in the retrocomputer palette, to replicate the incoming color. 
// The algorithm is based on the assumption that the best color is the 
// one that has the shortest color distance. This implies that the 
// distance for each color of the palette is calculated and the one with 
// the shortest distance is chosen.

mr_color calculate_midres_color_index(Configuration *_configuration, RGB _rgb ) {

    // Used as index.
    int i = 0;

    // We use the maximum value as the difference, in order to 
    // (implicitly) select the first color in palete.
    int min_distance = 255;

    // This is the result of the algorithm.
    mr_color result = MR_COLOR_BLACK;

    // Iterate for each color in palette.
    // The first color is *not* selected, since it is equal
    // to the background color.
    for (i = _configuration->palette_skip; i < _configuration->palette_size; ++i) {

        // Let's calculate the distance.
        int distance = calculate_distance(_rgb, COLORS[i+_configuration->palette_start]);

        // If the distance is less than the minimum 
        // recorded so far ...
        if (distance < min_distance) {

            // This will be the selected color
            result = i;

            // Update minimum distance recorded.
            min_distance = distance;

        }

    }

    if (result == 0) {
        result = _configuration->black_replacement;
    }

    return result;

}

// This function infers whether the pixel whose color is calculated 
// is "on" or "off". In general, this condition depends on the 
// luminance level of the pixel. If it is higher than a certain 
// threshold, the pixel should be considered "on" (and, in practice, 
// it will have a color). Otherwise, it will be considered off (and 
// it will have the background color, equal to black).

int calculate_midres_pixel_on_off(Configuration * _configuration, RGB _rgb) {

    // If the luminance is greater than the threshold value 
    // indicated by the configuration ...
    if (calculate_luminance(_rgb) > _configuration->minimum_luminance_level) {
        // ... pixel is "on".
        return mr_pixel_on;
    } else {
        // ... otherwise, pixel is "off".
        return mr_pixel_off;
    }

}

// This function generates the midres image starting from the input one.
// The algorithm used to scale the input image to be compatible with the 
// output image is the linear "mean" of a certain number of input pixels 
// for each output pixel. This algorithm should be accurate.

void generate_midres_output(Configuration* _configuration, 
                                unsigned char* _source, 
                                int _width, int _height, int _depth,
                                Output* _output) {

    // Used as index
    int i, j;
    int i0, j0;

    // Set brightness, screen width and height for midres library.
    MR_WIDTH = _configuration->screen_width;
    MR_HEIGHT = _configuration->screen_height;
    MR_BRIGHTNESS = _configuration->brightness_correction;

    // The number of mixels needed is equal to the product of the width 
    // and height.
    int surface_in_mixel = 
            _configuration->screen_width * _configuration->screen_height;

    // Let's calculate how many pixels we have to discard 
    // for the input (in height and in width).
    int wratio = _width / (_configuration->screen_width * 2);
    int hratio = _height / (_configuration->screen_height * 2);

    // Allocate the needed memory space.
    _output->surface_in_mixels = surface_in_mixel;
    _output->mixels = malloc(surface_in_mixel);
    _output->colors = malloc(surface_in_mixel);

    // Initialize spaces.
    for (i = 0; i < surface_in_mixel; ++i) {
        _output->mixels[i] = MR_RENDERED_MIXELS[0]; // empty mixel
        _output->colors[i] = MR_COLOR_BLACK; // BLACK
    }

    if (verbose) {
        printf(" ratios: %d %d\n\n", wratio, hratio);
    }

    // We move by moving along the pixels of the output image.
    for (i = 0; i < configuration.screen_height * 2; ++i) {
        if (verbose) {
            printf("%d: ", i);
        }
        for (j = 0; j < configuration.screen_width * 2; ++j) {

            RGB c;

            // We calculate from which memory address the input pixel (R, G, B)
            // to convert is found. The offset is calculated taking into 
            // account that for each pixel we must discard "wratio" horizontally 
            // and "hratio" vertically, and the fact that each pixel occupy 3 
            // bytes.

            // Let's extract the three color components from each pixel,
            // by calculating the "mean" color.

            c.red = 0; c.green = 0; c.blue = 0;
            for (i0 = 0; i0 < hratio; ++i0) {
                for (j0 = 0; j0 < wratio; ++j0) {

                    int hoff = (j * wratio)+j0;
                    if (hoff > _width) {
                        continue;
                    }

                    int voff = ( (i * hratio) * _width ) + i0;
                    if (voff > (_height * _width)) {
                        continue;
                    }
                    int offset = (voff + hoff);

                    if (offset > _width * _height * _depth) {
                        continue;
                    }

                    unsigned char* source_address =
                        (_source + offset * _depth);

                    c.red += *source_address;
                    c.green += *(source_address + 1);
                    c.blue += *(source_address + 2);
                }
            }
            c.red = c.red / (wratio * hratio);
            c.green = c.green / (wratio * hratio);
            c.blue = c.blue / (wratio * hratio);

            unsigned char pixel = calculate_midres_pixel_on_off(_configuration, c);
            mr_color color = calculate_midres_color_index(_configuration, c);

            if (verbose) {
                if (pixel == mr_pixel_on) {
                    printf("%x", color);
                }
                else {
                    printf(" ");
                }
            }
            // Let's draw (or not) the pixel and color it according to the 
            // levels set by the configuration.
            mr_psetop( _output->mixels, 
                    j, i, pixel);
            mr_pcolorop( _output->colors, 
                    j, i, color);

        }
        if (verbose) {
            printf("\n");
        }
    }

    // If a compressed image has been requested...
    if (configuration.compressed) {
        // We insert the information on the mixels and colors directly into the
        // mixels space, while zeroing the color space.
        int i;

        for (i = 0; i < surface_in_mixel; ++i) {

            _output->mixels[i] = mr_pack_mixels( 
                    _output->colors[i], 
                    get_mixel_bits(_output->mixels[i])
            );
            _output->colors[i] = MR_COLOR_BLACK;
        }
    }

}

// This function prints a short guide to the available options.

void usage_and_exit(int _level, int _argc, char* _argv[]) {

    printf("img2midres - Utility to convert images into midres pictures\n");
    printf("Copyright(c) 2020 by Marco Spedaletti.\n");
    printf("Licensed under Apache 2.0 License\n\n");
    printf("Usage: %s [options]", _argv[0]);
    printf("\n");
    printf("options:\n");
    printf("\n");
    printf("[mandatory]\n");
    printf("\n");
    printf(" -i <filename> input filename\n");
    printf("                  supported formats: \n");
    printf("                    JPEG (12 bpc not supported)\n");
    printf("                    PNG 1...16 bpc\n");
    printf("                    TGA\n");
    printf("                    BMP >1bpp, non-RLE\n");
    printf("                    PSD (composited view only, no extra channels, 8/16  bpc)\n");
    printf("                    GIF\n");
    printf("                    HDR (rgbE)\n");
    printf("                    PIC (Softimage)\n");
    printf("                    PNM (PPM / PGM binary)\n");
    printf(" -o <filename> output filename\n");
    printf(" -O <filename> additional output filename\n");
    printf("                  mandatory only if not compressed (-u)\n");
    printf(" -A <filename> add image to a slide show file\n");
    printf("\n");
    printf("[optional]\n");
    printf("\n");
    printf(" -64           use output format for Commodore 64\n");
    printf("                  alias for: \"-w 40 -h 25 -l 32 -c -p 16 -S 0 -s 0 -r 0 -B 0 -R cbm\"\n");
    printf(" -20           use output format for Commodore VIC 20\n");
    printf("                  alias for: \"-w 22 -h 23 -l 32 -c -p 8 -S 0 -s 1 -r 0 -B 0 -R cbm\"\n");
    printf(" -16           use output format for Commodore 16\n");
    printf("                  alias for: \"-w 40 -h 25 -l 32 -c -p 16 -S 0 -s 0 -r 0 -B 96 -R cbm\"\n");
    printf(" -a            use output format for Atari 800\n");
    printf("                  alias for: \"-w 40 -h 24 -l 32 -u -p 2 -S 0 -s 0 -r 0 -B 0 -R atari\"\n");
    printf(" -x            use output format for VDP (MSX, Spectravideo, Colecovision and more)\n");
    printf("                  alias for: \"-w 40 -h 24 -l 32 -u -p 2 -S 0 -s 0 -r 0 -B 0 -R vdp\"\n");
    printf(" \n");
    printf(" -b            image is just black/white\n");
    printf("                  alias for: \"-p 2\"\n");
    printf(" -B <level>    brightness correction\n");
    printf(" -c            use compressed format (mixels/color in the same file)\n");
    printf(" -h <height>   height of generated image [in characters]\n");
    printf(" -l <lum>      threshold luminance\n");
    printf(" -p <size>     size of palette (2, 8 or 16 colors)\n");
    printf(" -q            quiet execution (suppress output)\n");
    printf(" -r <index>    replace black (0) with <index> color\n");
    printf(" -R <platform> use midres tiles for specific platform:\n");
    printf("                 cbm: Commodore platforms (built-in)\n");
    printf("                 atari: Atari platforms (8 custom tiles)\n");
    printf("                 vanilla: generic platform (16 custom tiles)\n");
    printf("                 vdp: VDP chipset platform (16 custom tiles)\n");
    printf(" -s <index>    skip first <index> palette entries\n");
    printf(" -S <index>    start from <index> palette entry\n");
    printf(" -u            use uncompressed format (mixels/color in separate files)\n");
    printf(" -v            make execution verbose (leave output)\n");
    printf(" -w <width>    width of generated image [in characters]\n");
    printf(" ");

    exit(_level);

}

// This function allows to parse the options entered on the command line. 
// Options must start with a minus character ('-') and only the first letter 
// is considered.

void parse_options(int _argc, char* _argv[]) {

    // Used as index.
    int i;

    // We check for each option...
    for (i = 1; i < _argc; ++i) {

        // Parse it only if begins with '-'
        if (_argv[i][0] == '-') {

            switch (_argv[i][1]) {
                case '6': // "-64"
                    configuration.screen_width = 40;
                    configuration.screen_height = 25;
                    configuration.minimum_luminance_level = 32;
                    configuration.compressed = 1;
                    configuration.palette_start = 0;
                    configuration.palette_size = 16;
                    configuration.palette_skip = 0;
                    configuration.brightness_correction = 0;
                    starting_address_mixels = 0x0400;
                    starting_address_colors = 0xd800;
                    memcpy(MR_RENDERED_MIXELS, MR_RENDERED_MIXELS_CBM, 16);
                    break;
                case '2': // "-20"
                    configuration.screen_width = 22;
                    configuration.screen_height = 23;
                    configuration.minimum_luminance_level = 32;
                    configuration.compressed = 1;
                    configuration.palette_start = 0;
                    configuration.palette_size = 8;
                    configuration.palette_skip = 1;
                    configuration.brightness_correction = 0;
                    starting_address_mixels = 0x1e00;
                    starting_address_colors = 0x9600;
                    memcpy(MR_RENDERED_MIXELS, MR_RENDERED_MIXELS_CBM, 16);
                    break;
                case '1': // "-16"
                    configuration.screen_width = 40;
                    configuration.screen_height = 25;
                    configuration.minimum_luminance_level = 32;
                    configuration.compressed = 1;
                    configuration.palette_start = 16;
                    configuration.palette_size = 16;
                    configuration.palette_skip = 0;
                    configuration.brightness_correction = 96;
                    starting_address_mixels = 0x0C00;
                    starting_address_colors = 0x0800;
                    memcpy(MR_RENDERED_MIXELS, MR_RENDERED_MIXELS_CBM, 16);
                    break;
                case 'a': // "-a"
                    configuration.screen_width = 40;
                    configuration.screen_height = 24;
                    configuration.minimum_luminance_level = 32;
                    configuration.compressed = 0;
                    configuration.palette_start = 0;
                    configuration.palette_size = 2;
                    configuration.palette_skip = 0;
                    configuration.brightness_correction = 96;
                    starting_address_mixels = 0x0;
                    starting_address_colors = 0x0;
                    memcpy(MR_RENDERED_MIXELS, MR_RENDERED_MIXELS_ATARI, 16);
                    break;
                case 'x': // "-x"
                    configuration.screen_width = 32;
                    configuration.screen_height = 24;
                    configuration.minimum_luminance_level = 32;
                    configuration.compressed = 0;
                    configuration.palette_start = 0;
                    configuration.palette_size = 2;
                    configuration.palette_skip = 0;
                    configuration.brightness_correction = 96;
                    starting_address_mixels = 0x0;
                    starting_address_colors = 0x0;
                    memcpy(MR_RENDERED_MIXELS, MR_RENDERED_MIXELS_VDP, 16);
                    break;
                case 'w':  // "-w <width>"
                    configuration.screen_width = atoi(_argv[i + 1]);
                    ++i;
                    break;
                case 'h': // "-h <height>"
                    configuration.screen_height = atoi(_argv[i + 1]);
                    ++i;
                    break;
                case 'l': // "-l <luminance>"
                    configuration.minimum_luminance_level = atoi(_argv[i + 1]);
                    ++i;
                    break;
                case 'i': // "-i <filename>"
                    filename_in = _argv[i + 1];
                    ++i;
                    break;
                case 'o': // "-o <filename>"
                    filename_out_pic = _argv[i + 1];
                    ++i;
                    break;
                case 'O': // "-O <filename>"
                    filename_out_col = _argv[i + 1];
                    ++i;
                    break;
                case 'A': // "-A <filename>"
                    filename_slideshow = _argv[i + 1];
                    ++i;
                    break;
                case 'c': // "-c"
                    configuration.compressed = 1;
                    break;
                case 'u': // "-u"
                    configuration.compressed = 0;
                    break;
                case 'v': // "-v"
                    verbose = 1;
                    break;
                case 'q': // "-q"
                    verbose = 0;
                    break;
                case 'p': // "-p <size>"
                    configuration.palette_size = atoi(_argv[i + 1]);
                    ++i;
                    break;
                case 'r': // "-r <index>"
                    configuration.black_replacement = atoi(_argv[i + 1]);
                    ++i;
                    break;
                case 's': // "-s <index>"
                    configuration.palette_skip = atoi(_argv[i + 1]);
                    ++i;
                    break;
                case 'S': // "-S <index>"
                    configuration.palette_start = atoi(_argv[i + 1]);
                    ++i;
                    break;
                case 'B': // "-S <level>"
                    configuration.brightness_correction = atoi(_argv[i + 1]);
                    ++i;
                    break;
                case 'b': // "-b"
                    configuration.palette_size = 2;
                    break;
                case 'R': // "-R <platform>"
                    if (strcmp(_argv[i + 1], "atari")) {
                        memcpy(MR_RENDERED_MIXELS, MR_RENDERED_MIXELS_ATARI, 16);
                    } else if (strcmp(_argv[i + 1], "cbm")) {
                        memcpy(MR_RENDERED_MIXELS, MR_RENDERED_MIXELS_CBM, 16);
                    } else if (strcmp(_argv[i + 1], "vanilla")) {
                        memcpy(MR_RENDERED_MIXELS, MR_RENDERED_MIXELS_VANILLA, 16);
                    } else if (strcmp(_argv[i + 1], "vdp")) {
                        memcpy(MR_RENDERED_MIXELS, MR_RENDERED_MIXELS_VDP, 16);
                    } else {
                        printf("Unknown platform for -R: %s", _argv[i+1]);
                        usage_and_exit(ERL_WRONG_OPTIONS, _argc, _argv);
                    }
                    ++i;
                    break;
                default:
                    printf("Unknown option: %s", _argv[i]);
                    usage_and_exit(ERL_WRONG_OPTIONS, _argc, _argv);
            }

        }
    }

}

// Main function

int main(int _argc, char *_argv[]) {

    memcpy(MR_RENDERED_MIXELS, MR_RENDERED_MIXELS_CBM, 16);

    parse_options(_argc, _argv);

    if (filename_in == NULL) {
        printf("Missing input filename.\n");
        usage_and_exit(ERL_MISSING_INPUT_FILENAME, _argc, _argv);
    }

    if (filename_out_pic == NULL) {
        printf("Missing output filename for luminance.\n");
        usage_and_exit(ERL_MISSING_OUTPUT_FILENAME, _argc, _argv);
    }

    if (!configuration.compressed&&filename_out_col == NULL&&starting_address_colors>0) {
        printf("Missing output filename for color.\n");
        usage_and_exit(ERL_MISSING_OUTPUT_FILENAME2, _argc, _argv);
        exit(1);
    }

    if (verbose) {
        printf("Input image ................. %s\n", filename_in);
        if (configuration.compressed) {
            printf("Output image ................ %s\n", filename_out_pic);
        } else {
            printf("Output mixels ............... %s\n", filename_out_pic);
            if (starting_address_colors > 0) {
                printf("Output color ................ %s\n", filename_out_col);
            }
        }
        if (filename_slideshow != NULL) {
            printf("Add to slideshow ............ %s\n", filename_slideshow);
        }
    }

    int image_width, image_height, image_depth = 3;
    unsigned char *source = stbi_load(filename_in, &image_width, &image_height, &image_depth, 0);

    if (source == NULL) {
        printf("Unable to open file %s\n", filename_in);
        usage_and_exit(ERL_CANNOT_OPEN_INPUT, _argc, _argv);
    }

    if (verbose) {
        printf(" %s: (%dx%d, %d bpp)\n", filename_in, image_width, image_height, image_depth);
        printf(" luminance: %d\n\n", configuration.minimum_luminance_level);
    }

    Output result;
    generate_midres_output(&configuration, source, image_width, image_height, image_depth, &result);
    stbi_image_free(source);

    if (verbose) {
        if (configuration.compressed) {
            printf(" %s: (%dx%d, 5 bpp) [compressed]\n", filename_out_pic, configuration.screen_width*2, configuration.screen_height*2);
        } else {
            printf(" %s: (%dx%d, 1 bpp)\n", filename_out_pic, configuration.screen_width*2, configuration.screen_height*2);
            printf(" %s: (%dx%d, 4 bpp)\n", filename_out_col, configuration.screen_width*2, configuration.screen_height*2);
        }
    }

    FILE *handle = fopen(filename_out_pic, "wb");
    if (handle == NULL) {
        printf("Unable to open file %s\n", filename_out_pic);
        usage_and_exit(ERL_CANNOT_OPEN_OUTPUT, _argc, _argv);
    }

    if (starting_address_mixels > 0) {
        fwrite(&starting_address_mixels, 2, 1, handle);
    }
    fwrite(result.mixels, result.surface_in_mixels, 1, handle);
    fclose(handle);

    if (!configuration.compressed&&starting_address_colors>0) {
        handle = fopen(filename_out_col, "wb");
        if (handle == NULL) {
            printf("Unable to open file %s\n", filename_out_pic);
            usage_and_exit(ERL_CANNOT_OPEN_OUTPUT2, _argc, _argv);
        }
        fwrite(&starting_address_colors, 2, 1, handle);
        fwrite(result.colors, result.surface_in_mixels, 1, handle);
        fclose(handle);
    }

    if (filename_slideshow != NULL) {
        FILE* slideshow = fopen(filename_slideshow, "a+b");
        if (slideshow == NULL) {
            printf("Unable to open file %s\n", filename_slideshow);
            usage_and_exit(ERL_CANNOT_OPEN_SLIDESHOW, _argc, _argv);
        }
        if (configuration.compressed) {
            unsigned char command = 3; // read compressed
            fwrite(&command, 1, 1, slideshow);
            fwrite_pad(basename(filename_out_pic), 16, slideshow);
        }
        else {
            unsigned char command = 1; // read picture
            fwrite(&command, 1, 1, slideshow);
            fwrite_pad(basename(filename_out_pic), 16, slideshow);
            if (starting_address_colors > 0) {
                command = 2; // read color
                fwrite(&command, 1, 1, slideshow);
                fwrite_pad(basename(filename_out_col), 16, slideshow);
            }
        }
        fclose(slideshow);
    }

}