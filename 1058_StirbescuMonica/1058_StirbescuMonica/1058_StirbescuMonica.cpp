#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <strsafe.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <windows.h>
#include<shellapi.h>
#include<conio.h>

void displayTime(const std::string name, const FILETIME* filetime)
{
    SYSTEMTIME        stSystemTime;
    if (FileTimeToSystemTime(filetime, &stSystemTime))
    {
        std::cout << name << " Year = " << stSystemTime.wYear << " Month = " << stSystemTime.wMonth << " Day = " << stSystemTime.wDay
            << " Hour = " << stSystemTime.wHour << " Minute = " << stSystemTime.wMinute << " Second = " << stSystemTime.wSecond << std::endl;
    }
    else
    {
        std::cout << "Cannot convert " << name << " time!";
    }

}

void printSize(DWORD size)
{
    std::cout << "FileSize: " << size << "bytes" << std::endl;
}

void displayAttributes(DWORD fileAttributes)
{

    std::cout << "FILE_ATTRIBUTE_READONLY: " << ((fileAttributes & FILE_ATTRIBUTE_READONLY) ? "TRUE" : "FALSE") << std::endl;
    std::cout << "FILE_ATTRIBUTE_HIDDEN: " << ((fileAttributes & FILE_ATTRIBUTE_HIDDEN) ? "TRUE" : "FALSE") << std::endl;
    std::cout << "FILE_ATTRIBUTE_COMPRESSED: " << ((fileAttributes & FILE_ATTRIBUTE_COMPRESSED) ? "TRUE" : "FALSE") << std::endl;
}

int main()
{
    LPCWSTR strFile = L"1058_StirbescuMonica.lnk";
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;

    if (GetFileAttributesEx(strFile, GetFileExInfoStandard, &fileInfo))
    {

        displayTime("CreationTime:", &fileInfo.ftCreationTime);
        displayTime("AccessTime:", &fileInfo.ftLastAccessTime);
        displayTime("WriteTime:", &fileInfo.ftLastWriteTime);
        printSize(fileInfo.nFileSizeLow);
        displayAttributes(fileInfo.dwFileAttributes);
    }
    else
    {
        std::cout << "Cannot get file attributes!";
    }

	return 0;
}