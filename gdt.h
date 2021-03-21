#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>
#include "descriptors.h"

typedef struct  gdtr
{
    uint16_t limit;
    uint32_t baseAddress;
} __attribute__ ((__packed__)) gdtr;

typedef segmentDescriptor_t gdt[4];

void initGDT(void);

#endif // _GDT_H_