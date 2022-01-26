
#ifndef MB_FILELOADER_H
#define MB_FILELOADER_H

#include <vector>
#include <string>


class FileLoader {

public:
    /// constructor
    explicit FileLoader(const std::string &filename);

    /// returnes the content of the file (type: std::basic_string<char>)
    std::string getFilecontents(const std::vector<char> &exeptions = {}) const;

private:

    /// reads the contens of the file
    void readfile(const std::string &filename);

    std::string filecontents;

};


#endif //MB_FILELOADER_H
