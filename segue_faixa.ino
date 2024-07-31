#include <Servo.h>

#define BLACK 1
#define WHITE 0

int sensor_left = A5;
int sensor_mid = A4;
int sensor_right = A3;

int led_right = 13;
int led_left = 2;

int in1 = 6;
int in2 = 7;
int in3 = 8;
int in4 = 9;
int ena = 5;
int enb = 11;

int left_value, mid_value, right_value;
int last_value = 0;

int read_left() {
  return analogRead(sensor_left) < 1010 ? WHITE : BLACK;
}

int read_mid() {
  return analogRead(sensor_mid) < 1010 ? WHITE : BLACK;
}

int read_right() {
  return analogRead(sensor_right) < 1010 ? WHITE : BLACK;
}

void speed(int speed) {
  analogWrite(ena, speed);
  analogWrite(enb, speed);
} 

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  speed(100);
}

void forward() {
  //direita
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  //esquerda
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  speed(100);

  delay(50);
  stop();
  delay(50);
}

void left() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  speed(100);

  delay(50);
  stop();
  delay(50);
}

void right() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  speed(100);

  delay(50);
  stop();
  delay(50);
}

int obstacle() {
  return !digitalRead(led_left) && !digitalRead(led_right);
}

void handle_obstacle() {
  int t = 0;

  while (t < 10) {
    left();
    delay(50);
    t++;
  }

  t = 0;

  while (t < 20) {
    forward();
    delay(50);
    t++;
  }
  
  t = 0;

  while (t < 20) {
    right();
    delay(50);
    t++;
  }

  while (!read_left() && !read_mid() && !read_right())
    forward();
}

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  pinMode(led_right, INPUT);
  pinMode(led_left, INPUT);

  Serial.begin(9600);
}

void loop() {
  if (!obstacle()) {
    if (read_left()) {
      left();
      last_value = 0;
    }
    else if (read_mid()) {
      forward();
      last_value = 1;
    }
    else if (read_right()) {
      right();
      last_value = 2;
    }
    else switch (last_value) {
      case 0:
        left();
        break;
      case 1:
        forward();
        break;
      case 2:
        right();
        break;
    }
  } else {
    handle_obstacle();
  }

  left_value = analogRead(sensor_left);
  mid_value = analogRead(sensor_mid);
  right_value = analogRead(sensor_right);

  Serial.print(left_value);
  Serial.print(" ");
  Serial.print(mid_value);
  Serial.print(" ");
  Serial.print(right_value);
  Serial.print(" ");
  Serial.print(read_left());
  Serial.print(" ");
  Serial.print(read_mid());
  Serial.print(" ");
  Serial.print(read_right());
  Serial.print(" ");
  Serial.print(obstacle());

  Serial.print("\n");
}
