#ifndef SERIALMANAGER_H_INCLUDED
#define SERIALMANAGER_H_INCLUDED

/*                                FRAME
/*   ____________ ___________ _________ _____________ ______ _________________
/*  | Start byte | Type byte | ID byte | Length byte | Data | Checksum 2bytes |
/*   
/*  5 octets pour le header et 2 octets pour le chercksum
*/

#include "Arduino.h"
#include "HardwareSerial.h"
#include <cstdint>

#define REQ_MAX_SIZE        128     // taille max d'une trame reçue 
#define START_BYTE          0xAA
#define MIN_FRAME_SIZE      6       // start + type + id + len + crc(2)



class SerialManager
{
    public:
        SerialManager(HardwareSerial *serial);
        ~SerialManager();

        void update();
        bool available() const;
        uint8_t getFrame(uint8_t *type, uint8_t *id, uint8_t *data);
        void sendFrame(uint8_t type, const uint8_t *data, uint8_t len);

    private:

        uint16_t calculateCRC(const uint8_t *buf, uint16_t len) const;
        HardwareSerial *_serial;

        uint8_t rxBuffer[REQ_MAX_SIZE * 2]; 
        volatile uint16_t rxHead = 0;
        volatile uint16_t rxTail = 0;

        uint8_t lastType = 0;
        uint8_t lastId   = 0;
        uint8_t lastData[REQ_MAX_SIZE];
        uint8_t lastLen  = 0;
        bool frameReady  = false;
};

#endif // SERIALMANAGER_H_INCLUDED