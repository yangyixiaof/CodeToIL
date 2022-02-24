#include "Utility.h"
#ifdef BUILD_LINUX
#include <unistd.h>
#include <sys/stat.h>
#else
#include <io.h> //_access fun
#include <direct.h> //_mkdir fun
#include <windows.h>
#endif
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

namespace
{
    const int CONST_NUM_2 = 2;
    const int CONST_NUM_4 = 4;
    const int CONST_NUM_7 = 7;
    const int CONST_NUM_100 = 100;
    const int CONST_NUM_1000 = 1000;
}

string stringToLower(string str)
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

string stringToUpper(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

int stringStartsWith(string s, string sub) {
	return s.find(sub) == 0 ? 1 : 0;
}

int stringEndsWith(string s, string sub) {
	return s.rfind(sub) == (s.length() - sub.length()) ? 1 : 0;
}

string stringTrim(string s)
{
	if (s.empty())
	{
		return s;
	}
	int start = 0;
	int end = s.length() - 1;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '\n' || s[i] == '\r' || s[i] == ' ' || s[i] == '\t') start++;
		else break;

	}
	for (int i = s.length() - 1; i >= 0; i--)
	{
		if (s[i] == '\n' || s[i] == '\r' || s[i] == ' ' || s[i] == '\t') end--;
		else break;
	}
	s = s.substr(start, end - start + 1);
	return s;
}

std::vector<std::string> stringTrimList(std::vector<std::string> strList)
{
    int offset = 0;
    for(int i = 0; i < strList.size() + offset; i++)
    {
        strList[i - offset] = stringTrim(strList[i - offset]);
        if(strList[i - offset].empty())
        {
            strList.erase(strList.begin() + i - offset);
            offset++;
        }
    }
    return strList;
}

vector<string> stringSplit(const string& s, const string& seperator) {
	vector<string> result;
	if (s.empty())
		return result;
	if (seperator.empty())
	{
		result.push_back(s);
		return result;
	}
	size_t start = 0;
	size_t end = s.find(seperator);
	while (end != s.npos)
	{
		result.push_back(s.substr(start, end - start));
		start = end + seperator.length();
		end = s.find(seperator, start);
	}
	result.push_back(s.substr(start, s.length() - start));
	return result;
}

string stringFillBlank(string s, int length, bool left)
{
	int l = s.length();
	if (l >= length)
		return s;
	string ret;
	for (int i = 0; i < length - l; i++)
		ret += " ";
    if(left)
        ret += s;
    else
        ret = s + ret;
	return ret;
}

int stringToInt(string str)
{
	int ret = atoi(str.c_str());
	return ret;
}


string codeIndentProcess(string code)
{
    code = codeTrimAllLines(code);
    int codeLength = code.length();
    bool isInBlockCmt = false;
    bool isInLineCmt = false;
    bool isInStr = false;
    string retCode;
    int curIndent = 0;
    for (int i = 0; i < codeLength;) {
        bool inComment = true;
		if (i + 1 < codeLength && code.substr(i,CONST_NUM_2) == "/*" && !isInBlockCmt && isInLineCmt) {
			isInBlockCmt = true;
		} else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "//" && !isInBlockCmt && isInLineCmt) {
			isInLineCmt = true;
		} else if (code[i] == '\"' && (!(i > 0 && code[i-1] == '\\')) && !isInStr && !isInBlockCmt && isInLineCmt) {
			isInStr = true;
            inComment = false;
		} else if (code[i] == '\n' && isInLineCmt) {
			isInLineCmt = false;
		} else if (i>0 && code.substr(i-1, CONST_NUM_2) == "*/" && isInBlockCmt) {
			isInBlockCmt = false;
		} else if (code[i] == '\"' && (!(i > 0 && code[i-1] == '\\')) && !isInStr && !isInBlockCmt && isInLineCmt) {
			isInStr = false;
            inComment = false;
		} else if (isInBlockCmt || isInLineCmt) {
        //} else if (isInString) {
        //    inComment = false;
        } else {
            inComment = false;
        }
        if(inComment) {
            retCode += stringGetBlanks(curIndent * CONST_NUM_4) + code[i];
            i++;
            continue;
        } else if(i > 0 && code[i - 1] == '\n' && 
            (i + CONST_NUM_4 <= codeLength && code.substr(i, CONST_NUM_4) == "case" || 
            i + CONST_NUM_7 <= codeLength && code.substr(i, CONST_NUM_7) == "default" ||
            code[i] == '}')) {
            retCode += stringGetBlanks(curIndent * CONST_NUM_4 - CONST_NUM_4) + code[i];
        } else if(i > 0 && code[i - 1] == '\n') {
            retCode += stringGetBlanks(curIndent * CONST_NUM_4) + code[i];
        } else {
            retCode += code[i];
        }
        if(code[i] == '}' && !isInStr)
            curIndent--;
        else if(code[i] == '{' && !isInStr)
            curIndent++;
        
        i++;
	}
	return retCode;
}

