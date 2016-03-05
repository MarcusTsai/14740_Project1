/**
 * @file   getmime.c
 * @author Chinmay Kamat <chinmaykamat@cmu.edu>
 * @date   Thu, 14 February 2013 20:34:46 EST
 *
 * @brief  Prints the mime type of the passed file name. Does not check for
 * exitence of the file
 */

#include <stdio.h>
#include <stdlib.h>

#include <helper.h>

int main(int argc, char *argv[])
{
    char *mime_type;

    if (argc != 2) {
        printf("Usage : ./getmime <filename>\n");
        exit(1);
    }

    /* Get the mime type and print it */
    mime_type = get_mime(argv[1]);
    printf("File name: %s\t MIME Type: %s\n", argv[1], mime_type);

    return(0);
}
