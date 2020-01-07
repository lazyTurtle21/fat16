#ifndef FAT_CUSTOM_TYPES_H
#define FAT_CUSTOM_TYPES_H

#include <cstdint>

struct boot_info {
    unsigned char unused1[3];
    unsigned char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t size_of_reserved;
    uint8_t fat_number;
    uint16_t max_number_of_files;
    unsigned char unused2[3];
    uint16_t sectors_per_fat;
    unsigned char unused3[486];
    unsigned char sector_signature[2];
} __attribute__((packed));


struct fat_entry {
    unsigned char name[8];
    unsigned char extension[3];
    unsigned char attributes;
    unsigned char nt;
    uint8_t create_time_fine;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t access_date;
    uint16_t ea_index;
    uint16_t modify_time;
    uint16_t modify_date;
    uint16_t start_cluster;
    uint32_t size;
} __attribute__((packed));

#endif //FAT_CUSTOM_TYPES_H
