#include "FileLoader.h"

FileLoader::FileLoader(const std::string &filename) {
    filecontents = "";
    this->readfile(filename);
}

void FileLoader::readfile(const std::string &filename) {

    char byte = 0;

    std::ifstream input_file(filename);
    /// check if the file opens
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '" << filename << "'" << std::endl;
        return;
    }

    /// while the file is open read contents
    while (input_file.get(byte)) {
        filecontents.push_back(byte);
    }

    input_file.close();
}

std::string FileLoader::getFilecontents(const std::vector<char> &exceptions) const {
    // Traverse the string
    std::string filecontents_ = filecontents;
    for (int i = 0; i < exceptions.size(); i++) {
        //remove exceptions from string
        filecontents_.erase(remove(filecontents_.begin(), filecontents_.end(), exceptions[i]), filecontents_.end());
    }

    return filecontents_;
}
