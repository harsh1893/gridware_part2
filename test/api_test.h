#ifndef API_TEST_H
#define API_TEST_H
#include "comm.h"
#include <fstream>
#include <thread>

#define TEST_BIN_FILE "../nanopb/README.md"



// mutex mtx
// condition_variable cv;
#define max_tx_size 2048
class StoD{
    public:
    uint8_t tx_msg[gridware_FirmwareImagePage_size];
    uint16_t size;
    bool data_ready;
};



class DtoS{
    public:
    uint8_t tx_msg[gridware_DeviceResponse_size];
    uint16_t size;
    bool data_ready;
};



#endif