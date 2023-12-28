#include <stdio.h>
#include "textures.h"
#include "constants.h"

texture_t wallTexture[NUM_TEXTURES];

static const char* textureFileNames[NUM_TEXTURES] = {
    "./images/redbrick.png",
    "./images/purplestone.png",
    "./images/mossystone.png",
    "./images/graystone.png",
    "./images/colorstone.png",
    "./images/bluestone.png",
    "./images/wood.png",
    "./images/eagle.png"
};


void loadWallTextures()
{
    // load PNG textures
    for (int i = 0; i < NUM_TEXTURES; i++)
    {
        upng_t* upng;

        upng = upng_new_from_file(textureFileNames[i]);

        if (upng != NULL)
        {
            upng_decode(upng);
            if (upng_get_error(upng) == UPNG_EOK)
            {
                wallTexture[i].upngTexture = upng;
                wallTexture[i].width = upng_get_width(upng);
                wallTexture[i].height = upng_get_height(upng);
                wallTexture[i].texture_buffer = (uint32_t *)upng_get_buffer(upng);
                // upng_free(upng);
            }
        }
    }
}

void freeWallTextures()
{
    for (int i = 0; i < NUM_TEXTURES; i++)
    {
        upng_free(wallTexture[i].upngTexture);
    }
}

