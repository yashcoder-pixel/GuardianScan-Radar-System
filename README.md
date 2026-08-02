
# GuardianScan –Radar-System

## Overview

GuardianScan is an Arduino-based radar surveillance system that continuously scans its surroundings using an ultrasonic sensor mounted on a servo motor. The system detects nearby objects, identifies potential threats based on distance, locks onto dangerous targets, and alerts the user using visual and audible indicators.

The project demonstrates embedded system concepts including state-machine programming, servo motor control, ultrasonic distance measurement, LCD interfacing, GPIO control, and real-time event handling.


# Features

* 180° continuous radar scanning
* Real-time distance measurement
* Servo-based radar movement
* Object detection zone
* Target lock mechanism
* Danger zone monitoring
* Continuous alarm for nearby objects
* LCD status display
* RGB LED indication
* Buzzer alerts
* Automatic scan resume after threat clearance



# Working

The servo motor continuously rotates from 0° to 180°, simulating a radar scan.

During every scan, the HC-SR04 ultrasonic sensor measures the distance to nearby objects.

The system classifies objects into different zones.

Distance > 50 cm
↓
Safe Zone
Green LED
Scanning continues

20 cm – 50 cm
↓
Detection Zone
Object Detected
Red LED ON

Distance < 20 cm
↓
Danger Zone
Target Locked
Servo Stops
Continuous Buzzer
Alarm Active

Object Removed
↓
Threat Cleared
Scanning Resumes
```

---

# Hardware Used

* Arduino UNO
* HC-SR04 Ultrasonic Sensor
* SG90 Servo Motor
* 16x2 LCD (I2C)
* Red LED
* Green LED
* Passive Buzzer
* Breadboard
* Jumper Wires
* Resistors

---

# Software

* Arduino IDE
* Embedded C
* Wire Library
* Servo Library
* LiquidCrystal_I2C Library

---

# Pin Connections

Ultrasonic Trigger  → D7

Ultrasonic Echo     → D8

Servo Motor         → D9

Red LED             → D13

Green LED           → D12

Buzzer              → D2

LCD SDA             → A4

LCD SCL             → A5
```


# State Machine

The project uses six operating states.


BOOT

↓

SCANNING

↓

OBJECT DETECTED

↓

TARGET LOCKED

↓

ALARM ACTIVE

↓

THREAT CLEARED

↓

SCANNING
```


# Skills Learned

* Embedded C Programming
* Arduino Programming
* State Machine Design
* Servo Motor Control
* Ultrasonic Sensor Interfacing
* LCD (I2C) Communication
* GPIO Programming
* Event Driven Programming
* Real-Time Monitoring
* Sensor Calibration
* Hardware Debugging
* Embedded System Design


# Future Improvements

* Radar Visualization using Processing IDE

* ESP32 Wi-Fi Monitoring

* Mobile App Notifications

* Camera Integration

* Object Tracking

* OpenCV Face Detection

* Firebase Cloud Logging

* MQTT Support

* STM32 Version

* FreeRTOS Implementation



# Author

**Yash Doke**

B.Tech Electronics & Communication Engineering

[https://github.com/yashcoder-pixel](https://github.com/yashcoder-pixel)

