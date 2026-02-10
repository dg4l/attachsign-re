#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "loadfile.h"
#include "savefile.h"
#define HMAC_EXPECTED_SIZE 155
#define SIGN_INSERTION_OFFSET 136

int main(int argc, char **argv) {
    int generic_retval;
    void *dst;
    bool save_failure;
    int retval;
    int ret;
    char *sign_bufptr;
    int i;
    char *outfile;
    char *infile;
    size_t alloc_size;
    size_t hmacsize;
    char *sign_buffer;
    size_t file_size;
    char *infile_buffer;
    int d_flag;
    size_t ntr_rom_size;
    char *signfile;
    
    infile_buffer = 0;
    file_size = 0;
    sign_buffer = 0;
    hmacsize = 0;
    save_failure = false;
    if (argc != 4) {
        printf("%s [INPUT file] [SIGN file] [OUTPUT file]\n", argv[0]);
        printf("%s -D [INPUT file] [OUTPUT file]\n", argv[0]);
        return -1;
    }
    generic_retval = strcmp(argv[1], "-D");
    if (!generic_retval) {
        d_flag = 1;
        infile = argv[2];
        outfile = argv[3];
        sign_buffer = malloc(HMAC_EXPECTED_SIZE);
        if (sign_buffer == 0) { 
            printf("internal memory allocation error! \n");
            return -1;
        }
        sign_buffer[0] = 'a';
        sign_buffer[1] = 'c';
        sign_buffer[2] = 1;
        memset(sign_buffer + 4,3,128);
        sign_buffer[132] = 4;
        sign_bufptr = 0;
    }
    else {
        d_flag = 0;
        infile = argv[1];
        signfile = argv[2];
        outfile = argv[3];
        generic_retval = loadFile(signfile, &sign_buffer, &hmacsize);
        if (generic_retval) {
            printf("load error! %s \n", signfile);
            return -1;
        }
        if (hmacsize != HMAC_EXPECTED_SIZE) {
            printf("invalid hmac file size is not %d =>%d %ld    %s \n",155,136,hmacsize,signfile);
            return -1;
        }
        if (((sign_buffer[0] != 'a') || (sign_buffer[1] != 'c')) || sign_buffer[2] != 1) {
            printf("invalid sign file header %s \n", signfile);
            return -1;
        }
        sign_bufptr = sign_buffer + 0x88;
    }
    generic_retval = loadFile(infile,&infile_buffer,&file_size);
    if (!generic_retval) {
        ntr_rom_size = infile_buffer[0x80];
        if (!ntr_rom_size) {
            printf("file format error! %s \n",infile);
            ret = -1;
        }
        else {
            if (!d_flag) {
                for (i = 0; i < 12; i++) {
                    if (infile_buffer[i] != sign_bufptr[i]) {
                        printf("does not match game name !\n");
                        return -1;
                    }
                    if (infile_buffer[i] == '\0') break;
                }
                                        /* 12 = gamecode offset */
                if (infile_buffer[12] != sign_bufptr[12]) {
                    printf("does not match game code !\n");
                    return -1;
                }
                                        /* 16 = makercode offset */
                if (sign_bufptr[16] != infile_buffer[16]) {
                    printf("does not match maker code !\n");
                    return -1;
                }
            }
            if (file_size < (uint8_t)ntr_rom_size) {
                    printf("error: invalid rom size !\n");
                ret = -1;
            }
            else {
                if (file_size == (uint8_t)ntr_rom_size) {
                    alloc_size = file_size + 0x88;
                }
                else if (file_size < (uint8_t)ntr_rom_size + 0x88) {
                    printf("pad..?? \n");
                    alloc_size = (uint8_t)ntr_rom_size + 0x88;
                }
                else {
                    printf("pad\n");
                    alloc_size = file_size;
                }
                dst = malloc(alloc_size);
                if (dst) {
                                        /* dest, src, size */
                    memcpy(dst,infile_buffer,file_size);
                    memcpy(((uint8_t)ntr_rom_size + dst), sign_buffer, SIGN_INSERTION_OFFSET);
                    generic_retval = saveFile(outfile, dst, alloc_size);
                    save_failure = generic_retval != 0;
                    if (save_failure) {
                        printf("save error! %s \n",outfile);
                    }
                    free(dst);
                }
                if (infile_buffer) {
                    free(infile_buffer);
                }
                if (sign_buffer) {
                    free(sign_buffer);
                }
                if (save_failure) {
                    printf("attach error %s!\n", outfile);
                    retval = -1;
                }
                else {
                    printf("attach success! %s\n",outfile);
                    retval = 0;
                }
                ret = retval;
            }
        }
    }
    else {
        printf("load error! %s \n",infile);
        ret = -1;
    }
    return ret;
}

