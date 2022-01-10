#include "file.h"

#include <iostream>
#include <fstream>

namespace nFile
{
    const std::string DirectoryPath {"/home/alisa/tiny_engine2.0/glsl"};
}

using namespace te;

file::file(const std::string& aFileName)
    : fileName (aFileName)
    , absoluteFilePath (nFile::DirectoryPath + aFileName)
{}

void file::read()
{
    std::cout << absoluteFilePath << std::endl;
    std::ifstream fstream(absoluteFilePath);
    if(fstream.fail())
    {
        perror(absoluteFilePath.c_str());
    }

    std::string line;

    while(std::getline(fstream, line))
    {
        fileContext += line + "\n";

    }
    std::cout << fileContext << std::endl;
    fstream.close();
}

std::string file::get_context()
{
    return fileContext;
}

std::string file::get_absolute_file_path()
{
    return absoluteFilePath;
}

std::string file::get_file_name()
{
    return fileName;
}
