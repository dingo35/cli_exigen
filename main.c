#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// External project headers
#include "iso20_AC_Encoder.h"
#include "iso20_AC_Datatypes.h"
#include "exi_basetypes.h"
#include "iso20_AC_Decoder.h"
/*
// Bitstream struct as defined
typedef struct exi_bitstream {
    uint8_t* data;
    size_t data_size;

    uint8_t bit_count;
    size_t byte_pos;

    uint8_t _init_called;
    size_t _flag_byte_pos;

    exi_status_callback status_callback;
} exi_bitstream_t;
*/
// Convert a single hex character to its numeric value
int hex_char_to_int(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return 10 + (c - 'a');
    if ('A' <= c && c <= 'F') return 10 + (c - 'A');
    return -1;
}

// Convert hex string to byte buffer
size_t hex_to_bytes(const char *hex, uint8_t **out) {
    size_t len = strlen(hex);
    if (len % 2 != 0) {
        fprintf(stderr, "Error: Hex string must have an even number of characters.\n");
        return 0;
    }

    size_t num_bytes = len / 2;
    uint8_t *bytes = malloc(num_bytes);
    if (!bytes) {
        perror("malloc");
        return 0;
    }

    for (size_t i = 0; i < num_bytes; ++i) {
        int hi = hex_char_to_int(hex[2 * i]);
        int lo = hex_char_to_int(hex[2 * i + 1]);

        if (hi == -1 || lo == -1) {
            fprintf(stderr, "Error: Invalid hex character at position %zu.\n", 2 * i);
            free(bytes);
            return 0;
        }

        bytes[i] = (hi << 4) | lo;
    }

    *out = bytes;
    return num_bytes;
}

// Initialize an exi_bitstream_t with the given data
void init_exi_bitstream(exi_bitstream_t *stream, uint8_t *data, size_t size) {
    stream->data = data;
    stream->data_size = size;

    stream->bit_count = 0;
    stream->byte_pos = 0;

    stream->_init_called = 1;
    stream->_flag_byte_pos = 0;

    stream->status_callback = NULL; // Can assign a real callback if needed
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hexstring>\n", argv[0]);
        return 1;
    }

    const char *hex_input = argv[1];
    uint8_t *data = NULL;
    size_t size = hex_to_bytes(hex_input, &data);

    if (size == 0) {
        return 1;
    }

    exi_bitstream_t bitstream;
    init_exi_bitstream(&bitstream, data, size);

    struct iso20_ac_exiDocument exiDoc;
    decode_iso20_ac_exiDocument(&bitstream, &exiDoc);

    // Debug print
    printf("Bitstream initialized (%zu bytes):\n", bitstream.data_size);
    for (size_t i = 0; i < bitstream.data_size; ++i) {
        printf("%02x ", bitstream.data[i]);
    }
    printf("\n");

    // Future EXI decoder or encoder usage goes here
    // Example: decode_iso20_AC_Message(&bitstream, &your_struct);

    free(data);
    return 0;
}

