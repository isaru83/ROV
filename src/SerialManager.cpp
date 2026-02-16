#include "SerialManager.h"
#include "Pins.h"
#include <cstring>  

SerialManager::SerialManager(HardwareSerial *serial)
    : _serial(serial)
{
    
}

SerialManager::~SerialManager()
{

}

void SerialManager::update()
{
    while (_serial->available())
    {
        uint8_t b = _serial->read();
        rxBuffer[rxHead] = b;
        rxHead = (rxHead + 1) % (sizeof(rxBuffer));

        // On cherche une trame complète
        while (rxTail != rxHead)
        {
            // Cherche start byte
            if (rxBuffer[rxTail] != START_BYTE)
            {
                rxTail = (rxTail + 1) % sizeof(rxBuffer);
                continue;
            }

            // Vérifie qu'on a assez de bytes pour le header
            uint16_t avail = (rxHead - rxTail + sizeof(rxBuffer)) % sizeof(rxBuffer);
            if (avail < MIN_FRAME_SIZE) break;

            uint8_t type = rxBuffer[(rxTail + 1) % sizeof(rxBuffer)];
            uint8_t id   = rxBuffer[(rxTail + 2) % sizeof(rxBuffer)];
            uint8_t len  = rxBuffer[(rxTail + 3) % sizeof(rxBuffer)];

            // Vérifie longueur
            if (len > REQ_MAX_SIZE || len == 0) {
                rxTail = (rxTail + 1) % sizeof(rxBuffer);
                continue;
            }

            // Vérifie qu'on a toute la trame + CRC
            if (avail < (5 + len + 2)) break;

            // Calcule CRC sur start + type + id + len + data
            uint16_t crc_calc = calculateCRC(&rxBuffer[rxTail], 4 + len);

            uint16_t crc_received;
            memcpy(&crc_received, &rxBuffer[(rxTail + 4 + len) % sizeof(rxBuffer)], 2);

            if (crc_calc == crc_received)
            {
                // Trame valide !
                lastType = type;
                lastId   = id;
                lastLen  = len;
                memcpy(lastData, &rxBuffer[(rxTail + 4) % sizeof(rxBuffer)], len);
                frameReady = true;

                // Avance tail après la trame complète
                rxTail = (rxTail + 5 + len + 2) % sizeof(rxBuffer);
                break;
            }
            else
            {
                // CRC invalide → on avance d'un octet et on recommence
                rxTail = (rxTail + 1) % sizeof(rxBuffer);
            }
        }
    }
}

bool SerialManager::available() const
{
    return frameReady;
}

uint8_t SerialManager::getFrame(uint8_t *type, uint8_t *id, uint8_t *data)
{
    if (!frameReady) return 0;

    *type = lastType;
    *id   = lastId;
    memcpy(data, lastData, lastLen);
    frameReady = false;  

    return lastLen;
}

void SerialManager::sendFrame(uint8_t type, const uint8_t *data, uint8_t len)
{
    uint8_t buf[5 + len + 2];
    buf[0] = START_BYTE;
    buf[1] = type;
    buf[2] = 0;
    buf[3] = len;
    memcpy(&buf[4], data, len);

    uint16_t crc = calculateCRC(buf, 4 + len);
    memcpy(&buf[4 + len], &crc, 2);

    digitalWrite(PIN_RS485_DE, HIGH);
    delayMicroseconds(100);
    _serial->write(buf, 5 + len + 2);
    _serial->flush();
    delayMicroseconds(100);
    digitalWrite(PIN_RS485_DE, LOW);
}

uint16_t SerialManager::calculateCRC(const uint8_t *buf, uint16_t len) const
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)buf[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}