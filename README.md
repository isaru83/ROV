# SubMarin - ROV d'exploration

Projet personnel de **ROV** (Remotely Operated Vehicle) compact conçu pour l'observation sous-marine.

L'objectif est d'obtenir un engin **très maniable**, **lent** (0.1–0.8 m/s), stable et silencieux, plutôt qu'un ROV de course.

## Caractéristiques principales

- **Profondeur max cible** : 10 mètres (boîtier étanche PETG 3D imprimé + plexiglas vissé)
- **Configuration moteurs** : 4 × ESC 2212 920KV en + hélices maison Ø60 mm dans tunnels de poussée
- **Asservissement** : PID sur pitch, yaw + asservissement profondeur par ballast central (Pas de                          contole sur le roll)
- **Ballast** : pompe péristaltique + poche d'eau interne
- **Capteurs** : IMU BNO08x, pression MS5837, futur capteur Hall sur pompe
- **Communication** : RS485 demi-duplex (MAX485) avec protocole binaire simple
- **Contrôle** : station PC/joystick avec commandes throttle, profondeur, assiette
- **Failsafe** : vidange ballast + faible avance si perte de communication > 1,2 s
- **Microcontrôleur** : Black Pill STM32F401CC
- **Langage** : C++ (Arduino framework + PlatformIO)

## Matériel principal

- Black Pill STM32F401CC
- 4 × ESC + moteurs 2212 920KV
- IMU BNO08x
- Capteur pression MS5837 (BlueRobotics)
- Pompe péristaltique + driver DRV8871
- MAX485 pour RS485
- Boîtier étanche PETG 3D imprimé (170×120×95 mm)
- Plexiglas vissé pour couvercle
- Hélices maison Ø60 mm 3 pales

## Fonctionnalités implémentées

- Asservissement pitch/yaw (PID)
- Asservissement profondeur (PID sur ballast)
- Calibration ESC parallèle (4 s au démarrage)
- Vidange ballast au démarrage (6 s ajustable)
- Failsafe : vidange + faible poussé vertical si perte de com > 1,2 s
- Protocole binaire simple (trame type 0x01 pour pilotage complet)
- Commandes : throttle V/H, depth order, pitch/yaw order, demande télémétrie, tuning PID
- Télémétrie : pitch/roll/yaw, accel, heading, profondeur, pression, température

## Protocole de communication (RS485)

- Maître = station PC
- Trame binaire (start 0xAA + type + id + len + data + CRC16)
- Commande principale (0x01) : vThrottle (int16), hThrottle (int16), depthOrder (float), pitchOrder (float), yawOrder (float)
- Télémétrie sur demande (0x02 → 0x80)
- Tuning PID (get 0x03, set 0x04)
- Changement densité fluide (0x05)

## Installation / Développement

### Prérequis

- PlatformIO (VS Code)
- STM32 core (dans PlatformIO)
- Bibliothèques :
  - Adafruit BNO08x
  - PID_v1
  - BlueRobotics MS5837 (GitHub)

