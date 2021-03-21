#include "gdt.h"
#include "descriptors.h"


gdt GDT;

static void simpliefiedToDescriptor(segmentDescriptorSimplified_t simplified, segmentDescriptor_t *descriptor)
{
    descriptor->segmentLimit_00_15 = (simplified.segmentLimit & 0xffff);
    descriptor->segmentLimit_16_19 = (simplified.segmentLimit >> 16) & 0xf;
    descriptor->baseAddress_00_15 = (simplified.baseAddress & 0xffff);
    descriptor->baseAddress_16_23 = (simplified.baseAddress >> 16) & 0xff;
    descriptor->baseAddress_24_31 = (simplified.baseAddress >> 24) & 0xff;
    descriptor->type = simplified.type;
    descriptor->S = simplified.S;
    descriptor->DPL = simplified.DPL;
    descriptor->P = simplified.P;
    descriptor->AVL = simplified.AVL;
    descriptor->L = simplified.L;
    descriptor->DB = simplified.DB;
    descriptor->G = simplified.G;
}

static void fillGDT(void)
{
    segmentDescriptorSimplified_t simplified;
    segmentDescriptor_t descriptor;

    /* Null descriptor */
    GDT[0] = (segmentDescriptor_t){0};

    /* Code segment */
    simplified = (segmentDescriptorSimplified_t){.baseAddress = 0, .segmentLimit = 0xfffff, .G=1, .DB=1, .type = SEG_CODE_EXRD, .P = PRESENT, .DPL = RING0, .S = CODE_DATA_SEG};
    simpliefiedToDescriptor(simplified, &descriptor);
    GDT[1] = descriptor;

    /* data segment */
    simplified = (segmentDescriptorSimplified_t){.baseAddress = 0, .segmentLimit = 0xfffff, .G=1, .DB=1, .type = SEG_DATA_RDWR, .P = PRESENT, .DPL = RING0, .S = CODE_DATA_SEG};
    simpliefiedToDescriptor(simplified, &descriptor);
    GDT[2] = descriptor;

    /* tss */
    GDT[3] = (segmentDescriptor_t){0}; // tss
}

static void loadGDT(void)
{
    gdtr gdtPointer;
    gdtPointer.baseAddress = GDT;
    gdtPointer.limit = 64*4;//sizeof(segmentDescriptor_t) * 4 - 1;
    asm volatile("lgdt %0"
                 :
                 : "m"(gdtPointer));
}

static void reloadSegmentRegisters(void)
{
    asm volatile(".intel_syntax noprefix;"
        "cli;"
        
        "mov ax, 0x10;"    // 0x10 points at the new data selector
        "mov ds, eax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "mov ss, ax;"
        "jmp 0x08:reloadCS;"
        "reloadCS:"
        ".att_syntax;");
}

void initGDT(void)
{
    fillGDT();
    loadGDT();
    reloadSegmentRegisters();
}