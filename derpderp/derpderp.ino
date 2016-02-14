#include <Servo.h> 
#include <Wire.h>

#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.

Servo scanner;

const int servoPin = 10;
const int pwm_speed = 6;   // Channel A speed
const int pwm_steer = 9;   // Channel B speed
const int dir_a0 = 4;  // Channel A direction 0
const int dir_a1 = 5;  // Channel A direction 1
const int dir_b0 = 7;  // Channel B direction 0
const int dir_b1 = 8;  // Channel B direction 1

const int leftPosition = 45;
const int rightPosition = 135;
const int centerPosition = 90;

const int directions[] = {-255, 0, 255};

const int servoDelay = 180;
const int readDelay = 20;
const int turnDelay = 250;
const int minDiff = 10;
int f, l, r;

void setup() {
  Serial.begin(115200);
  Wire.begin(); // join i2c bus
  scanner.attach(servoPin); // Attach servo to pin 10
  scanner.write(centerPosition); 
  
  pinMode(pwm_speed, OUTPUT);  // Set control pins to be outputs
  pinMode(pwm_steer, OUTPUT);
  pinMode(dir_a0, OUTPUT);
  pinMode(dir_a1, OUTPUT);
  pinMode(dir_b0, OUTPUT);
  pinMode(dir_b1, OUTPUT);
}

void loop() {
  f = scan(90);
  if (f < 100) {
    brake();
    l = scan(45);
    r = scan(135);
    if (l > r) {
      if (l > f) {
        drive(64,255);
      } else {
        drive(64,directions[random(3)]);
      }
    } else {
      if (r > f) {
        drive(64,-255);
      } else {
        drive(64,directions[random(3)]);  
      }
    }
    if (l < 100 && r < 100) {
      drive(-64, directions[random(3)]);
      delay(1000);
    }
  } else {
      drive(90,0);
  }
  
  delay(500);
}

void brake() {
    Serial.println("Braking...");
    digitalWrite(dir_a0, 1);
    digitalWrite(dir_a1, 1);
 
    digitalWrite(dir_b0, 1);
    digitalWrite(dir_b1, 1);
}

void drive(int speed, int direction) {
  Serial.print("Speed: ");
  Serial.println(speed);
  Serial.print("Direction: ");
  Serial.println(direction);
  if (speed >= 0) {
    digitalWrite(dir_a0, 0);
    digitalWrite(dir_a1, 1);
  } else {
    digitalWrite(dir_a0, 1);
    digitalWrite(dir_a1, 0);
  }
  
  if (direction >= 0) {
    digitalWrite(dir_b0, 0);
    digitalWrite(dir_b1, 1);
  } else {
    digitalWrite(dir_b0, 1);
    digitalWrite(dir_b1, 0);
  }
  
  analogWrite(pwm_speed, abs(speed)); 
  analogWrite(pwm_steer, abs(direction)); 
}

int scan(int position) {
    int reading;
    scanner.write(position);
  
    delay(servoDelay); // Wait for positioning
    
    Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
    Wire.write((int)RegisterMeasure); // sets register pointer to  (0x00)  
    Wire.write((int)MeasureValue); // sets register pointer to  (0x00)  
    Wire.endTransmission(); // stop transmitting
  
    delay(readDelay); // Wait 20ms for transmit
  
    Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
    Wire.write((int)RegisterHighLowB); // sets register pointer to (0x8f)
    Wire.endTransmission(); // stop transmitting
  
    delay(readDelay); // Wait 20ms for transmit
  
    Wire.requestFrom((int)LIDARLite_ADDRESS, 2); // request 2 bytes from LIDAR-Lite
  
    if(2 <= Wire.available()) {
      reading = Wire.read(); // receive high byte (overwrites previous reading)
      reading = reading << 8; // shift high byte to be high 8 bits
      reading |= Wire.read(); // receive low byte as lower 8 bits
      if (reading < 0 || reading > 4000) {
        reading = 0;
      }
      Serial.print("Scan Angle: ");
      Serial.println(position);
      Serial.print("Distance: ");
      Serial.println(reading);
      return reading;
    }
    return 0;
}

