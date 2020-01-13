#include "headers/fat16.h"

int main(int argc, char ** argv) {
    if (argc != 2){
        std::cerr << "How to run: fat <fat16 name>\n";
        return -1;
    }
    fat16 fs{argv[1]};
    fs.read_filesystem();
    fs.print_filesystem_info();
    fs.read_files();
    return 0;
}