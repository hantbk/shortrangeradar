# Short Range Ultrasonic Radar


## 📝 Table of Contents:
[A. Introduction](#I-Introduction)

[B. User Guide](#II-User-Guide)

[C. Component List](#III-Component-List)

[D. Schematic Diagram](#IV-Schematic-Diagram)

[E. Software Design](#V-Software-Design)

[F. Authors](#VI-Authors)

<a name="I-Introduction"></a>
## 🏁 A. Introduction
- This project uses an ultrasonic sensor to measure the distance from the sensor to obstacles within a range of 2cm to 50cm.
- Uses a servo motor to sweep a 180-degree angle.
- The measured distance will be displayed on a 1.3-inch OLED screen in the form of a horizontal scanning radar.

<a name="II-User-Guide"></a>
## 🧾 B. User Guide
Steps to implement:
- Connect the ultrasonic sensor to the circuit according to the schematic diagram.
- Connect the servo motor to the circuit according to the schematic diagram.
- Connect the OLED screen to the circuit according to the schematic diagram.
- Upload the code to the circuit.
- Place the circuit in a fixed position, do not move it.
- When an obstacle moves past the sensor, the screen will display a horizontal scanning radar image.

Pin connection convention:
| SR04 Ultrasonic Pin | Servo Pin    | OLED Pin    | ESP32 Pin |
| :---:   | :---: | :---: | :---: |
| VCC | VCC   | VCC   | 3.3V |
| Trig |   |  | 23   |
| Echo |   | | 18   |
| GND  | GND   | GND  | GND  |
|       |   | SDA  | 21   |
|       |  | SCK  | 22   |
|       | PWM  |  | D5   |

Connection diagram:
![Connection diagram](./simulation.png)

Actual image:

![Actual circuit](./radar.png)

Demo video:

https://github.com/DoDat-12/shortrangeradar/assets/89787228/abf60fdb-f4bd-479f-adab-8d7409c9bfb1



<a name="III-Component-List"></a>
## 🧰 C. Component List
1. HC-SR04 Ultrasonic Sensor
- Quantity: 1
- Purchase link: [HC-SR04](https://banlinhkien.com/cam-bien-sieu-am-srf04-p6646905.html)
2. SG90 Servo Motor
- Quantity: 1
- Purchase link: [SG90](https://banlinhkien.com/dong-co-servo-sg90-goc-quay-180-p6648774.html)
3. 1.3-inch OLED Screen
- Quantity: 1
- Purchase link: [1.3-inch OLED](https://banlinhkien.com/man-hinh-oled-1.3-128x64-i2c-xanh-oled13i2cb-p12496195.html)
4. ESP32 Board
- Quantity: 1
- Purchase link: [ESP32](https://banlinhkien.com/kit-wifi-esp32-espwroom32s-p6649289.html)
5. SYB-170 Test Board
- Quantity: 1
- Purchase link: [Test Board](https://banlinhkien.com/board-test-syb170-170-lo-4.7x3.5cm-p6649612.html)
6. Connecting Wires
- Quantity: multiple
- Purchase link: [Connecting Wires](https://banlinhkien.com/day-nhieu-mau-3p-xh2.54-20cm-p20789741.html)

<a name="IV-Schematic-Diagram"></a>
## 🚀 D. Schematic Diagram
Schematic diagram of the project:

![Schematic diagram](./nguyenly.png)

<a name="V-Software-Design"></a>
## 💻 E. Software Design
- The code is written in C++.
- Uses the `Ultrasonic.h` library to read data from the ultrasonic sensor.
- Uses the `ESP32Servo.h` library to control the servo motor.
- Uses the `U8g2lib.h` library to control the OLED screen.
- The code is divided into subroutines such as `setup()`, `loop()`, `GetDistance()`.
- The `GetDistance()` function will return the distance from the sensor to the obstacle.
- The `loop()` function will sweep the 180-degree angle of the servo motor and display the distance on the OLED screen.

The HC-SR04 ultrasonic sensor operates on the following principle:
- The sensor sends an ultrasonic signal to the obstacle through the `Trig` pin at a low level for 2uS within 10uS.
- The ultrasonic signal will reflect from the obstacle and return to the sensor.
- The sensor will read the returned value through the `Echo` pin with values ranging from 10 - 38 corresponding to physical distances from 2cm - 50cm.
```c
// Determine distance
int GetDistance() {
digitalWrite(SR04_TRIG_PIN, LOW); // Set Trig pin to low level for 2uS
delayMicroseconds(2);
digitalWrite(SR04_TRIG_PIN, HIGH); // Send ultrasonic wave for 10uS
delayMicroseconds(10);
digitalWrite(SR04_TRIG_PIN, LOW); // Turn off ultrasonic wave
unsigned int microseconds = pulseIn(SR04_ECHO_PIN, HIGH, 30000); // Wait for response, limit waiting time
return microseconds / 58; // Calculate distance from travel time
}
```
The SG90 servo motor operates on the following principle:
- The servo motor will sweep an angle from 0 to 180 degrees.
- The servo motor will sweep the angle according to the angle we have set in the code.
```c
// Initialize Servo
radarServo.attach(SERVO_PIN);
radarServo.write(angle);
```


The 1.3-inch OLED screen operates on the following principle:
- The OLED screen uses I2C communication.
- The OLED screen will display a horizontal scanning radar image.
- The OLED screen will display the distance on the OLED screen.
```c
dis = GetDistance();
    Serial.println(dis);

    // Draw radar beam
    if (dis >= MAX_DISTANCE) {
      int x = centerX - radius * cos(currentAngle * PI / 180);
      int y = centerY - radius * sin(currentAngle * PI / 180);
      u8g2.drawLine(centerX, centerY, x, y);
    } else {
      int x = centerX - radius * dis * cos(currentAngle * PI / 180) / MAX_DISTANCE;
      int y = centerY - radius * dis * sin(currentAngle * PI / 180) / MAX_DISTANCE;
      u8g2.drawLine(centerX, centerY, x, y);
    }
    u8g2.sendBuffer();
    radarServo.write(currentAngle);
```
Full code: [Code](./shortrangeradar.ino)

<a name="VI-Authors"></a>
## ✍️ F. Authors
- [Nguyen Thanh Ha](https://github.com/hantbk) - 20210298
- [Do Thanh Dat](https://github.com/DoDat-12) - 20215560
- [Vu Van Hao](https://github.com/vanhao2310) - 20215572
- [Quach Dinh Duong](https://github.com/Duongneee) - 20215558

⭐ If you use the project, please give us a star. Thank you!











