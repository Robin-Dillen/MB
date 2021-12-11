#include "FileLoader.h"

FileLoader::FileLoader(const std::string &filename) {
    filecontents = "";
    this->readfile(filename);
}

void FileLoader::readfile(const std::string &filename){


    char byte = 0;

    std::ifstream input_file(filename);
    /// check if the file opens
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '" << filename << "'" << std::endl;
        return;
    }

    /// while the file is open read contents
    while (input_file.get(byte)) {
        if(byte == ' ' || byte == '\n'|| byte == '\t'|| byte == '\r'){
            continue;
        }
        filecontents.push_back(byte);
    }

    input_file.close();
}

const std::string &FileLoader::getFilecontents() const {
    return filecontents;
}
