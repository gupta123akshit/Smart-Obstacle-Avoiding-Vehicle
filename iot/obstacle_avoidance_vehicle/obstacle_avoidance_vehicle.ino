#include <Servo.h>  
#include <AFMotor.h> 
// PIN DEFINITIONS 
#define Echo A0 
#define Trig A1 
#define motor 10    // Servo pin on Motor Shield 
#define Speed 170   // Motor speed (0-255) 
#define spoint 103  // Servo neutral angle 
// GLOBAL VARIABLES 
char value; 
int distance; 
int Left; 
int Right; 
int L = 0; 
int R = 0; 
// OBJECT INITIALIZATION 
Servo servo; 
AF_DCMotor M1(1); // Front-left 
AF_DCMotor M2(2); // Front-right 
AF_DCMotor M3(3); // Back-left 
AF_DCMotor M4(4); // Back-right 
void setup() { 
Serial.begin(9600); 
pinMode(Trig, OUTPUT); 
pinMode(Echo, INPUT); 
servo.attach(motor); 
M1.setSpeed(Speed); 
M2.setSpeed(Speed); 
M3.setSpeed(Speed); 
  M4.setSpeed(Speed); 
 
  Serial.println("ML Decision Tree Loaded - Bluetooth + Voice Control Active"); 
} 
 
void loop() { 
  // Functions for manual control (uncomment to use) 
  //Bluetoothcontrol(); 
  //voicecontrol(); 
   
  // Autonomous ML-based control 
  obstacleAvoidML(); 
} 
 
 
// 1. MANUAL BLUETOOTH CONTROL 
 
void Bluetoothcontrol() { 
  if (Serial.available() > 0) { 
    value = Serial.read(); 
    Serial.println(value); 
  } 
 
  if (value == 'F') forward(); 
  else if (value == 'B') backward(); 
  else if (value == 'L') left(); 
  else if (value == 'R') right(); 
  else if (value == 'S') Stop(); 
} 
 
 
// 2. MANUAL VOICE CONTROL 
 
void voicecontrol() { 
  if (Serial.available() > 0) { 
    value = Serial.read(); 
    Serial.println(value); 
 
    if (value == '^') { 
      forward(); 
    } else if (value == '-') { 
      backward(); 
    } else if (value == '<') { 
      L = leftsee(); 
      servo.write(spoint); 
      if (L >= 10) { 
        left(); 
        delay(500); 
        Stop(); 
      } else Stop(); 
41 
 
    } else if (value == '>') { 
      R = rightsee(); 
      servo.write(spoint); 
      if (R >= 10) { 
        right(); 
        delay(500); 
        Stop(); 
      } else Stop(); 
    } else if (value == '*') { 
      Stop(); 
    } 
  } 
} 
 
 
// 3. ML OBSTACLE AVOIDANCE 
 
void obstacleAvoidML() { 
  // 1. SENSE: Get distances from 3 angles 
  float distFront = getSensorReading(90); 
  float distLeft = getSensorReading(160); 
  float distRight = getSensorReading(20); 
 
  // 2. THINK: Get the ML model's decision 
  int action = mlPredict(distFront, distLeft, distRight); 
   
  // 3. ACT: Execute the decided action 
  executeMLAction(action, distFront, distLeft, distRight); 
} 
 
 
// 4. DECISION TREE LOGIC 
 
