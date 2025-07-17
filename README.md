# 🐦 Birds of Diversity — Robotics Showcase

## 🌍 Overview

**Birds of Diversity** is an educational robotics project that celebrates **global cultures and unity** through autonomous robots modeled after symbolic birds from different countries.

Each robot is designed to:

* Represent a **bird** significant to a specific country or region,
* Demonstrate **autonomous behavior** using sensors and AI,
* **Interact with its environment** in a way that reflects its character,
* Participate in a **choreographed performance** with other robots.

Together, these robots form a dynamic show promoting **inclusion, diversity, and cross-cultural appreciation** through engineering, creativity, and storytelling.

---

## 🤖 Core Features

* **Autonomous Navigation**: Robots follow a predefined path and respond to environmental cues.
* **Obstacle Avoidance**: Using ultrasonic sensors to detect and avoid obstacles.
* **Line Detection**: Robots stop or redirect when crossing specific stage lines.
* **Object Tracking**: Integration with AI camera (HUSKYLENS) to recognize and react to visual targets.
* **Heading & Orientation**: Maintain directional awareness via an MPU6050 sensor.
* **Modular Movement System**: Forward, backward, and turn functions using dual DC motors.

---

## 🔧 Hardware Components (Common Across All Robots)

| Component               | Purpose                                  |
| ----------------------- | ---------------------------------------- |
| **Arduino Board**       | Main microcontroller                     |
| **HUSKYLENS AI Camera** | Object recognition and tracking          |
| **Ultrasonic Sensor**   | Proximity detection (obstacle avoidance) |
| **MPU6050**             | Orientation and heading tracking         |
| **IR Line Sensors**     | Line detection for navigation control    |
| **Motor Driver**        | Motor control using PWM and logic pins   |
| **DC Motors**           | Drive system for movement                |

---

## 🧠 How It Works

Each robot runs its own program built around shared behavior modules:

* **Sensor Setup**: Initializes all sensors and performs connection checks.
* **Movement Commands**: Defined functions to control motors for turning, moving, or stopping.
* **Interaction Logic**: Robot behavior adapts based on input from line sensors, object recognition, or distance detection.
* **Performance Sequence**: The robot performs a choreographed routine, optionally reacting to visual cues mid-performance.

---

## 🎭 Educational & Cultural Theme

Each robot is themed around a bird that symbolizes a specific country's **identity, values, or heritage**. For example:

* 🦅 **Falcon** for the **UAE** – symbolizing strength and vision.
* 🦜 **Parrot ("Rio")** for **Brazil** – representing color, energy, and joy.
* 🕊️ Other birds may be added to expand the diversity of the performance.

This initiative is designed to:

* Encourage STEM engagement,
* Foster global awareness,
* Teach the importance of **cultural representation** in technology.

