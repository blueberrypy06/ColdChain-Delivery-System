#ifndef PROJECT2025112431_FILEHANDLER_H
#define PROJECT2025112431_FILEHANDLER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Package.h"

using namespace std;

class FileHandler
{
public:
    // 상태 저장
    // Package 포인터가 담긴 배열과 그 개수를 받아 CSV 파일로 저장
    static bool savePackages(string& fileName, Package** packages, int count);

    // 상태 불러오기
    // CSV 파일을 읽어 동적 배열을 생성하여 반환
    static Package** loadPackages(string& fileName, int& outCount, int maxPackages);
};


#endif //PROJECT2025112431_FILEHANDLER_H