// Feature mapping-> 0=Front, 1=Left, 2=Right 
// Classes-> 0=Forward, 1=Backward, 2=Right, 3=Left 
int mlPredict(float distFront, float distLeft, float distRight) { 
 
  if (distFront <= 19.91) { 
    if (distLeft <= 28.44) { 
      if (distRight <= 28.39) { 
        if (distRight <= 13.52) { 
          if (distLeft <= 12.30) { 
            if (distLeft <= 6.96) { 
              return 3;   // LEFT 
            } else { 
              return 1;   // BACKWARD 
            } 
          } else { 
            return 2;     // RIGHT 
          } 
42 
 
        } else { 
          if (distLeft <= 11.85) { 
            if (distFront <= 12.57) { 
              return 3;   // LEFT 
            } else { 
              return 1;   // BACKWARD 
            } 
          } else { 
            if (distLeft <= 25.61) { 
              return 3;   // LEFT 
            } else { 
              return 2;   // RIGHT 
            } 
          } 
        } 
      } else { 
        if (distRight <= 46.66) { 
          if (distLeft <= 41.38) { 
            if (distRight <= 30.81) { 
              if (distRight <= 22.90) { 
                return 2;   // RIGHT 
              } else { 
                return 0;   // FORWARD 
              } 
            } else { 
              return 2;   // RIGHT 
            } 
          } else { 
            return 2;   // RIGHT 
          } 
        } else { 
          if (distFront <= 7.84) { 
            return 3;   // LEFT 
          } else { 
            return 2;   // RIGHT 
          } 
        } 
      } 
    } else { 
      if (distRight <= 59.63) { 
        if (distRight <= 55.12) { 
          if (distLeft <= 40.45) { 
            return 3;   // LEFT 
          } else { 
            return 3;   // LEFT 
          } 
        } else { 
          return 2;   // RIGHT 
        } 
      } else { 
43 
 
        if (distRight <= 81.02) { 
          if (distFront <= 12.50) { 
            return 3;   // LEFT 
          } else { 
            if (distFront <= 13.56) { 
              return 3;   // LEFT 
            } else { 
              return 2;   // RIGHT 
            } 
          } 
        } else { 
          if (distLeft <= 10.02) { 
            return 3;   // LEFT 
          } else { 
            if (distRight <= 25.30) { 
              return 2;   // RIGHT 
            } else { 
              return 2;   // RIGHT 
            } 
          } 
        } 
      } 
    } 
  } else { 
    if (distLeft <= 9.89) { 
      if (distRight <= 25.02) { 
        return 0;   // FORWARD 
      } else { 
        return 3;   // LEFT 
      } 
    } else { 
      return 0;   // FORWARD 
    } 
  } 
} 
 
// 5. ACTION EXECUTION FUNCTION 
 
void executeMLAction(int action, float dF, float dL, float dR) { 
  // Print debug information 
  Serial.print("Sensors: F="); 
  Serial.print(dF); 
  Serial.print(" L="); 
  Serial.print(dL); 
  Serial.print(" R="); 
  Serial.print(dR); 
  Serial.print(" | Action: "); 
 
  switch(action) { 
    case 0: 
44 
 
      Serial.println("FORWARD"); 
      forward(); 
      delay(300); // Move forward for a short duration 
      break; 
    case 1: 
      Serial.println("BACKWARD"); 
      backward(); 
      delay(400); // Back up 
      Stop(); 
      delay(200); 
      break; 
    case 2: 
      Serial.println("RIGHT"); 
      backward(); // Short reverse 
      delay(200); 
      right();    // Turn right 
      delay(500); 
      Stop(); 
      delay(200); 
      break; 
    case 3: 
      Serial.println("LEFT"); 
      backward(); // Short reverse 
      delay(200); 
      left();     // Turn left 
      delay(500); 
      Stop(); 
      delay(200); 
      break; 
  } 
} 
 
//  6. SENSOR AND MOTOR HELPER FUNCTIONS 
 
// Gets a sensor reading from a specific angle 
float getSensorReading(int angle) { 
  servo.write(angle); 
  delay(300); // Wait for servo to move 
  int dist = ultrasonic(); 
  servo.write(spoint); // Return servo to center 
  return (float)dist; 
} 
 
// Pings the ultrasonic sensor and returns distance in cm 
int ultrasonic() { 
  digitalWrite(Trig, LOW); 
  delayMicroseconds(4); 
  digitalWrite(Trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(Trig, LOW); 
45 
 
  long t = pulseIn(Echo, HIGH); 
  return t / 29 / 2; // Convert time to cm 
} 
 
//  Motor Control Functions  
void forward() { 
  M1.run(FORWARD); 
  M2.run(FORWARD); 
  M3.run(FORWARD); 
  M4.run(FORWARD); 
} 
 
void backward() { 
  M1.run(BACKWARD); 
  M2.run(BACKWARD); 
  M3.run(BACKWARD); 
  M4.run(BACKWARD); 
} 
 
void right() { 
  M1.run(BACKWARD); 
  M2.run(BACKWARD); 
  M3.run(FORWARD); 
  M4.run(FORWARD); 
} 
 
void left() { 
  M1.run(FORWARD); 
  M2.run(FORWARD); 
  M3.run(BACKWARD); 
  M4.run(BACKWARD); 
} 
 
void Stop() { 
  M1.run(RELEASE); 
  M2.run(RELEASE); 
  M3.run(RELEASE); 
  M4.run(RELEASE); 
} 
 
// Helper functions for voice control 
int rightsee() { 
  servo.write(20); 
  delay(800); 
  Right = ultrasonic(); 
  return Right; 
} 
 
int leftsee() { 
  servo.write(160); 
delay(800); 
Left = ultrasonic(); 
return Left; 
} 
