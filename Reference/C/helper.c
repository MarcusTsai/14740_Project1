/**
 * @file   helper.c
 * @author Chinmay Kamat - ckamat
 * @date   Thu, 14 February 2013 20:34:25 EST
 *
 * @brief  Contains various helper functions
 *
 */

#include <stdlib.h>
#include <string.h>

#include <helper.h>

/* Supported mime types */
char *mimes[] = {
    [HTML] = "text/html",
    [CSS]  = "text/css",
    [TXT]  = "text/plain",
    [PNG]  = "image/png",
    [JPG]  = "image/jpg",
    [GIF]  = "image/gif",
    [OTHER]= "application/octet-stream"
};

/**
 * Function to get mime type of the response file
 *
 * @param path - Path from request URI
 *
 * @return mime-type of path
 */
char *get_mime(char *path)
{
    char *ext_start;

    /* Make sure input is proper */
    if (NULL == path) {
        return mimes[OTHER];
    }

    /* Get the file extension */
    ext_start = strrchr(path, '.');

    if (ext_start == NULL) {
        return mimes[OTHER];
    } else {
        if (strlen(ext_start) == 4) {
            if (!(strncmp(ext_start, ".txt", 4))) {
                return mimes[TXT];
            } else if (!(strncmp(ext_start, ".css", 4))) {
                return mimes[CSS];
            } else if (!(strncmp(ext_start, ".png", 4))) {
                return mimes[PNG];
            } else if (!(strncmp(ext_start, ".jpg", 4))) {
                return mimes[JPG];
            } else if (!(strncmp(ext_start, ".gif", 4))) {
                return mimes[TXT];
            } else {
                return mimes[OTHER];
            }
        } else if (strlen(ext_start) == 5) {
            if (!strncmp(ext_start, ".html", 5)) {
                return mimes[HTML];
            }
        }
    }

    /* No matching extension found */
    return mimes[OTHER];
}
