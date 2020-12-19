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
    extern unsigned char RENDERED_MIXELS_VANILLA[16];

#endif
