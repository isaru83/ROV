#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

// =====================================================================
// PROTOCOLE DE COMMUNICATION RS485 (types de trames)
// =====================================================================
// Tous les types sont sur 1 octet (0x00 à 0xFF)
// Convention :
//   0x00 à 0x7F : commandes station → ROV
//   0x80 à 0xFE : réponses / données ROV → station
//   0xFF         : alertes spontanées (urgence)
// =====================================================================

enum TrameType : uint8_t
{
    // ────────────────────────────────────────────────────────────────
    // COMMANDES STATION → ROV  (0x00 à 0x7F)
    // ────────────────────────────────────────────────────────────────
    CMD_THROTTLE_BASE = 0x01,       //Control ROV
                                    //Data: uint16 vThrottle, uint16 hThrottle, float depthOrder, 
                                    //      float yaw, float pitch (16 octets)

    CMD_REQUEST_TELEMETRY = 0x02,   // Demande envoi de la télémétrie complète
                                    // Data : aucun (ou uint8 subType si plusieurs niveaux)

    CMD_REQUEST_PID_TUNINGS = 0x03, // Demande les coef PID actuels d’un axe
                                    // Data : uint8 axe (0=pitch, 1=yaw, etc.)

    CMD_SET_PID_TUNINGS  = 0x04,    // Modification des coef PID d’un axe
                                    // Data : uint8 axe, float Kp, float Ki, float Kd

    CMD_SET_FLUID_DENSITY = 0x05,   // Changer densité fluide (eau douce / mer)
                                    // Data : uint16 density (kg/m³)

    CMD_CALIBRATE_SURFACE = 0x06,   // Demande calibration surface (pression 0 m)
                                    // Data : aucun

    CMD_REBOOT           = 0x07,    // Demande redémarrage du ROV (sécurité)
                                    // Data : aucun (ou code confirmation)

    CMD_START            = 0x08,    //Demande de demarage
                                    //Data : aucun 

    // Réserve 0x11 à 0x7F pour futures commandes (ex. mode auto, caméra, etc.)


    // ────────────────────────────────────────────────────────────────
    // RÉPONSES / TÉLÉMÉTRIE ROV → STATION  (0x80 à 0xFE)
    // ────────────────────────────────────────────────────────────────
    RSP_TELEMETRY_FULL   = 0x80,    // Télémétrie complète envoyée en réponse à CMD_REQUEST_TELEMETRY
                                    // Data : pitch(float), roll, yaw, depth, vbat, ballast%, temp, status, etc.

    RSP_PID_TUNINGS      = 0x81,    // Réponse avec les coef PID actuels (suite à CMD_REQUEST_PID_TUNINGS)
                                    // Data : uint8 axe, float Kp, float Ki, float Kd

    RSP_CALIBRATE_ACK    = 0x82,    // Confirmation calibration surface effectuée
                                    // Data : float pressureRef (hPa)

    RSP_STATUS           = 0x83,    // Statut rapide (tension, ballast %, erreurs)
                                    // Data : float vbat, uint8 ballastPercent, uint8 errorCode

    RSP_ERROR            = 0x8F,    // Erreur lors du traitement d’une commande
                                    // Data : uint8 commandTypeReceived, uint8 errorCode

    // Réserve 0x90 à 0xFE pour futures réponses


    // ────────────────────────────────────────────────────────────────
    // ALERTES SPONTANÉES (ROV → Station)  (0xFF uniquement)
    // ────────────────────────────────────────────────────────────────
    ALERT_URGENT         = 0xFF     // Alerte spontanée (prioritaire)
                                    // Data : uint8 alertCode (1=fuite, 2=low bat, 3=overtemp, etc.)
                                    // + éventuelles données supplémentaires (vbat, etc.)
};

#endif