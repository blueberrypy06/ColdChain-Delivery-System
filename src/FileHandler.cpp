#include "FileHandler.h"

bool FileHandler::savePackages(string& fileName, Package** packages, int count)
{
    ofstream outFile(fileName);
    if (!outFile.is_open())
    {
        return false;   // 파일 열기 실패
    }

    for (int i = 0; i < count; ++i)
    {
        Package* p = packages[i];

        // 열거형은 문자열 대신 int로 형변환하여 저장
        outFile << p->getId() << ","
                << p->getName() << ","
                << static_cast<int>(p->getType()) << ","
                << p->getWeight() << ","
                << p->getBasePriority() << ","
                << static_cast<int>(p->getStatus()) << ","
                << p->getTtl() << "\n";
    }
    outFile.close();
    return true;
}

Package** FileHandler::loadPackages(string& fileName, int& outCount, int maxPackages)
{
    ifstream inFile(fileName);
    if (!inFile.is_open())
    {
        outCount = 0;
        return nullptr; // 파일 열기 실패
    }

    // 새로운 물류들을 담을 배열 동적 할당
    Package** loadedPackages = new Package*[maxPackages];
    for(int i = 0; i < maxPackages; i++) {
        loadedPackages[i] = nullptr;
    }
    outCount = 0;
    string line;

    // 파일 한 줄씩 읽기
    while (getline(inFile, line) && outCount < maxPackages)
    {
        stringstream ss(line);
        string token;
        string data[7];
        int idx = 0;

        // 쉼표 기준으로 문자열 자르기
        while (getline(ss, token, ',') && idx < 7)
        {
            data[idx++] = token;
        }

        // 데이터가 7개 모두 정상적으로 파싱되면 객체 복원
        if (idx == 7)
        {
            string id = data[0];
            string name = data[1];
            DeliveryType type = static_cast<DeliveryType>(stoi(data[2])); // int -> enum
            double weight = stod(data[3]);  // string -> double
            int priority = stoi(data[4]);   // string -> int
            DeliveryStatus status = static_cast<DeliveryStatus>(stoi(data[5])); // int -> enum
            int ttl = stoi(data[6]); // string -> int

            Package* newPkg = new Package(id, name, type, weight, priority, ttl);
            newPkg->updateStatus(status);   // 현재 상태 덮어쓰기

            loadedPackages[outCount++] = newPkg;
        }
    }

    inFile.close();
    return loadedPackages;
}
