#include "CrossLinuxWindows.h"
#include "FileUtil.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#ifdef BUILD_WIN
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <cstring>
const int NUM4 = 4;
const int NUM8 = 8;
const int NUM10 = 10;
#endif

void CopyFile(std::string source, std::string dest) {
	const int flushNum = 128;
	std::ifstream in(source, std::ios::binary);
	std::ofstream out(dest, std::ios::binary);
	if (!in) {
		printf("open file error");
		return;
	}
	if (!out) {
		printf("open file error");
		return;
	}
	char flush[flushNum];
	while (!in.eof()) {
		in.read(flush, flushNum);
		out.write(flush, in.gcount());
	}
	in.close();
	out.close();
}

void FileWrite(std::string filePath, std::string cnt)
{
	std::ofstream fout;
	fout.open(filePath);
	fout << cnt.c_str() << std::endl;
	fout.close();
}

/*int CopyDir(std::string pSrc, std::string pDes)
{
	_mkdir(pDes.c_str());
	std::string dir = "";
	std::string str = "\\*.*";
	dir += pSrc;
	dir += str;
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(dir.c_str(), &fileinfo)) != -1)
	{
		do
		{
			std::string srcFileName = pSrc + "\\" + (fileinfo.name);
			std::string desFileName = pDes + "\\" + (fileinfo.name);

			bool isSubDir = false;
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				CopyFile(srcFileName, desFileName);
			}
			else
			{
				isSubDir = true;
			}
			if (isSubDir && strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
			{
				CopyDir(srcFileName, desFileName);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
		return 1;
	}
	return 0;
} */

void ListFiles(std::string cateDir, std::vector<std::string>& files)
{
#ifdef BUILD_WIN
	_finddata_t file;
	long lf;
	if ((lf = _findfirst(cateDir.c_str(), &file)) == -1) {
	}
	else {
		while (_findnext(lf, &file) == 0) {
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(file.name);
		}
	}
	_findclose(lf);
#else
	DIR* dir = nullptr;
	struct dirent* ptr = nullptr;

	if ((dir = opendir(cateDir.c_str())) == nullptr)
	{
		exit(1);
	}

	while ((ptr = readdir(dir)) != nullptr)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
			continue;
		else if (ptr->d_type == NUM8)    ///file
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
			files.push_back(ptr->d_name);
		else if (ptr->d_type == NUM10)    ///link file
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);
			continue;
		else if (ptr->d_type == NUM4)    ///dir
		{
			files.push_back(ptr->d_name);
		}
	}
	closedir(dir);
#endif
	// sort from small to big
//	sort(files.begin(), files.end());
//	return files;
}




