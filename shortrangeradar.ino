#define OLED_SCK_PIN 22
#define OLED_SDA_PIN 21

#define SR04_TRIG_PIN 23 // Vị trí chân GPIO của ESP32 được nối với Trig của SR04
#define SR04_ECHO_PIN 18 // Vị trí chân GPIO của ESP32 được nối với Echo của SR04
#define SERVO_PIN 5

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Math.h>
#include <ESP32Servo.h>

// U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE, OLED_SCK_PIN, OLED_SDA_PIN);

#define U8LOG_WIDTH 128
#define U8LOG_HEIGHT 64
#define MAX_DISTANCE 50
uint8_t u8log_buffer[U8LOG_WIDTH * U8LOG_HEIGHT];
U8X8LOG u8x8log;

// Khởi tạo đối tượng U8G2 cho SH1106 I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

// Servo
Servo radarServo;

// Tọa độ tâm của radar
const int centerX = 64;
const int centerY = 64;
const int radius = 64;
int angle = 0;
int dis;
unsigned long previousMillis = 0; // Để quản lý thời gian không đồng bộ

void setup()
{
  Serial.begin(115200);
  // SR04 phát xung âm khi có tín hiệu điều khiển từ chân Trig, và nhận siêu âm
  // phản hồi thì báo về qua Echo. Vì vậy Trig là chân OUTPUT, còn Echo là chân INPUT
  pinMode(SR04_TRIG_PIN, OUTPUT);
  pinMode(SR04_ECHO_PIN, INPUT);

  // Khởi động màn hình OLED
  u8g2.begin();
  u8g2.drawCircle(centerX, centerY, radius, U8G2_DRAW_ALL);
  u8x8log.setRedrawMode(1); // 0: Update screen with newline, 1: Update screen for every char

  // Khởi động Servo
  radarServo.attach(SERVO_PIN);
  radarServo.write(angle);
}

void loop()
{
  static int step = 1;         // Biến để theo dõi hướng quay (1 là tăng, -1 là giảm)
  static int currentAngle = 0; // Biến để theo dõi góc hiện tại
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 40)
  { // delay
    previousMillis = currentMillis;

    dis = GetDistance();
    Serial.println(dis);

    // Vẽ tia radar
    if (dis >= MAX_DISTANCE)
    {
      int x = centerX - radius * cos(currentAngle * PI / 180);
      int y = centerY - radius * sin(currentAngle * PI / 180);
      u8g2.drawLine(centerX, centerY, x, y);
    }
    else
    {
      int x = centerX - radius * dis * cos(currentAngle * PI / 180) / MAX_DISTANCE;
      int y = centerY - radius * dis * sin(currentAngle * PI / 180) / MAX_DISTANCE;
      u8g2.drawLine(centerX, centerY, x, y);
    }
    u8g2.sendBuffer();
    radarServo.write(currentAngle);

    // Cập nhật góc cho lần quét tiếp theo
    currentAngle += step;
    if (currentAngle >= 180 || currentAngle <= 0)
    {
      step = -step; // Đảo chiều quay
      // Xóa buffer và vẽ lại vòng tròn radar
      u8g2.clearBuffer();
      u8g2.drawCircle(centerX, centerY, radius, U8G2_DRAW_ALL);
    }
  }
}

// Xác định khoảng cách
int GetDistance()
{
  digitalWrite(SR04_TRIG_PIN, LOW); // Đưa chân Trig xuống mức thấp trong 2uS
  delayMicroseconds(2);
  digitalWrite(SR04_TRIG_PIN, HIGH); // Gửi luồng siêu âm kéo dài 10uS
  delayMicroseconds(10);
  digitalWrite(SR04_TRIG_PIN, LOW);                                // Tắt luồng siêu âm
  unsigned int microseconds = pulseIn(SR04_ECHO_PIN, HIGH, 30000); // Đợi cho tới khi có phản hồi, giới hạn thời gian chờ
  return microseconds / 58;                                        // Từ thời gian hành trình tính toán khoảng cách
}
