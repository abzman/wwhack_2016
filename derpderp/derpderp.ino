#include <Servo.h> 
#include <Wire.h>
#include <stdio.h>
#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.

Servo swivel;

int pwm_speed = 6;   // Channel A speed
int pwm_steer = 9;   // Channel B speed
int dir_a0 = 4;  // Channel A direction 0
int dir_a1 = 5;  // Channel A direction 1
int dir_b0 = 7;  // Channel B direction 0
int dir_b1 = 8;  // Channel B direction 1

const int minPos = 60;
const int maxPos = 120;
int swivelpos = 90; // Servo position
const int jump = 30;
const int servoDelay = 180;
const int readDelay = 20;
const int turnDelay = 250;
const int minDiff = 10;
int scandir = 1;
const int N = ((maxPos-minPos)/jump)+1;

char chrBuf[3];

int scanA[N];
int scanB[N];
int scanC[N];

int newDist = 0;
int prevDist = 0;
int prevAngle = 0;

void setup()
{
  Serial.begin(115200);
  Wire.begin(); // join i2c bus
  swivel.attach(10); // Attach servo to pin 11
  swivel.write(swivelpos); 
  
  pinMode(pwm_speed, OUTPUT);  // Set control pins to be outputs
  pinMode(pwm_steer, OUTPUT);
  pinMode(dir_a0, OUTPUT);
  pinMode(dir_a1, OUTPUT);
  pinMode(dir_b0, OUTPUT);
  pinMode(dir_b1, OUTPUT);
  /*
  Serial.println("Scanning forward!");
  for (int i = 0; i < N; ++i) {
    scanA[i] = scan(minPos + jump*i);
    //scanB[i] = 5000;
    sprintf(chrBuf,"%3d",scanA[i]);
    Serial.print(chrBuf);
    Serial.print(" ");
  }
  Serial.println("");
  drive(50,0);
  */
}

int dAB, dAC, dBC;
int maxDist=0;
int maxAng = 0;
int a = 1;
void loop()
{
  // scan back and see if anything got shorter
  int f = scan(90);
  int l,r;
  if (f < 100) {
    brake();
    l = scan(45);
    r = scan(135);
    if (l > r) {
      if (l > f) {
        drive(200,255);
      } else {
        drive(200,0);
      }
    } else {
      if (r > f) {
        drive(200,-255);
      } else {
        drive(200,0);  
      }
    }
    if (l < 100 && r < 100) {
      drive(-255, 0);
      delay(5000);
    }
  } else {
      drive(255,0);
  }
  
  delay(500);
  /*
  for(int i = N-1; i>=0; --i){
    scanB[i] = scan(minPos + jump*i);
    sprintf(chrBuf,"%3d",scanB[i]);
    Serial.print(chrBuf);
    Serial.print(" ");
    if((scanA[i]-scanB[i]) > minDiff){
      // if so, avoid
      Serial.println("");
      Serial.print("Point ");
      Serial.print(i,DEC);
      Serial.print(" shorter! ");
      sprintf(chrBuf,"%3d",scanB[i] - scanA[i]);
      Serial.println(chrBuf);
      drive(50,-255);
      delay(turnDelay);
      //break;
    }else{
      // otherwise, keep on
      //drive(50,0);
    }
  }
  Serial.println("\n\nScanning forward!");

  // scan forward and see if anything got shorter
  for(int i = 0; i<N; ++i){
    scanA[i] = scan(minPos + jump*i);
    sprintf(chrBuf,"%3d",scanA[i]);
    Serial.print(chrBuf);
    Serial.print(" ");
    if((scanB[i]-scanA[i]) > minDiff){
      // if so, avoid
      Serial.println("");
      Serial.print("Point ");
      Serial.print(i,DEC);
      Serial.print(" shorter! ");
      sprintf(chrBuf,"%3d",scanA[i] - scanB[i]);
      Serial.println(chrBuf);
      drive(50,255);
      delay(turnDelay);
      //break;
    }else{
      // otherwise, keep on
     // drive(50,0);
    }
  }
  */
}

void brake() {
    digitalWrite(dir_a0, 1);
    digitalWrite(dir_a1, 1);
 
    digitalWrite(dir_b0, 1);
    digitalWrite(dir_b1, 1);
}
void drive(int speed, int direction)
{ 
  /*
 Serial.print("Speed: ");
 Serial.println(speed);
 Serial.print("Direction: ");
 Serial.println(direction);
*/
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
  analogWrite(pwm_steer, (abs(direction) > jump*3) ? 255 : 0); 
}


int scan(int position) {
    int reading;
    //Serial.println(position);
    swivel.write(position);
  
    delay(servoDelay); // Wait 20ms for positioning
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
  
    if(2 <= Wire.available()) // if two bytes were received
    {
      reading = Wire.read(); // receive high byte (overwrites previous reading)
      reading = reading << 8; // shift high byte to be high 8 bits
      reading |= Wire.read(); // receive low byte as lower 8 bits
      if (reading < 0 || reading > 4000) {
        reading = 0;
      }/**
      Serial.print(position);
      Serial.print(" -> ");
      Serial.println(reading);*/
      return reading;
    }
    Serial.println("ERROR_________________________________");
    return 0;
}

