# 🦜 Rio Robot — Brazil Representative

*Part of the "Birds of Diversity" Robotics Showcase*

## 🌍 Project Overview

The **Rio Robot** is a vibrant, autonomous mobile robot that represents **Brazil** in the *Birds of Diversity* robotics showcase. Inspired by the colorful **macaws of Rio de Janeiro**, this robot embodies energy, motion, and rhythm. It performs a coordinated dance routine, emphasizing **movement, resilience, and celebration of culture**.

This robot integrates motion control, orientation awareness, and obstacle handling using multiple sensors. Through its choreography, Rio contributes to the project's goal of celebrating **diversity, unity, and identity** through engineering and performance.

---

## 🤖 Key Features

* 🧭 **Orientation-Aware Movement**: Uses the **MPU6050** to maintain consistent directional heading.
* 🌀 **Choreographed Spinning/Dancing**: Loops through square-like movement patterns.
* 🛑 **Line Detection**: Halts safely if it crosses performance boundaries.
* 🔊 **Obstacle Avoidance**: Stops or adjusts if objects are detected via an **ultrasonic sensor**.
* 🛠️ **Yaw Drift Correction**: Actively self-corrects if rotation becomes inaccurate.

---

## 🔧 Hardware Components

| Component                           | Purpose                               |
| ----------------------------------- | ------------------------------------- |
| **Arduino Uno/Nano**                | Core microcontroller                  |
| **MPU6050**                         | IMU for tracking yaw/rotation         |
| **Ultrasonic Sensor**               | Detects obstacles in front            |
| **IR Line Sensors (A0–A2)**         | Boundary detection via floor contrast |
| **Motor Driver (L298N or similar)** | Controls dual DC motors               |
| **DC Motors (x2)**                  | For mobility                          |
| **Battery Pack**                    | Power source                          |

---

## 🧠 Code Behavior Summary

* Runs a **20-cycle loop** simulating a dance:
  Each cycle consists of 4 forward movements and 4 left turns.
* Uses **real-time yaw tracking** to correct its orientation after each movement.
* Stops if it crosses a black line or detects a nearby object.
* Smooth and consistent due to gyro-based corrections.

---

## 🗂️ Functional Highlights

### Motion Functions

```cpp
moveForward(speedA, speedB, duration);
turnLeft(speed, duration);
turnRight(speed, duration);
```

Each movement is followed by **yaw correction** based on the MPU6050 reading to stay directionally accurate.

### Yaw Correction

```cpp
checkAndCorrectYaw(startYaw);
```

Compares initial and final yaw angles, then makes small turn adjustments if drift is detected.

### Obstacle Detection

```cpp
readDistanceCM();
```

Measures distance to objects using the ultrasonic sensor. If too close, robot stops.

### Line Detection

```cpp
readLineSensorsAndAct();
```

Reads left, center, and right IR sensors and stops the robot if a dark boundary line is crossed.

---

## 🎭 Performance Role

In the diversity showcase:

* The **Rio robot** is an expressive dancer.
* Its movement represents **Brazilian spirit**, joy, and freedom—aligned with cultural elements like Samba and Carnival.
* Performs alongside robots like the **Falcon (UAE)** or **rotating structures/buildings** in a collaborative stage sequence.

---

## 🛠️ Setup Instructions

1. Assemble robot with MPU6050, ultrasonic sensor, line sensors, and motor driver.
2. Connect all pins as defined in the code.
3. Ensure MPU6050 is properly calibrated.
4. Upload code and place robot on stage.
5. Watch the Rio robot **come alive** and perform!

---
