# Parking-Management-System
 using Aws
# 🚘 Parking Management System

A smart IoT-based system to monitor and manage parking space availability in real time. Built using NodeMCU, ultrasonic sensors, and AWS cloud services, this project helps users reserve, track, and manage parking slots via a web interface.

---

## 📖 Overview

This system:
- Detects car presence using ultrasonic sensors.
- Displays live slot availability on a web dashboard.
- Allows users to reserve slots with expiry timers.
- Stores and retrieves data using AWS services.

---

## 🔧 Technologies Used

### Hardware:
- NodeMCU ESP8266
- Ultrasonic Sensor (HC-SR04)
- Buzzer (for alerts)

### Cloud & Backend:
- AWS IoT Core
- AWS Lambda
- AWS DynamoDB
- RESTful API

### Frontend:
- HTML, CSS, JavaScript
- Responsive Web Dashboard

---

## ⚙️ Setup Instructions

### 1. Microcontroller Setup
- Write code in Arduino IDE for NodeMCU.
- Connect ultrasonic sensor and buzzer to the NodeMCU.
- Upload code with WiFi credentials and AWS endpoint.

### 2. AWS Setup
- Register device in **AWS IoT Core** and generate certificates.
- Create **DynamoDB** tables:
  - `ParkingSpaces` (for slot status)
  - `ParkingReservations` (for reservations)
- Deploy **Lambda functions** to:
  - Handle sensor updates
  - Process reservations and timer expiry

### 3. Web Interface
- Open `index.html` in a browser.
- Connect it to your deployed API endpoints.
- View parking status, make reservations, and check logs.

---

## 🖼️ Documents 


- Parking Management.docx contains the enough information about the project and Screenshots , please refer that 

- **aws/directory**  
  inside the aws directory there will be code for frontend development.

---

## 👤 Author

**Your Name**  
GitHub:Sudharsanan(https://github.com/susa0676)

---

## 📝 License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
