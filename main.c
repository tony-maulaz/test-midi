#include "stdio.h"
#include "stdlib.h"
#include <stdint.h>

uint16_t read_16(FILE* f){
    char buff[2];
    fread(buff, 2, 1, f);
    uint32_t val = buff[1];
    val += buff[0]<<8;
    return val;
}

uint32_t read_32(FILE* f){
    char buff[4];
    fread(buff, 4, 1, f);
    uint32_t val = buff[3];
    val += buff[2]<<8;
    val += buff[1]<<16;
    val += buff[0]<<24;
    return val;
}

#pragma pack(1)
typedef struct {
    char mthd[4];
    uint32_t header_size;
    uint16_t format;
    uint16_t nbr_track;
    uint16_t division;
}Header;

typedef struct{
    char mtrk[4];
    uint32_t length;

    MetaEvent metaEvent[10];
    MidiMessage midiMsg[50];
}Track;

// Only FF 51 is decode

typedef struct{
    uint32_t v_time;
    char Type;
    uint32_t length;
}MetaEvent;

typedef struct{
    uint32_t v_time;
    char Status;
    char bytes[2];
}MidiMessage;

void extract_header(FILE* f, Header* h){
    char buff[100];
    
    // mthd
    fread(h->mthd, 4, 1, f);
    h->header_size = read_32(f);
    h->format = read_16(f);
    h->nbr_track = read_16(f);
    h->division = read_16(f);
}

int main(){
    FILE* fp = fopen("test.mid", "r");
    if( fp == NULL )
        exit(1);

    Header h;

    extract_header(fp, &h);    

    fclose(fp);
}