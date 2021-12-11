
#ifndef MB_FILELOADER_H
#define MB_FILELOADER_H

#include <iostream>
#include <fstream>
#include <vector>


class FileLoader {

public:
    /// constructor
    FileLoader(const std::string &filename);

    /// returnes the content of the file (type: std::basic_string<char>)
    const std::string &getFilecontents() const;

private:

    /// reads the contens of the file
    void readfile(const std::string &filename);

    std::string filecontents;

};


#endif //MB_FILELOADER_H
