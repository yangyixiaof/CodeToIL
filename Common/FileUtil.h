#pragma once
#include <string>
#include <vector>

void CopyFile(std::string source, std::string dest);

void FileWrite(std::string filePath, std::string cnt);

void ListFiles(std::string cateDir, std::vector<std::string>& files);