std::string codeTrimAllLines(std::string code)
{
	vector<string> lines = stringSplit(code, "\n");
	code = "";
    for (int i = 0;i < lines.size(); i++) {
        lines[i] = stringTrim(lines[i]);
        if(lines[i].empty())
            continue;
        code += lines[i] + "\n";
    }
    return code;
}

std::string codeRemoveComment(std::string code)
{
    int i = 0;
    int codeLength = code.length();
    bool isInBlockComment = false;
    bool isInLineComment = false;
    bool isInString = false;
    string retCode;
    while (i < codeLength) {
        bool inComment = true;
		if (i + 1 < codeLength && code.substr(i,CONST_NUM_2) == "/*" && isInBlockComment == false && isInLineComment == false) {
			isInBlockComment = true;
		} else if (i + 1 < codeLength && code.substr(i, CONST_NUM_2) == "//" && isInBlockComment == false && isInLineComment == false) {
			isInLineComment = true;
		} else if (code[i] == '\"' && i > 0 && code[i-1] != '\\' && isInString == false && isInBlockComment == false && isInLineComment == false) {
			isInString = true;
            inComment = false;
		} else if (code[i] == '\n' && isInLineComment) {
			isInLineComment = false;
		} else if (i>0 && code.substr(i-1, CONST_NUM_2) == "*/" && isInBlockComment) {
			isInBlockComment = false;
		} else if (code[i] == '\"' && i > 0 && code[i-1] != '\\' && isInString == true && isInBlockComment == false && isInLineComment == false) {
			isInString = false;
            inComment = false;
		} else if (isInBlockComment || isInLineComment) {
        } else if (isInString) {
            inComment = false;
        } else {
            inComment = false;
        }
        if(inComment)
        {
            i++;
            continue;
        }
        retCode += code[i];
        i++;
	}
    
	return retCode;
}

int stringSubStrCount(const std::string& str, const std::string& sub)
{
	int num = 0;
	size_t len = sub.length();
	if (len == 0)len = 1;
	for (size_t i = 0; (i = str.find(sub, i)) != std::string::npos; i += len) {
	    num++;
	}
	return num;
}

string stringGetBlanks(int count)
{
	string ret = "";
	for (int i = 0;i < count;i++)
		ret += " ";
	return ret;
}

void getFullPermutation(vector<int> dimension, vector<vector<int>>& ret)
{
	//   getFullPermutation  Test
	//vector<int> dimension;
	//dimension.push_back(2);
	//dimension.push_back(3);
	//dimension.push_back(4);
	//vector<vector<int>> ret;
	//getFullPermutation(dimension, ret);
	//for(int i = 0;i < ret.size();i++)
	//{
	//	for(int j = 0; j < ret[i].size();j++)
	//	{
	//		cout << ret[i][j];
	//	}
	//	cout << "\n";
	//}

    int sum = 1;
	for(int i = 0; i<dimension.size();i++)
	{
		sum *= dimension[i];
	}
	vector<int> element;
	for(int i = 0; i < dimension.size(); i++)
	{
		element.push_back(0);
	}
	for(int i = 0; i < sum; i++)
	{
		ret.push_back(element);
		bool plusOne = true;
		for(int j = dimension.size() - 1; j >=0; j--)
		{
			if(plusOne)
				element[j]++;
			plusOne = false;
			if(element[j] >= dimension[j])
			{
				plusOne = true;
				element[j] = 0;
			}
			else
			{
				break;
			}
		}
	}
}

