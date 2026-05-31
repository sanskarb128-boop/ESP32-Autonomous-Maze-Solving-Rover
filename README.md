# 🚗 ESP32 Autonomous Maze Solving Rover

An ESP32-based autonomous rover designed to navigate through a maze using ultrasonic sensors and obstacle avoidance algorithms. The rover supports both autonomous navigation and manual control through a WiFi-based mobile web interface, along with OTA (Over-The-Air) firmware updates.

---

## 📸 Project Overview

This project demonstrates the integration of embedded systems, robotics, IoT, and wireless communication. The rover continuously scans its surroundings using three ultrasonic sensors and makes navigation decisions based on sensor data.

The system also includes a mobile control interface that allows users to manually control the rover through a web browser connected over WiFi.

---

## ✨ Features

- 🤖 Autonomous Maze Solving
- 📡 Obstacle Detection & Avoidance
- 📱 Mobile Web Control Interface
- 🌐 WiFi Connectivity using ESP32
- 🔄 OTA (Over-The-Air) Code Updates
- 🚗 Differential Drive Motor Control
- 📏 Real-Time Distance Measurement
- ⚡ Regulated Power Supply using Buck Converter
- 🎯 Left-Hand Rule Navigation Logic

---

## 🛠 Hardware Components

| Component | Quantity |
|------------|-----------|
| ESP32 Development Board | 1 |
| HC-SR04 Ultrasonic Sensor | 3 |
| L298N Motor Driver | 1 |
| DC Geared Motors | 2 |
| Wheels | 2 |
| Caster Wheel | 1 |
| Battery Pack | 1 |
| LM2596 Buck Converter | 1 |
| Power Switch | 1 |
| Chassis | 1 |

---

## 💻 Software & Technologies

- Arduino IDE
- Embedded C/C++
- ESP32 WiFi Library
- ArduinoOTA Library
- HTML/CSS
- Robotics & Automation Concepts

---

## 🔌 System Architecture

```text
Battery Pack
      │
      ▼
    Switch
      │
 ┌────┴─────┐
 ▼          ▼
L298N    Buck Converter
 ▼          ▼
Motors    ESP32
             │
     ┌───────┼───────┐
     ▼       ▼       ▼
 Front    Left    Right
 Sensor   Sensor  Sensor
```

---

## 📡 Sensor Configuration

| Sensor | ESP32 Pins |
|----------|------------|
| Front Sensor | TRIG: GPIO 5, ECHO: GPIO 18 |
| Left Sensor | TRIG: GPIO 19, ECHO: GPIO 21 |
| Right Sensor | TRIG: GPIO 22, ECHO: GPIO 23 |

---

## ⚙ Motor Driver Connections

| L298N | ESP32 |
|--------|--------|
| IN1 | GPIO 14 |
| IN2 | GPIO 27 |
| IN3 | GPIO 26 |
| IN4 | GPIO 25 |

---

## 🧠 Navigation Logic

The rover follows a decision-making process based on ultrasonic sensor readings:

```text
IF Front Path Clear
    Move Forward

ELSE IF Left Path Clear
    Turn Left

ELSE IF Right Path Clear
    Turn Right

ELSE
    Perform U-Turn
```

This allows the rover to autonomously navigate through obstacles and maze environments.

---

## 📱 Mobile Control Mode

The rover includes a WiFi-based control panel accessible through a smartphone browser.

### Controls

- Forward
- Backward
- Left
- Right
- Stop
- Auto Mode
- Manual Mode

---

## 🔄 OTA Support

The project supports OTA updates, allowing firmware uploads wirelessly without connecting a USB cable.

### Benefits

- Faster development
- Easy updates
- Wireless deployment

---

## 🚀 How to Run

### 1. Upload Code

Upload the Arduino sketch to the ESP32 using the Arduino IDE.

### 2. Power the System

- Connect the battery pack
- Turn on the power switch

### 3. Connect to WiFi

Connect the mobile device to the configured WiFi network.

### 4. Open Control Panel

Enter ESP32 IP address in the browser:

```text
http://<ESP32-IP>
```

### 5. Select Mode

- AUTO → Autonomous Navigation
- MANUAL → Mobile Control

---

## 🎯 Applications

- Autonomous Navigation
- Educational Robotics
- Smart Automation Systems
- Warehouse Robots
- Obstacle Avoidance Research
- Robotics Learning Projects

---

## 📚 Learning Outcomes

Through this project, I gained hands-on experience in:

- Embedded Systems Development
- ESP32 Programming
- Sensor Integration
- Robotics Algorithms
- IoT Communication
- Motor Control Systems
- Hardware Debugging
- OTA Deployment

---

## 🔮 Future Enhancements

- React-Based Dashboard
- Live Sensor Monitoring
- Cloud Connectivity
- MQTT Communication
- Path Optimisation
- AI-Based Navigation
- Camera Integration
- Remote Monitoring System

---

## 👨‍💻 Author

**Sanskar Bhatt**

Robotics & Automation Engineering Student

📧 sanskarb128@gmail.com

🔗 LinkedIn: https://www.linkedin.com/in/sanskar-bhatt-37b76136b

---

⭐ If you found this project interesting, consider giving it a star!
