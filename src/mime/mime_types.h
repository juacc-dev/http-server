#ifndef MIME_H
#define MIME_H

#define MIME_MAX_LEN 16

enum MimeType {
    MIME_NO_TYPE,
    MIME_TEXT_PLAIN,
    MIME_TEXT_HTML,
    MIME_TEXT_CSS,
    MIME_TEXT_JS,
    MIME_IMAGE_PNG,
    MIME_IMAGE_JPG,
    MIME_IMAGE_GIF,
};

extern const char* mime_types[];
static inline const char* mime_str(enum MimeType type) {
    return mime_types[type];
}

// Get mime type of file `filename` into `mime_type`.
// The return value is 0 for success and -1 for unrecognized extension
enum MimeType get_mime_type(char* filename);

#endif /* MIME_H */
