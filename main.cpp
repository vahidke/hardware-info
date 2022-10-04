#include "HardwareInfo.h"
#include <iostream>

int  main(int argc, char *argv[])
{
    auto  cpuId     = HardwareInfo::getCpuId();
    auto  diskId    = HardwareInfo::getDiskId();
    auto  machineId = HardwareInfo::getMachineId();
    std::cout << "CPU Id: " << cpuId.toStdString() << std::endl;
    std::cout << "Hard Disk Id: " << diskId.toStdString() << std::endl;
    std::cout << "Machine Id: " << machineId.toStdString() << std::endl;

    return 0;
}