bool isFolderExist(string folder)
{
#ifdef BUILD_LINUX
    int ret = access(folder.c_str(), 0);
#else
    int ret = _access(folder.c_str(), 0);
#endif
	if (ret == 0)
		ret = true;
	else
		ret = false;

	return ret;
}

bool isFileExist(string file)
{
#ifdef BUILD_LINUX
    int ret = access(file.c_str(), 0);
#else
    int ret = _access(file.c_str(), 0);
#endif
	if (ret == 0)
		return true;
	else
		return false;
}

bool createFolder(string folder)
{
#ifdef BUILD_LINUX
    int ret = mkdir(folder.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
#else
    int ret = _mkdir(folder.c_str());
#endif
    
	if (ret == 0)
		return true;
	else
		return false;
}
namespace
{
    const int FILE_PATH_STR_BUF_LEN = 2048;
}
string getExeDirPath()
{
    char szPath[FILE_PATH_STR_BUF_LEN] = {0};
#ifdef BUILD_LINUX
    readlink("/proc/self/exe", szPath, FILE_PATH_STR_BUF_LEN - 1);
#else
    GetModuleFileNameA(nullptr, szPath, sizeof(szPath) - 1);
#endif
    szPath[FILE_PATH_STR_BUF_LEN - 1] = '\0';
	int length = strlen(szPath);
	if(length <= 0 || length >= FILE_PATH_STR_BUF_LEN)
		return "";
	length--;
	while(length>0 && szPath[length] != '\\' && szPath[length] != '/')
	{
		szPath[length] = 0;
		length--;
	}
	if(szPath[length] == '\\' || szPath[length] == '/')
		szPath[length] = 0;

	string ret(szPath);

	return ret;
}

string getExeFilePath()
{
    char szPath[FILE_PATH_STR_BUF_LEN] = {0};
#ifdef BUILD_LINUX
    readlink("/proc/self/exe", szPath, FILE_PATH_STR_BUF_LEN - 1);
#else
    GetModuleFileNameA(nullptr, szPath, sizeof(szPath) - 1);
#endif
    szPath[FILE_PATH_STR_BUF_LEN - 1] = '\0';
	string ret(szPath);
	return ret;
}

string getParentPath(string path, int parentCount)
{
	string ret = path;
	for(int i = 0; i < parentCount; i++)
	{
		size_t offset1 = ret.rfind("\\");
		size_t offset2 = ret.rfind("/");
		if(offset1 != string::npos)
		{
			ret = ret.substr(0, offset1);
		}
		else if(offset2 != string::npos)
		{
			ret = ret.substr(0, offset2);
		}
	}

	return ret;
}

void SetCurrentDirPath(string path)
{
#ifdef BUILD_LINUX
    chdir(path.c_str());
#else
    SetCurrentDirectoryA(path.c_str());
#endif
}

string readFile(string path)
{
	ifstream file;
	file.open(path, ios::in);
	string lineContext;

	if (!file)
	{
	    file.close();
		return "";
	}

	string fileContext;
	while (file && !file.eof())
	{
		getline(file, lineContext);
		fileContext += lineContext;

        if(!file.eof())
		    fileContext += "\n";
	}
    file.close();
	return fileContext;
}

void writeFile(string path, string content)
{
    ofstream file;
    file.open(path, ios::out);
    file << content;
    file.flush();
    file.close();
}


void errorLog(string str)
{
	std::cerr << "Error: " << str << endl;
}

void debugLog(string str, int width, bool pure)
{
	//return;
	if (str == "\n") {
		cout << endl;
		return;
	}
	if (!pure)
		cout << "Debug: ";
	if (width == 0)
	{
		cout.width(width);
		cout.setf(std::ios::left);
		cout << str << endl;
		return;
	}
	vector<string> sp = stringSplit(str, "\t");
	for (int i = 0;i < sp.size();i++)
	{
		cout.width(width);
		cout.setf(std::ios::left);
		cout << sp[i];
	}
	cout << endl;

}
void debugLog(string str, bool pure)
{
	debugLog(str, 0, pure);
}

void debugLog(string str)
{
	debugLog(str, 0, false);
}

void log(string str, int width, bool pure)
{
	if (str == "\n") {
		cout << endl;
		return;
	}
	if (!pure)
		cout << "Debug: ";
	if (width == 0)
	{
		cout.width(width);
		cout.setf(std::ios::left);
		cout << str << endl;
		return;
	}
	vector<string> sp = stringSplit(str, "\t");
	for (int i = 0;i < sp.size();i++)
	{
		cout.width(width);
		cout.setf(std::ios::left);
		cout << sp[i];
	}
	cout << endl;
}
void log(string str, bool pure)
{
	log(str, 0, pure);
}

void log(string str)
{
	log(str, 0, false);
}

std::string stringRepeat(std::string s, int count)
{
    string ret;
    for(int i = 0; i < count; i++)
    {
        ret += s;
    }
    return ret;
}

std::string stringReplaceAll(std::string s, std::string src, std::string dst)
{
    size_t pos = s.find(src);
    while(pos != string::npos)
    {
        s = s.replace(pos, src.length(), dst);
        pos = s.find(src, pos + dst.length());
    }
    return s;
}

int systemCallWithTimeOut(string exeFile, string parameter, int time, string redirectOutputFile)
{
#ifdef BUILD_LINUX
    string t = to_string(time / CONST_NUM_1000);
    string cmd = "timeout " + t + " " + exeFile + " " + parameter + " > " + redirectOutputFile;
    system(cmd.c_str());

#else
    parameter = " " + parameter;
    SECURITY_ATTRIBUTES secattr; 
    ZeroMemory(&secattr, sizeof(secattr));
    secattr.nLength = sizeof(secattr);
    secattr.bInheritHandle = TRUE;

    HANDLE rPipe;
    HANDLE wPipe;
    CreatePipe(&rPipe, &wPipe ,&secattr, 0);

    STARTUPINFOA sInfo; 
    ZeroMemory(&sInfo,sizeof(sInfo));
    PROCESS_INFORMATION pInfo; 
    ZeroMemory(&pInfo,sizeof(pInfo));
    sInfo.cb = sizeof(sInfo);
    sInfo.dwFlags = STARTF_USESTDHANDLES;
    sInfo.hStdInput = NULL; 
    sInfo.hStdOutput = wPipe; 
    sInfo.hStdError = NULL;

    //Create the process here.
    BOOL ret = CreateProcessA(exeFile.c_str(), const_cast<char*>(parameter.c_str()), 0, 0,
        TRUE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 0, 0, &sInfo, &pInfo);
    if (ret) {
        WaitForSingleObject(pInfo.hProcess, time);
        TerminateProcess(pInfo.hProcess, 0);
        CloseHandle(pInfo.hThread);
        CloseHandle(pInfo.hProcess);
    }
    CloseHandle(wPipe);
    
    char buf[CONST_NUM_100] = {0};
    DWORD reDword; 
    string csOutput;
    string csTemp;
    BOOL res;
    do
    {
        ZeroMemory(buf, CONST_NUM_100);
        res = ReadFile(rPipe, buf, CONST_NUM_100 - 1, &reDword, 0);
        csTemp = string(buf);
        csOutput += csTemp;
    }while(res);

    writeFile(redirectOutputFile, csOutput);
    return 1;
#endif
}
