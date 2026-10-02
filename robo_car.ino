#include <Servo.h>

// ── Pin definitions ───────────────────────────────────
#define TRIG_PIN   10
#define ECHO_PIN   9
#define SERVO_PIN  11

// L298N motor driver
#define IN1  4
#define IN2  3
#define IN3  8
#define IN4  7
#define ENA  5   // PWM speed Motor A
#define ENB  6   // PWM speed Motor B

#define SAFE_DIST  60    // cm — minimum clear distance
#define SPEED      140   // 0–255 (was 300 — invalid, max is 255)

Servo scanServo;

// ── Get distance from HC-SR04 ─────────────────────────
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2;
}

// ── Motor functions ───────────────────────────────────
void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// ── Slow servo movement ───────────────────────────────
void slowMove(int fromAngle, int toAngle, int stepDelay) {
  if (fromAngle < toAngle) {
    for (int pos = fromAngle; pos <= toAngle; pos++) {
      scanServo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = fromAngle; pos >= toAngle; pos--) {
      scanServo.write(pos);
      delay(stepDelay);
    }
  }
}

// ── Setup ─────────────────────────────────────────────
void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);

  scanServo.attach(SERVO_PIN);
  scanServo.write(90);
  delay(1000);

  Serial.begin(9600);
}

// ── Main loop ─────────────────────────────────────────
void loop() {
  long dist = getDistance();

  Serial.print("Distance:");
  Serial.print(dist);
  Serial.print(",SafeLimit:");
  Serial.println(60);

  if (dist > SAFE_DIST || dist == 0) {
    Serial.println("Status: FORWARD");
    moveForward();
  } else {
    stopMotors();
    Serial.println("Status: OBSTACLE - scanning...");
    delay(500);

    slowMove(90, 0, 10);
    delay(500);
    long rightDist = getDistance();
    Serial.print("Right dist: "); Serial.println(rightDist);

    slowMove(0, 180, 10);
    delay(500);
    long leftDist = getDistance();
    Serial.print("Left dist: "); Serial.println(leftDist);

    slowMove(180, 90, 10);
    delay(500);

    if (leftDist == 0 && rightDist == 0) {
      Serial.println("Status: BACKING UP");
      moveBackward();
      delay(500);
    } else if (leftDist > rightDist) {
      Serial.println("Status: TURNING LEFT");
      turnLeft();
      delay(500);
    } else {
      Serial.println("Status: TURNING RIGHT");
      turnRight();
      delay(500);
    }
    stopMotors();
  }

  delay(50);
}
