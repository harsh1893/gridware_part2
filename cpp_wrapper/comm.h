#ifndef COMM_H
#define COMM_H
#include <pb_encode.h>
#include <pb_decode.h>
#include "gridware.pb.h"
#include <iostream>


template <class T>
class Comm
{
private:
/*
@function:  get the CRC value based of data param.
@param: data: upon which CRC is calculated
@param: size: number of bytes of data
@return: T: get CRC value of T-type
*/
    T Get_CRC16(uint8_t const *data, size_t size);

public:

Comm(){

}
/*
@function:  encodes firmware Image Page.
@param: last: true if last set of data else false.
@param: data_size: number of bytes of data to be encoded
@param: data: data that needs to be transferred.
@param: tx_msg_size: reference variable for size of encoded message
@param: tx_msg: pointer to store encoded message
@return: bool: True: encoding worked ,False: encoding failed
@Note: Use gridware_FirmwareImagePage_size as the tx_msg array size
*/
    bool Encode_FirmwareImagePage(
        bool last,
        uint16_t data_size,
        uint8_t *data,
        uint16_t &tx_msg_size,
        uint8_t *tx_msg);
/*
@function:  decodes firmware Image Page.
@param: last: referenece variable is set true if last series of 
              rx message  is received 
@param: data_size: referenece variable to store received data size
@param: data: data that was received.
@param: is_crc_verified: true if CRC checks out else false.
@param: rx_msg_size:  size of decoded message
@param: rx_msg: pointer to decoded message
@return: bool: True: decoding worked ,False: decoding failed
*/
    bool Decode_FirmwareImagePage(
        bool &last,
        uint16_t &data_size,
        uint8_t *data,
        bool &is_crc_verified,
        uint16_t rx_msg_size,
        uint8_t *rx_msg);
/*
@function:  encodes device response
@param: verified: true if CRC passed on received data.
@param: tx_msg_size: reference variable for size of encoded message
@param: tx_msg: pointer to store encoded message
@return: bool: True: encoding worked ,False: encoding failed
@Note: Use gridware_DeviceResponse_size as the tx_msg array size
*/
    bool Encode_DeviceResponse(
        bool verified,
        uint16_t &tx_msg_size,
        uint8_t *tx_msg);
/*
@function:  decodes device response
@param: verified: reference variable is set true if decoded message 
                  contains true as verified
@param: rx_msg_size: size of encoded message
@param: rx_msg: pointer to received encoded message
@return: bool: True: decoding worked ,False: decoding failed
*/
    bool Decode_DeviceResponse(
        bool &verified,
        uint16_t rx_msg_size,
        uint8_t *rx_msg);
};

template <class T>
bool Comm<T>::Encode_FirmwareImagePage(
    bool last,
    uint16_t data_size,
    uint8_t *data,
    uint16_t &tx_msg_size,
    uint8_t *tx_msg)
{
    bool status;

    gridware_FirmwareImagePage message_FirmwareImagePage = gridware_FirmwareImagePage_init_zero;


    message_FirmwareImagePage.last = last;
    message_FirmwareImagePage.page.size = data_size;
    memcpy(message_FirmwareImagePage.page.bytes, data, data_size);
    T crc = Get_CRC16(data, data_size);
    message_FirmwareImagePage.crc.size = 2;
    message_FirmwareImagePage.crc.bytes[0] = crc & 0xFF;
    message_FirmwareImagePage.crc.bytes[1] = (crc >> 8) & 0xFF;
    /* Create a stream that will write to our buffer. */
    pb_ostream_t stream = pb_ostream_from_buffer(tx_msg, gridware_FirmwareImagePage_size);

    /* Now we are ready to encode the message! */
    status = pb_encode(&stream, gridware_FirmwareImagePage_fields, &message_FirmwareImagePage);

    /* Then just check for any errors.. */
    if (!status)
    {
        return false;
    }
    tx_msg_size = stream.bytes_written;
    return true;
}
template <class T>
bool Comm<T>::Decode_FirmwareImagePage(
    bool &last,
    uint16_t &data_size,
    uint8_t *data,
    bool &is_crc_verified,
    uint16_t rx_msg_size,
    uint8_t *rx_msg)
{
    bool status;
    gridware_FirmwareImagePage message_FirmwareImagePage = gridware_DeviceResponse_init_zero;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(rx_msg, rx_msg_size);

    /* Now we are ready to decode the message. */
    status = pb_decode(&stream, gridware_FirmwareImagePage_fields, &message_FirmwareImagePage);
    /* Check for errors... */
    if (!status)
    {
        return false;
    }
    data_size = message_FirmwareImagePage.page.size;
    memcpy(data, message_FirmwareImagePage.page.bytes, data_size);
    last = message_FirmwareImagePage.last;
    is_crc_verified = (Get_CRC16(data, data_size) & 0xFFFF) == (
        ((message_FirmwareImagePage.crc.bytes[1] << 8) & 0xff00) + 
        message_FirmwareImagePage.crc.bytes[0]);
    return true;
}
template <class T>
bool Comm<T>::Encode_DeviceResponse(
    bool verified,
    uint16_t &tx_msg_size,
    uint8_t *tx_msg)
{
    bool status;
    gridware_DeviceResponse message_DeviceResponse = gridware_DeviceResponse_init_zero;
    message_DeviceResponse.verified = verified;

    /* Create a stream that will write to our buffer. */
    pb_ostream_t stream = pb_ostream_from_buffer(tx_msg, gridware_DeviceResponse_size);

    /* Now we are ready to encode the message! */
    status = pb_encode(&stream, gridware_DeviceResponse_fields, &message_DeviceResponse);

    /* Then just check for any errors.. */
    if (!status)
    {
        return false;
    }
    tx_msg_size = stream.bytes_written;
    return true;
}
template <class T>
bool Comm<T>::Decode_DeviceResponse(
    bool &verified,
    uint16_t rx_msg_size,
    uint8_t *rx_msg)
{
    bool status;
    gridware_DeviceResponse message_DeviceResponse = gridware_DeviceResponse_init_zero;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(rx_msg, rx_msg_size);

    /* Now we are ready to decode the message. */
    status = pb_decode(&stream, gridware_DeviceResponse_fields, &message_DeviceResponse);

    /* Check for errors... */
    if (!status)
    {
        return false;
    }
    verified = message_DeviceResponse.verified;
    return true;
}
template <class T>
T Comm<T>::Get_CRC16(uint8_t const *data, size_t size)
{
    T crc = 0;
    while (size--)
    {
        crc ^= *data++;
        for (unsigned k = 0; k < 8; k++)
            crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
    }
    return crc;
}


#endif