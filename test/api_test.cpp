#include "api_test.h"
#include <iostream>

using namespace std;

StoD stod_msg;
DtoS dtos_msg;

void server_task()
{
    Comm<uint16_t> server;

    bool last = false;
    uint16_t data_size = 0;
    uint8_t data[max_tx_size];
    bool verified;

    //Read file as binary
    std::ifstream file(TEST_BIN_FILE, std::ios::binary | std::ios::ate);
    std::streamsize file_size = file.tellg();
    uint32_t curr_file_idx = 0;

    cout<< "Total file size: "<< file_size<<endl;

    while (true)
    {
        //Read file from desired index
        file.seekg(curr_file_idx, std::ios::beg);
        //Check if last trnasmission
        if ((file_size - curr_file_idx) <= max_tx_size)
        {
            last = true;
            data_size = file_size % max_tx_size;
        }
        else
        {
            last = false;
            data_size = max_tx_size;
        }
        // read file into buffer
        file.read((char *)(&data[0]), data_size);
        //Use API to encode
        server.Encode_FirmwareImagePage(
            last,
            data_size,
            data,
            stod_msg.size,
            stod_msg.tx_msg);
        cout << "> FirmwareImagePage "<< curr_file_idx/ max_tx_size <<" Encoded" << endl;
        stod_msg.data_ready = true;

        //busy wait till message received from other thread
        while (!dtos_msg.data_ready)
            ;
        cout << "> DeviceResponse Received" << endl;
        // decode the message
        server.Decode_DeviceResponse(
            verified,
            dtos_msg.size,
            dtos_msg.tx_msg);
        //mark message as read.
        dtos_msg.data_ready = false;

        cout << "> DeviceResponse Decoded" << endl;
        //change file seek value based on verified param
        if (verified)
        {
            cout << "> DeviceResponse Decoded and Verified" << endl;
            curr_file_idx += max_tx_size;
        }
        else
        {
            cout << "> DeviceResponse Decoded and NOT Verified" << endl;
        }
        //if last trasnmission verified, exit.
        if (last && verified)
        {
            cout << "> Last FirmwareImagePage is sent.Transfer Complete" << endl;
            file.close();
            return;
        }
    }
}

void device_task()
{
    Comm<uint16_t> device;

    bool last;
    uint16_t data_size;
    uint8_t data[max_tx_size];
    bool is_crc_verified;
    while (true)
    {
        //busy wait till message is received from server
        while (!stod_msg.data_ready)
            ;
        cout << "> FirmwareImagePage is received" << endl;
        // decode message
        device.Decode_FirmwareImagePage(
            last,
            data_size,
            data,
            is_crc_verified,
            stod_msg.size,
            stod_msg.tx_msg);
        //mark message as read
        stod_msg.data_ready = false;
        //check if CRC is verified
        if (is_crc_verified)
            cout << "> FirmwareImagePage is decoded and CRC verified" << endl;
        else
            cout << "> FirmwareImagePage is decoded and NOT CRC verified" << endl;
        //Encode message for server
        device.Encode_DeviceResponse(
            is_crc_verified,
            dtos_msg.size,
            dtos_msg.tx_msg);
        cout << "> DeviceResponse encoded" << endl;
        //mark message is available for server
        dtos_msg.data_ready = true;

        //if last message and crc is verified. exit
        if (is_crc_verified && last)
        {
            cout << "> Last FirmwareImagePage is received. Transfer Compelete" << endl;

            return;
        }
    }
}

int main()
{

    cout << "Test begins" << endl;
    stod_msg.data_ready = false;
    dtos_msg.data_ready = false;

    thread server(server_task);
    thread device(device_task);

    server.join();
    device.join();

    cout << "Test Ended" << endl;

    return 1;
}