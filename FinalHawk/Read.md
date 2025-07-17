# 🦅 Falcon Robot — UAE Representative

**Part of "Birds of Diversity" Robotic Showcase**
**Theme:** *Celebrating Global Diversity Through Robotics and Avian Symbolism*

## 🌍 Project Overview

The *Falcon Robot* is an autonomous robot developed as part of a multicultural robotics showcase. Each robot represents a country and a bird symbolic to that region. The Falcon represents the **United Arab Emirates (UAE)**—a symbol of strength, vision, and cultural pride.

This robot is equipped with:

* Object tracking (via HUSKYLENS AI camera),
* Proximity and orientation detection (via ultrasonic and MPU6050 sensors),
* Line-following logic to maintain navigation boundaries.

It performs movements in coordination with other country-themed robots—such as a dancing **Rio** robot representing **Brazil**—to highlight **unity in diversity** through a synchronized performance.

---

## 🔧 Hardware Components

| Component                   | Purpose                             |
| --------------------------- | ----------------------------------- |
| **Arduino**                 | Main controller                     |
| **HUSKYLENS**               | Object tracking & recognition       |
| **Ultrasonic Sensor**       | Obstacle avoidance                  |
| **MPU6050**                 | Orientation and heading adjustment  |
| **Line Sensors (A0-A2)**    | Line detection & stopping mechanism |
| **Motor Driver (PWM pins)** | Robot movement control              |
| **DC Motors (2)**           | Drive system                        |

---

## 🚀 Features

* **Autonomous Movement:** Sequence of directional movements that simulate flight paths.
* **Obstacle Avoidance:** Stops or redirects upon detecting close obstacles.
* **Line Sensor Stop:** Stops when it detects a boundary line.
* **Heading Awareness:** Tracks and adjusts robot orientation.
* **Object Interaction:** Detects specific learned objects and reacts accordingly (e.g., moves toward a target).

---

## 🧠 Code Highlights

### Sensor Setup

* Initializes **HUSKYLENS** using I2C for object recognition.
* Configures **ultrasonic** sensor for proximity detection.
* Activates **MPU6050** for basic heading tracking and directional awareness.
* Reads **analog line sensors** for stopping at performance boundaries.

### Movement Logic

* `moveForward()`, `moveBackward()`, `turnLeft()`, `turnRight()`: Basic directional commands with integrated sensor checks.
* `updateHeadingFromTurn()`: Updates internal orientation when turning.
* `checkAndMoveIfObjectFound()`: Responds to a learned object in view.

### Performance Flow

* The `loop()` function defines a **choreographed path** simulating the Falcon’s movements, mixed with turns and pauses.
* After completing the flight pattern, the robot scans for a specific object and interacts accordingly.

---

## 🎭 Cultural & Educational Context

**The Falcon robot represents the UAE**, joining other country-themed robots (like the **Rio parrot from Brazil**) in a celebration of cultural diversity. Each robot's design and behavior align with its symbolic bird and national identity.

This robotics performance helps **teach diversity, cooperation, and global understanding** through an engaging tech-based narrative.

---

