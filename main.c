#include "stdio.h"
#include "stdlib.h"
#include <stdint.h>

#pragma pack(1)
typedef struct {
    char mthd[4];
    //char c1;
    //char c2;
    //char c3;
    //char c4;
    uint16_t header_size;
    uint16_t header_size1;
    uint16_t format;
    uint16_t nbr_track;
    uint16_t division;
}Header;

int main(){
    FILE* fp = fopen("test.mid", "r");
    if( fp == NULL )
        exit(1);

    char buff_in[100] = {0};
    Header h;
    fread(&h, sizeof(Header), 1, fp);
    //fread(&h, 1, sizeof(Header), fp);
    //fread(buff_in, 8, 1, fp);
    //fread(buff_in, 4, 1, fp);

    fclose(fp);
}