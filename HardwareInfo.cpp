#include "HardwareInfo.h"

HardwareInfo::HardwareInfo()
{
}

QString  HardwareInfo::getCpuId()
{
    std::string   cpu_id;
    unsigned int  s1 = 0;
    unsigned int  s2 = 0;

    asm volatile (
        "movl $0x01, %%eax; \n\t"
        "xorl %%edx, %%edx; \n\t"
        "cpuid; \n\t"
        "movl %%edx, %0; \n\t"
        "movl %%eax, %1; \n\t"
        : "=m" (s1), "=m" (s2));

    if ((0 == s1) && (0 == s2))
    {
        return QString::fromStdString(cpu_id);
    }

    char  cpu[128];
    memset(cpu, 0, sizeof(cpu));
    snprintf(cpu, sizeof(cpu), "%08X%08X", htonl(s2), htonl(s1));
    cpu_id.assign(cpu);

    return QString::fromStdString(cpu_id);
}

void  HardwareInfo::parseDiskSerial(const char *file_name, const char *match_words, std::string &serial_no)
{
    serial_no.c_str();

    std::ifstream  ifs(file_name, std::ios::binary);

    if (!ifs.is_open())
    {
        return;
    }

    char  line[4096] = { 0 };

    while (!ifs.eof())
    {
        ifs.getline(line, sizeof(line));

        if (!ifs.good())
        {
            break;
        }

        const char *board = strstr(line, match_words);

        if (NULL == board)
        {
            continue;
        }

        board += strlen(match_words);

        while ('\0' != board[0])
        {
            if (' ' != board[0])
            {
                serial_no.push_back(board[0]);
            }

            ++board;
        }

        if ("None" == serial_no)
        {
            serial_no.clear();
            continue;
        }

        if (!serial_no.empty())
        {
            break;
        }
    }

    ifs.close();
}

QString  HardwareInfo::getDiskId()
{
    std::string  serial_no;
    std::string  disk_name = "/dev/sda";

    serial_no.c_str();

    const char *dmidecode_result = ".dmidecode_result.txt";
    char        command[512]     = { 0 };
    snprintf(command, sizeof(command), "udevadm info --query=all --name=%s | grep ID_SERIAL_SHORT= > %s", disk_name.c_str(), dmidecode_result);

    if (0 == system(command))
    {
        parseDiskSerial(dmidecode_result, "ID_SERIAL_SHORT=", serial_no);
    }

    unlink(dmidecode_result);

    return QString::fromStdString(serial_no);
}

QString  HardwareInfo::getMachineId()
{
    return getCpuId().append(getDiskId());
}
