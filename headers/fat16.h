#ifndef FAT_FAT16_H
#define FAT_FAT16_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "custom_types.h"
#include "fat16.h"

using std::string;

class fat16 {
    string path;
    FILE *fd{};
    struct boot_info boot{};
    std::vector<struct fat_entry> files{};

public:
    explicit fat16(const string &path_);

    bool set_fd();

    FILE *get_fd();

    struct boot_info &get_boot_info();

    void read_filesystem();

    void print_filesystem_info();

    static void print_file_info(fat_entry *entry);

    void read_files();

    ~fat16();
};

#endif //FAT_FAT16_H
