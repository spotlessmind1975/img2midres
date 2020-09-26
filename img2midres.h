/****************************************************************************
 * img2midres - Utility to convert images into midres pictures              *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * INCLUDE FILE                                                             *
 ****************************************************************************/

#ifndef _IMG2MIDRES_H_
#define _IMG2MIDRES_H_

     /************************************************************************
      * ------ DATA TYPES
      ************************************************************************/

    // This structure stores the color components (red, blue and green) of a 
    // pixel, 8 bits wide. This structure is used both to represent the 
    // retrocomputer palette and to process input data from image files.
    typedef struct {
        int red;
        int green;
        int blue;
    } RGB;


    // This structure maintains the program's options, and used by the process 
    // of converting from image files to midres.

    typedef struct {

        // Screen width (in mixels)
        int screen_width;

        // Screen height (in mixels)
        int screen_height;

        // Minimum luminance level (0...255)
        int minimum_luminance_level;

        // Luminance and color in a single file?
        int compressed;

        // Palette size
        int palette_size;

        // Palette start
        int palette_start;

        // Palette skip
        int palette_skip;

        // Black replacement
        int black_replacement;

        // Brightness correction
        int brightness_correction;

    } Configuration;

    // This structure maintain the result of conversion operation.

    typedef struct {

        // Surface of the image (in mixels)
        int surface_in_mixels;

        // Luminance's data
        mr_mixel* mixels;

        // Color's data
        mr_color* colors;

    } Output;

    extern unsigned char RENDERED_MIXELS_ATARI[16];
    extern unsigned char RENDERED_MIXELS_CBM[16];

#endif
