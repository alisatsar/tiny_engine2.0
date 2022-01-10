#pragma once

#include <string>

namespace te {
class file
{
public:
    file(const std::string& aFileName);
    void read();
    std::string get_context();
    std::string get_absolute_file_path();
    std::string get_file_name();
private:
    std::string absoluteFilePath;
    std::string fileName;
    std::string fileContext;
};
}
