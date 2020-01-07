#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "../headers/custom_types.h"

using std::string;

#include "../headers/fat16.h"

fat16::fat16(const string &path_) {
    path = path_;
}

bool fat16::set_fd() {
    return fd = fopen(path.c_str(), "r");
}

FILE *fat16::get_fd() {
    return fd;
}

struct boot_info &fat16::get_boot_info() {
    return boot;
}

void fat16::read_filesystem() {
    if (!set_fd())
        throw std::runtime_error("Error opening filesystem");

    if (!fread(&get_boot_info(), sizeof(get_boot_info()), 1, get_fd()))
        throw std::runtime_error("Error opening filesystem");
}

void fat16::print_filesystem_info() {
    std::stringstream sign;
    sign << std::hex << (int) boot.sector_signature[1] << (int) boot.sector_signature[0];

    std::cout << "\nSize of sector:                        " << boot.bytes_per_sector << std::endl;
    std::cout << "Number of sectors in cluster:          " << (int) boot.sectors_per_cluster << std::endl;
    std::cout << "Number of FAT copies:                  " << (int) boot.fat_number << std::endl;
    std::cout << "Size of FAT copy in sectors, bytes:    " << (int) boot.sectors_per_fat << ", "
              << boot.sectors_per_fat * boot.bytes_per_sector << std::endl;
    std::cout << "Number of entries in root catalog:     " << (int) boot.max_number_of_files << std::endl;
    std::cout << "Size root catalog (bytes)              " << (int) boot.max_number_of_files * 32 << std::endl;
    std::cout << "The correct signature in the end:      " << (sign.str() == "aa55" ? "true" : "false")
              << std::endl;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

void fat16::print_file_info(fat_entry *entry) {
    switch (entry->name[0]) {
        case 0x00:
            return; // unused entry
        case 0xE5:
            printf("Deleted file: [?%.7s.%.3s]\n", entry->name + 1, entry->extension);
            return;
        default:
            if (!(entry->attributes & 0x10u))
                printf("File: %.8s.%.3s\n", entry->name, entry->extension);
            else
                printf("Directory: %.8s\n", entry->name);
    }

    std::cout << "\tSize:                 " << entry->size << std::endl;
    std::cout << "\tFile attributes: " << std::endl;
    std::cout << "\t\tRead-only:        \t" << ((entry->attributes & 0x01u) ? "true" : "false") << std::endl;
    std::cout << "\t\tHidden file:      \t" << ((entry->attributes & 0x02u) ? "true" : "false") << std::endl;
    std::cout << "\t\tSystem file:      \t" << ((entry->attributes & 0x04u) ? "true" : "false") << std::endl;
    std::cout << "\t\tVolume label:     \t" << ((entry->attributes & 0x08u) ? "true" : "false") << std::endl;
    std::cout << "\t\tLong file name:   \t" << ((entry->attributes & 0x0fu) ? "true" : "false") << std::endl;
    std::cout << "\t\tDirectory:        \t" << ((entry->attributes & 0x10u) ? "true" : "false") << std::endl;
    std::cout << "\t\tArchive:          \t" << ((entry->attributes & 0x20u) ? "true" : "false") << std::endl;
    std::cout << "\tFirst cluster number: " << entry->start_cluster << std::endl;
    printf("\tModifying time:       %04d-%02d-%02d %02d:%02d.%02d\n",
           1980 + (entry->modify_date >> 9), (entry->modify_date >> 5) & 0xF, entry->modify_date & 0x1F,
           (entry->modify_time >> 11), (entry->modify_time >> 5) & 0x3F, entry->modify_time & 0x1F);
    printf("\tCreation time:        %04d-%02d-%02d %02d:%02d.%02d\n",
           1980 + (entry->create_date >> 9), (entry->create_date >> 5) & 0xF, entry->create_date & 0x1F,
           (entry->create_time >> 11), (entry->create_time >> 5) & 0x3F, entry->create_time & 0x1F);
}

#pragma clang diagnostic pop

void fat16::read_files() {
    fat_entry entry{};

    fseek(fd, (boot.size_of_reserved - 1 + boot.sectors_per_fat * boot.fat_number) * boot.bytes_per_sector,
          SEEK_CUR);
    std::cout << std::endl << std::endl << boot.max_number_of_files << std::endl;
    for (int i = 0; i < boot.max_number_of_files; ++i) {
        fread(&entry, sizeof(entry), 1, fd);
        print_file_info(&entry);
    }
}

fat16::~fat16() {
    if (fd)
        fclose(fd);
};