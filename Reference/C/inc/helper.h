/**
 * @file   helper.h
 * @author Chinmay Kamat <chinmaykamat@cmu.edu>
 * @date   Thu, 14 February 2013 20:26:17 EST
 */

#ifndef __HELPER_H__
#define __HELPER_H__

enum mime_index {
    TXT,
    HTML,
    CSS,
    PNG,
    JPG,
    GIF,
    OTHER
};

char *get_mime(char *path);
#endif
