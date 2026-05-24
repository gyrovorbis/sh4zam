#include <sh4zam/shz_version.h>

#include <string.h>
#include <stdio.h>

shz_version_t shz_version_linked(void) {
    return SHZ_VERSION;
}

void shz_version_fields(shz_version_t version, uint8_t* major, uint16_t* minor, uint8_t* patch) {
    if(major)
        *major = ((version >> 24) & 0xff);

    if(minor)
        *minor = ((version >> 8) & 0xffff);

    if(patch)
        *patch = (version & 0xff);
}