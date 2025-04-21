#include "mime_types.h"

#include <string.h>

const char* mime_types[] = {
    [MIME_NO_TYPE] = NULL,

    [MIME_TEXT_PLAIN] = "text/plain",
    [MIME_TEXT_HTML]  = "text/html",
    [MIME_TEXT_CSS]   = "text/css",
    [MIME_TEXT_JS]    = "application/js",
    [MIME_IMAGE_PNG]  = "image/png",
    [MIME_IMAGE_JPG]  = "image/jpg",
    [MIME_IMAGE_GIF]  = "image/gif",
};

// Apparently this is not that bad
enum MimeType get_mime_type(char* filename) {
    const char* ext = strrchr(filename, '.');

    if(!strcmp(ext, ".txt"))
        return MIME_TEXT_PLAIN;

    else if(!strcmp(ext, ".html"))
        return MIME_TEXT_HTML;

    else if(!strcmp(ext, ".css"))
        return MIME_TEXT_CSS;

    else if(!strcmp(ext, ".js"))
        return MIME_TEXT_JS;

    else if(!strcmp(ext, ".png"))
        return MIME_IMAGE_PNG;

    else if(!strcmp(ext, ".jpg"))
        return MIME_IMAGE_JPG;

    else if(!strcmp(ext, ".gif"))
        return MIME_IMAGE_GIF;

    else
        return MIME_NO_TYPE;

    return 0;
}
