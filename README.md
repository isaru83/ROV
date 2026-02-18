# 🚤 ROV – Remotely Operated Vehicle

<<<<<<< HEAD
**ROV (Remotely Operated Vehicle)** est un projet personnel de sous‑marin téléopéré conçu pour l’exploration sous‑marine légère.  
L’objectif est de créer un engin maniable, stable et silencieux, plutôt orienté observation que vitesse.
=======
Projet personnel de **ROV** (Remotely Operated Vehicle) compact conçu pour l'observation sous-marine.
>>>>>>> 6078f80ec8499ac097c0fea9df73601aea54c7cd

---

## 🔍 Description du projet

<<<<<<< HEAD
Ce ROV est un véhicule sous‑marin télécommandé (Remote Operated Vehicle) construit autour d’un microcontrôleur **Black Pill STM32F401CC** et conçu pour fonctionner jusqu’à ~10 m de profondeur dans un boîtier étanche imprimé en 3D. :contentReference[oaicite:0]{index=0}
=======
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
>>>>>>> 6078f80ec8499ac097c0fea9df73601aea54c7cd

Le ROV est capable de :

<<<<<<< HEAD
- 🚀 propulsion via 4 moteurs brushless + ESC  
- 📏 asservissement en pitch, yaw et profondeur  
- 🧠 commandes via joystick / station PC  
- 🧭 capteurs de navigation (IMU, pression)  
- 📡 télémétrie en temps réel (attitude, profondeur, température)  
- 🔄 protocole de communication RS485 demi‑duplex  
- 🧪 vecteurs de télécommande + tuning PID paramétrable :contentReference[oaicite:1]{index=1}
=======
- Black Pill STM32F401CC
- 4 × ESC + moteurs 2212 920KV
- IMU BNO08x
- Capteur pression MS5837 (BlueRobotics)
- Pompe péristaltique + driver DRV8871
- MAX485 pour RS485
- Boîtier étanche PETG 3D imprimé (170×120×95 mm)
- Plexiglas vissé pour couvercle
- Hélices maison Ø60 mm 3 pales
>>>>>>> 6078f80ec8499ac097c0fea9df73601aea54c7cd

---

## ⚙️ Caractéristiques techniques

**Matériel principal :**  
- 🧠 Microcontrôleur : Black Pill STM32F401CC  
- ⚙️ Propulsion : 4× moteurs brushless + ESC 2212 920KV  
- 🧭 Capteurs : IMU BNO08x, capteur de pression MS5837  
- 📡 Communication : RS485 (MAX485)  
- 💧 Boîtier étanche imprimé en PETG + plexiglas  
- 🔌 Ballast actif avec pompe péristaltique et driver DRV8871 :contentReference[oaicite:2]{index=2}

---

## 🧪 Fonctionnalités implémentées

- 🔁 Asservissement PID pour pitch, yaw et profondeur  
- ⚙️ Calibration ESC au démarrage  
- 📊 Télémétrie (accélération, orientation, profondeur, pression, température)  
- 📡 Protocole binaire sur RS485 pour commandes et retour  
- 🛟 Gestion des automatismes failsafe :contentReference[oaicite:3]{index=3}

---

## 🛠️ Installation & Développement

### Pré-requis

Avant de commencer, assure‑toi d’avoir :

- 📌 **PlatformIO** avec **VS Code**
- 📌 Core STM32 pour PlatformIO
- 📌 Bibliothèques suivantes dans PlatformIO :  
  - Adafruit BNO08x  
  - PID_v1  
  - BlueRobotics MS5837  
  - (et autres dépendances selon tes sources) :contentReference[oaicite:4]{index=4}

---
