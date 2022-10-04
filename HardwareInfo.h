#ifndef HardwareInfo_H
#define HardwareInfo_H
#include <QString>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>


class HardwareInfo
{
public:
    HardwareInfo();

    static QString  getCpuId();

    static QString  getDiskId();

    static QString  getMachineId();

    static void     parseDiskSerial(const char *file_name, const char *match_words, std::string &serial_no);
};

#endif
