/*
 * esp-hc-sr04p
 *
 * A simple safe/unsafe distance indicator on an ESP8266 with 
 * a HC-SR04P ultrasonic distance sensor
 *
 * Copyright (c) 2021 Roozbeh Farahbod
 * Distributed under the MIT License.
 *
 */


// LED pins
#define RED_PIN D6
#define GRN_PIN D7

// HC-SR04 Trigger and Echo pins
#define TRIG_PIN D4                                        
#define ECHO_PIN D3                                        

// Serial baudrate 
#define BAUDRATE 9600                                      

// Distance constants
#define MIN_SAFE_DISTANCE 240
#define MAX_NORMAL_MEASURE 400

// States of the LEDS
#define STATE_UNKNOWN -1
#define STATE_SAFE 0
#define STATE_UNSAFE 1

// Debugging flag
#define DEBUG_FLAG true

long duration;
int distance;

void setup() {
  pinMode(GRN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  setLEDState(STATE_UNKNOWN);
  
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT); 
  Serial.begin(BAUDRATE);
  Serial.setTimeout(2000);

  // wait for serial to initialize.
  while(!Serial) { }
  
  log("Setting up..."); 
}

void loop() {
  // clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // set the trigger pin on HIGH for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // read the Echo Pin and receive the echo time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  
  distance = duration / (2.0 * 29.1);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= MAX_NORMAL_MEASURE) {
    log("Normal reading.");
    if (distance < MIN_SAFE_DISTANCE) {
      log("Unsafe distance.");
      setLEDState(STATE_UNSAFE);   
    } else {
      log("Safe distance.");
      setLEDState(STATE_SAFE);   
    }
  } else {
    log("Abnormal reading.");
    setLEDState(STATE_UNKNOWN);
  }
  
  delay(250);
}

// setLEDState sets the LED powers based on the given state
void setLEDState(int state) {
  if (state == STATE_SAFE) {
    red(false);
    green(true);
  } else {
    if (state == STATE_UNSAFE) {
      green(false);
      red(true);
    } else {
      red(false);
      green(false);
    }
  }
}

// red turns the red LED on or off
void red(boolean state) {
  led(RED_PIN, state); 
}

// green turns the green LED on or off
void green(boolean state) {
  led(GRN_PIN, state); 
}

// led turns the LED on the given pin on (true) or off (false)
void led(int pin, boolean state) {
  if (state) {
      digitalWrite(pin, HIGH); 
  } else {
      digitalWrite(pin, LOW); 
  }
}

// log logs the message based on the debug flag
void log(char* msg) {
  if (DEBUG_FLAG) {
    Serial.println(msg);  
  }
}
