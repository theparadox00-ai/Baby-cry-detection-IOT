Smart Baby Monitor: Edge AI Cry Translator on ESP32-S3

Overview
This project implements an offline, privacy-first baby monitor using the ESP32-S3 FireBeetle 2. The system uses embedded Deep Learning to detect baby cries and classify the specific reason for the cry. Unlike cloud-based monitors, this device performs all processing locally on the edge to ensure low latency and data privacy.

System Architecture: Cascade Logic
The system utilizes a dual-stage neural network pipeline to optimize performance:

1. Stage 1: Binary Classification
Function: This model works first to identify whether the detected sound is a cry or background noise.
Performance: Achieved 100% accuracy during testing.
Role: Acts as a gatekeeper to prevent false alarms and unnecessary processing.

2. Stage 2: Multi-Class Classification
Function: If Stage 1 confirms a cry, this secondary model analyzes the audio to determine the specific cause.
Classes: Hungry, Tired, Belly Pain, Discomfort, Burping.
Performance: Achieved 80% accuracy during testing.

Hardware Specifications
Microcontroller: DFRobot FireBeetle 2 (ESP32-S3)
Microphone: ICS-43434 (I2S Digital Microphone)
Battery Power: JST-PH 2.0 connector with a 523450 Li-Po battery. This ensures the device continues running during power outages.
External Power: The system supports a standard 5V/1A external power adapter, which can power the device and charge the battery simultaneously.

Current Status
The device currently performs real-time inference using the binary and multi-class models. The audio processing pipeline (I2S capture, DSP feature extraction, and inference) is fully operational on the ESP32-S3.

Future Implementation
The following features are planned for the next phase of development to enhance power efficiency and user connectivity:
Power Saving Duty Cycle: To extend battery life on the 523450 Li-Po cell, the system will implement a sleep logic where the device sleeps for 30 seconds and wakes up for 5 seconds to listen for audio events.

Dataset Link : https://www.kaggle.com/datasets/mennaahmed23/baby-cry-dataset

Bluetooth Notifications: Upon detecting and classifying a cry, the system will transmit a text notification directly to the parent's smartphone via Bluetooth, providing an offline alert mechanism.
