# robo_car
# 🤖 Obstacle Avoiding Robot Car

An autonomous robot car that detects obstacles using an ultrasonic sensor and automatically changes its direction to avoid collisions.

## Features

- Autonomous movement
- Real-time obstacle detection
- Ultrasonic distance sensing
- Automatic obstacle avoidance
- DC motor control using a motor driver

## Components Used

- Arduino / ESP32
- Ultrasonic Sensor
- Motor Driver
- DC Motors
- Robot Car Chassis
- Battery

## How It Works

The ultrasonic sensor continuously measures the distance in front of the car.

- If the path is clear, the car moves forward.
- When an obstacle is detected within the defined distance, the controller changes the motor direction.
- The car turns and continues moving while avoiding the obstacle.

## Hardware Setup

```text
        Ultrasonic Sensor
                │
                ▼
        Arduino / ESP32
                │
                ▼
          Motor Driver
           │       │
           ▼       ▼
        Motor 1  Motor 2
