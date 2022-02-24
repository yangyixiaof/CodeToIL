#pragma once

#include <string>
#include <vector>
#include <iostream>

int stringStartsWith(std::string s, std::string sub);

int stringEndsWith(std::string s, std::string sub);

std::vector<std::string> stringSplit(const std::string& s, const std::string& seperator);

std::string stringTrim(std::string s);

std::vector<std::string> stringTrimList(std::vector<std::string> strList);

std::string stringFillBlank(std::string s, int length, bool left = false);

int stringToInt(std::string str);

std::string codeIndentProcess(std::string code);

std::string codeTrimAllLines(std::string code);

std::string codeRemoveComment(std::string code);

int stringSubStrCount(const std::string& str, const std::string& sub);

std::string stringGetBlanks(int count);

void getFullPermutation(std::vector<int> dimension, std::vector<std::vector<int>>& ret);

bool isFolderExist(std::string folder);

bool isFileExist(std::string file);

bool createFolder(std::string folder);

std::string getExeDirPath();

std::string getExeFilePath();

std::string getParentPath(std::string path, int parentCount = 1);

void SetCurrentDirPath(std::string path);

std::string readFile(std::string path);
void writeFile(std::string path, std::string content);

std::string stringToLower(std::string str);
std::string stringToUpper(std::string str);


void debugLog(std::string str, int width, bool pure);
void debugLog(std::string str, bool pure);
void debugLog(std::string str);

void log(std::string str, int width, bool pure);
void log(std::string str, bool pure);
void log(std::string str);


std::string stringRepeat(std::string s, int count);

std::string stringReplaceAll(std::string s, std::string src, std::string dst);

int systemCallWithTimeOut(std::string exeFile, std::string parameter, int time, std::string redirectOutputFile);