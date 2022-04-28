#include <stdbool.h>
#include <stdint.h>

#include "stdio.h"
#include "stdlib.h"
 
// note 60 C4
// Do maj : 60 64 67 72 : do mi sol do
// Sol maj : 67 71 74 79 : sol si re sol 
 
uint16_t read_16(FILE* f)
{
    char buff[2];
    fread(buff, 2, 1, f);
    uint32_t val = buff[1];
    val += buff[0] << 8;
    return val;
}

uint32_t read_32(FILE* f)
{
    char buff[4];
    fread(buff, 4, 1, f);
    uint32_t val = buff[3];
    val += buff[2] << 8;
    val += buff[1] << 16;
    val += buff[0] << 24;
    return val;
}

uint32_t read_vlv(FILE* f)
{
    uint32_t value = 0;
    char v;
    do
    {
        fread(&v, 1, 1, f);
        value = (value << 7) | (v & 0x7F);
        bool msb = v & 0x80;
    } while ((v & 0x80) == 0x80);
}

#pragma pack(1)
typedef struct
{
    char mthd[4];
    uint32_t header_size;
    uint16_t format;
    uint16_t nbr_track;
    uint16_t division;
} Header;

typedef struct
{
    uint32_t v_time;
    char type;
    uint32_t length;
} MetaEvent;

typedef struct
{
    uint32_t v_time;
    char status;
    char bytes[2];
} MidiMessage;

typedef struct
{
    char mtrk[4];
    uint32_t length;

    int nbr_meta;
    MetaEvent metaEvent[10];

    int nbr_midi;
    MidiMessage midiMsg[50];
} Track;

void init_track(Track* t)
{
    t->length = 0;
    t->nbr_meta = 0;
    t->nbr_midi = 0;
}

void extract_meta_event(FILE* f, MetaEvent* m)
{
    fread(&(m->type), 1, 1, f);
    m->length = read_vlv(f);
    char buf[100];
    fread(buf, 1, m->length, f);
}

void extract_midi(FILE* f, MidiMessage* m)
{
    uint8_t status = (m->status >> 4) & 0xF;
    switch ((m->status >> 4) & 0xF)
    {
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
            fread(m->bytes, 1, 2, f);
            break;

        case 0xC:
        case 0xD:
        case 0xE:
            fread(m->bytes, 1, 1, f);
            break;

        default:
            fread(m->bytes, 1, 1, f);
            break;
    }
}

void extract_header(FILE* f, Header* h)
{
    char buff[100];

    // mthd
    fread(h->mthd, 4, 1, f);
    h->header_size = read_32(f);
    h->format = read_16(f);
    h->nbr_track = read_16(f);
    h->division = read_16(f);
}

bool extract_event(FILE* f, Track* t)
{
    long pos_start = ftell(f);
    long pos = pos_start;

    fread(t->mtrk, 4, 1, f);
    t->length = read_32(f);

    do
    {
        uint32_t time = read_vlv(f);  // read vlv : time of event
        uint8_t type;
        fread(&type, 1, 1, f);  // read event type

        switch (type)
        {
            case 0xFF:  // Meta event
                t->metaEvent[t->nbr_meta].v_time = time;
                extract_meta_event(f, &(t->metaEvent[t->nbr_meta]));
                t->nbr_meta++;
                break;

            case 0xF0:  // System exclusive event
                break;

            default:  // midi message
                t->midiMsg[t->nbr_midi].v_time = time;
                // check if new status or same as last status
                if ((type & 0x80) == 0x80)
                {
                    t->midiMsg[t->nbr_midi].status = type;
                }
                else
                {
                    t->midiMsg[t->nbr_midi].status = t->midiMsg[t->nbr_midi - 1].status;
                    fseek(f, -1, SEEK_CUR); // no need to read status
                }
                extract_midi(f, &(t->midiMsg[t->nbr_midi]));
                t->nbr_midi++;
        }

        pos = ftell(f) - pos_start;
    } while (pos < t->length);

    return true;
}

int main()
{
    //FILE* fp = fopen("test.mid", "r");
    FILE* fp = fopen("sol_maj.mid", "r");
    if (fp == NULL) exit(1);

    Header h;
    Track t;
    init_track(&t);

    extract_header(fp, &h);
    extract_event(fp, &t);

    // print event note
    for(int i=0; i<t.nbr_midi; i++){
        if( (t.midiMsg[i].status & 0xF0) == 0x80 ){
            uint16_t note = t.midiMsg[i].bytes[0];
            //printf("Event time : %d - Note : %d - vel : %d OFF\n", t.midiMsg[i].v_time, note, t.midiMsg[i].bytes[1]);
        }
        else if( (t.midiMsg[i].status & 0xF0) == 0x90 && t.midiMsg[i].bytes[1] != 0){
            uint16_t note = t.midiMsg[i].bytes[0];
            printf("Event time : %d - Note : %d ON\n", t.midiMsg[i].v_time, note );
        }
    }
    
    fclose(fp);
}