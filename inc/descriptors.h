#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

#include <stdint.h>

typedef enum
{
    SEG_DATA_RD = 0x00, // Read-Only
    SEG_DATA_RDA,       // Read-Only, accessed
    SEG_DATA_RDWR,      // Read/Write
    SEG_DATA_RDWRA,     // Read/Write, accessed
    SEG_DATA_RDEXPD,    // Read-Only, expand-down
    SEG_DATA_RDEXPDA,   // Read-Only, expand-down, accessed
    SEG_DATA_RDWREXPD,  // Read/Write, expand-down
    SEG_DATA_RDWREXPDA, // Read/Write, expand-down, accessed
    SEG_CODE_EX,        // Execute-Only
    SEG_CODE_EXA,       // Execute-Only, accessed
    SEG_CODE_EXRD,      // Execute/Read
    SEG_CODE_EXRDA,     // Execute/Read, accessed
    SEG_CODE_EXC,       // Execute-Only, conforming
    SEG_CODE_EXCA,      // Execute-Only, conforming, accessed
    SEG_CODE_EXRDC,     // Execute/Read, conforming
    SEG_CODE_EXRDCA     // Execute/Read, conforming, accessed
} segmentType_e;

typedef enum
{
    RING0 = 0,
    RING1,
    RING2,
    RING3
} privilegeLevels_e;

typedef enum
{
    SYS_SEG = 0,
    CODE_DATA_SEG
} descriptorType_e;

typedef enum
{
    NOT_PRESENT = 0,
    PRESENT
} segmentPresent_e;

typedef struct
{
    union
    {
        struct
        {
            uint16_t segmentLimit_00_15;
            uint16_t baseAddress_00_15;
            uint8_t baseAddress_16_23;
            uint8_t type : 4; // segment/gate type
            uint8_t S : 1;    // descriptor type
            uint8_t DPL : 2;  // descriptorPrivilegeLevel
            uint8_t P : 1;    // segmentPresent
            uint8_t segmentLimit_16_19 : 4;
            uint8_t AVL : 1; // availability for use
            uint8_t L : 1;   // 64bit code segment
            uint8_t DB : 1;
            uint8_t G : 1; // granularity
            uint8_t baseAddress_24_31;
        };
        uint64_t value;
    };

} segmentDescriptor_t;

// TODO: make limit 32 bit in simplified form + no granularity, have the function handle that
typedef struct
{

    uint32_t baseAddress;
    uint32_t segmentLimit : 20;
    segmentType_e type;    // segment/gate type
    descriptorType_e S;    // descriptor type
    privilegeLevels_e DPL; // descriptorPrivilegeLevel
    segmentPresent_e P;    // segmentPresent
    uint8_t AVL : 1;       // availability for use
    uint8_t L : 1;         // 64bit code segment
    uint8_t DB : 1;
    uint8_t G : 1; // granularity

} segmentDescriptorSimplified_t;

#endif // _DESCRIPTORS_H_