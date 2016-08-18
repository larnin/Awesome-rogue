#ifndef FILEINFO_H
#define FILEINFO_H

#include <vector>
#include <string>

class FileInfo
{
public:
    static std::vector<std::string> filesIn(const std::string & dir, const std::string & extension);

private:
    FileInfo() = delete;
};

#endif // FILEINFO_H
