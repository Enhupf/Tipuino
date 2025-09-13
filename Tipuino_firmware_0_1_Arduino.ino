//Tipunino firmware that allowed the first full box
//pin number for Arduino Mega

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

  // --- Pin Definitions ---
  // Dispenser Axis
  #define DISPENSER_STEP_PIN         9
  #define DISPENSER_DIR_PIN          8
  #define DISPENSER_ENABLE_PIN       10
  #define DISPENSER_LIMIT_SWITCH_PIN 2
  #define DISPENSER_END_SWITCH_PIN   3
  #define DISPENSER_BEAM_PIN         4

  // Box Axis
  #define BOX_STEP_PIN               22
  #define BOX_DIR_PIN                23
  #define BOX_ENABLE_PIN             24
  #define BOX_LIMIT_SWITCH_PIN       25
  #define BOX_ENCODER_PIN            26

  // Screw Axis
  #define SCREW_STEP_PIN             5
  #define SCREW_DIR_PIN              6
  #define SCREW_ENABLE_PIN           7
  #define SCREW_ENCODER_PIN          11

  // Wheel Axis
  #define WHEEL_STEP_PIN             13
  #define WHEEL_DIR_PIN              A1
  #define WHEEL_ENABLE_PIN           A2
  #define WHEEL_ENCODER_PIN          A3
  #define WHEEL_BEAM_PIN             12

  // --- Parameters ---
  // Dispenser
  #define DISPENSE_SPEED             4
  #define DISPENSE_PAUSE_MS          600
  #define MAX_DISPENSE_CYCLES        8
  #define DISPENSE_EXTRA_STEPS       10650
  #define DISPENSER_END_WIGGLE       2000
  #define DISPENSER_WIGGLE           400
  #define FINAL_HOMING_DELAY_MS      1000
  #define DISPENSER_CLEAR_STEPS	     300

  //Screw
  #define SCREW_EXTRA_STEPS          60
  #define SCREW_SPEED                2

  // Box
  #define BOX_SPEED                  2
  #define BOX_CLEAR_EXTRA_STEPS      50
  #define BOX_WIGGLE                 400

  // Wheel
  #define WHEEL_SPEED                2
  #define WHEEL_CLEAR_EXTRA_STEPS    700
  #define WHEEL_MOVE_PAUSE_MS        1000
  #define WHEEL_WIGGLE               50

  // Sequencing
  #define BOX_MOVES                  12
  #define DEBOUNCE_DELAY             50
  #define DISPENSER_BEAM_TIMEOUT_MS  1000
  #define WHEEL_BEAM_TIMEOUT_MS      500

  // --- Globals ---
  bool dispenserHomed = false;
  int dispenseCount = 0;

  void setup() {
   lcd.init();          // or lcd.begin(16, 2); depending on your library
   lcd.backlight();     // turn on the backlight
   lcd.clear();         // optional
   lcd.setCursor(0, 0);
   lcd.print("Tipuino starting");


    // Dispenser pins
    pinMode(DISPENSER_STEP_PIN, OUTPUT);
    pinMode(DISPENSER_DIR_PIN, OUTPUT);
    pinMode(DISPENSER_ENABLE_PIN, OUTPUT);
    pinMode(DISPENSER_LIMIT_SWITCH_PIN, INPUT_PULLUP);
    pinMode(DISPENSER_END_SWITCH_PIN, INPUT_PULLUP);
    pinMode(DISPENSER_BEAM_PIN, INPUT_PULLUP);

    // Box pins
    pinMode(BOX_STEP_PIN, OUTPUT);
    pinMode(BOX_DIR_PIN, OUTPUT);
    pinMode(BOX_ENABLE_PIN, OUTPUT);
    pinMode(BOX_LIMIT_SWITCH_PIN, INPUT_PULLUP);
    pinMode(BOX_ENCODER_PIN, INPUT_PULLUP);

    // Screw pins
    pinMode(SCREW_STEP_PIN, OUTPUT);
    pinMode(SCREW_DIR_PIN, OUTPUT);
    pinMode(SCREW_ENABLE_PIN, OUTPUT);
    pinMode(SCREW_ENCODER_PIN, INPUT_PULLUP);

    // Wheel pins
    pinMode(WHEEL_STEP_PIN, OUTPUT);
    pinMode(WHEEL_DIR_PIN, OUTPUT);
    pinMode(WHEEL_ENABLE_PIN, OUTPUT);
    pinMode(WHEEL_ENCODER_PIN, INPUT_PULLUP);
    pinMode(WHEEL_BEAM_PIN, INPUT_PULLUP);

    digitalWrite(SCREW_ENABLE_PIN, HIGH);
    digitalWrite(DISPENSER_ENABLE_PIN, HIGH);
    digitalWrite(BOX_ENABLE_PIN, HIGH);
    digitalWrite(WHEEL_ENABLE_PIN, HIGH);

    lcd.setCursor(0, 1);
    lcd.print("homing dispenser");
    homeDispenserMotor();
    dispenserHomed = true;
    lcd.setCursor(0, 1);
    lcd.print("homing box      ");
    homeBoxMotor();
    moveBoxToEncoderClear();
    digitalWrite(BOX_ENABLE_PIN, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("homing screw    ");
    dispenseCount = 0;    
    homeScrewMotor();    
      digitalWrite(DISPENSER_ENABLE_PIN, LOW);
      digitalWrite(DISPENSER_DIR_PIN, HIGH);
      for (int i = 0; i < DISPENSER_WIGGLE; i++) stepDispenser();
      delay(50);
      digitalWrite(DISPENSER_DIR_PIN, LOW);
      for (int i = 0; i < DISPENSER_WIGGLE; i++) stepDispenser();
      digitalWrite(DISPENSER_DIR_PIN, HIGH);
      digitalWrite(DISPENSER_ENABLE_PIN, HIGH);

      unsigned long startWait = millis();
      while (digitalRead(DISPENSER_BEAM_PIN) == HIGH && millis() - startWait < DISPENSER_BEAM_TIMEOUT_MS) {}
      if (digitalRead(DISPENSER_BEAM_PIN) == LOW) {
        delay(DISPENSE_PAUSE_MS);
        moveDispenserToNextClear();
        dispenseCount++;
      }
    lcd.setCursor(0, 0);
    lcd.print("filling box     ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  void loop() {
    lcd.setCursor(0, 0);
    lcd.print("filling box     ");
    for (int cycle = 0; cycle < BOX_MOVES; cycle++) {
      lcd.setCursor(0, 1);
      lcd.print("rows: ");
      lcd.print(cycle);
      if (dispenseCount > 0 and dispenseCount < 2){
      }
      else{
        dispenseCount = 0;
        }
      lcd.setCursor(7, 1);
      lcd.print(" tips: ");
      lcd.print(dispenseCount);
      while (dispenseCount < MAX_DISPENSE_CYCLES) {
        moveWheelToNextClearPosition();
        if (digitalRead(WHEEL_BEAM_PIN) == LOW) {
          digitalWrite(WHEEL_ENABLE_PIN, HIGH);
          digitalWrite(SCREW_ENABLE_PIN, LOW);
          homeScrewMotor();

          delay(200);
          digitalWrite(DISPENSER_ENABLE_PIN, LOW);
          digitalWrite(DISPENSER_DIR_PIN, HIGH);
          for (int i = 0; i < DISPENSER_WIGGLE; i++) stepDispenser();
          delay(50);
          digitalWrite(DISPENSER_DIR_PIN, LOW);
          for (int i = 0; i < DISPENSER_WIGGLE; i++) stepDispenser();
          digitalWrite(DISPENSER_DIR_PIN, HIGH);
          digitalWrite(DISPENSER_ENABLE_PIN, HIGH);

          unsigned long startWait = millis();
          while (digitalRead(DISPENSER_BEAM_PIN) == HIGH && millis() - startWait < DISPENSER_BEAM_TIMEOUT_MS) {}
          if (digitalRead(DISPENSER_BEAM_PIN) == LOW) {
            delay(DISPENSE_PAUSE_MS);
            moveDispenserToNextClear();
            dispenseCount++;
            lcd.setCursor(14, 1);
            lcd.print(dispenseCount);
            }
        }

        if (digitalRead(DISPENSER_BEAM_PIN) == LOW) {
          delay(DISPENSE_PAUSE_MS);
          moveDispenserToNextClear();
          dispenseCount++;
          digitalWrite(WHEEL_ENABLE_PIN, LOW);
          delay(10);
        }
      }
      digitalWrite(WHEEL_ENABLE_PIN, HIGH);
      digitalWrite(DISPENSER_ENABLE_PIN, LOW);
      delay(100);
      while (digitalRead(DISPENSER_END_SWITCH_PIN) == HIGH) stepDispenser();
      delay(50);

      digitalWrite(BOX_ENABLE_PIN, LOW);
      for (int i = 0; i < BOX_WIGGLE; i++) stepBoxMotor();
      delay(50);
      digitalWrite(BOX_DIR_PIN, HIGH);
      for (int i = 0; i < BOX_WIGGLE; i++) stepBoxMotor();
      digitalWrite(BOX_ENABLE_PIN, HIGH);
      digitalWrite(BOX_DIR_PIN, LOW);

      digitalWrite(DISPENSER_DIR_PIN, LOW);
      for (int i = 0; i < DISPENSER_END_WIGGLE; i++) stepDispenser();
      delay(50);
      digitalWrite(DISPENSER_DIR_PIN, HIGH);
      for (int i = 0; i < DISPENSER_END_WIGGLE; i++) stepDispenser();
      delay(FINAL_HOMING_DELAY_MS);

      digitalWrite(BOX_ENABLE_PIN, LOW);
      for (int i = 0; i < BOX_WIGGLE; i++) stepBoxMotor();
      delay(50);
      digitalWrite(BOX_DIR_PIN, HIGH);
      for (int i = 0; i < BOX_WIGGLE; i++) stepBoxMotor();
      digitalWrite(BOX_ENABLE_PIN, HIGH);
      digitalWrite(BOX_DIR_PIN, LOW);

      digitalWrite(DISPENSER_DIR_PIN, LOW);
      for (int i = 0; i < DISPENSER_END_WIGGLE; i++) stepDispenser();
      delay(50);
      digitalWrite(DISPENSER_DIR_PIN, HIGH);
      for (int i = 0; i < DISPENSER_END_WIGGLE; i++) stepDispenser();
      delay(FINAL_HOMING_DELAY_MS);
      homeDispenserMotor();
      digitalWrite(BOX_ENABLE_PIN, LOW);
      moveBoxToEncoderClear();
      digitalWrite(BOX_ENABLE_PIN, HIGH);
      digitalWrite(WHEEL_ENABLE_PIN, LOW);
      delay(500);
    }

    lcd.setCursor(0, 0);
    lcd.print("box full        ");
    lcd.setCursor(0, 1);
    lcd.print("happy pipetting ");

    while (true) {}
  }

  void homeDispenserMotor() {
    digitalWrite(DISPENSER_ENABLE_PIN, LOW);
    digitalWrite(DISPENSER_DIR_PIN, LOW);
    while (digitalRead(DISPENSER_LIMIT_SWITCH_PIN) == HIGH) stepDispenser();
    digitalWrite(DISPENSER_DIR_PIN, HIGH);
    while (digitalRead(DISPENSER_BEAM_PIN) == LOW) stepDispenser();
    delay(DISPENSE_PAUSE_MS);
    for (int i = 0; i < DISPENSER_CLEAR_STEPS; i++) stepDispenser();
    digitalWrite(DISPENSER_ENABLE_PIN, HIGH);
  }

  void homeBoxMotor() {
    digitalWrite(BOX_ENABLE_PIN, LOW);
    digitalWrite(BOX_DIR_PIN, HIGH);
    while (digitalRead(BOX_LIMIT_SWITCH_PIN) == HIGH) stepBoxMotor();
    digitalWrite(BOX_DIR_PIN, LOW);
    while (digitalRead(BOX_ENCODER_PIN) == HIGH) stepBoxMotor();
    delay(DEBOUNCE_DELAY);
    while (digitalRead(BOX_ENCODER_PIN) == LOW) stepBoxMotor();
    for (int i = 0; i < BOX_CLEAR_EXTRA_STEPS; i++) stepBoxMotor();
    digitalWrite(BOX_ENABLE_PIN, HIGH);
  }

  void homeScrewMotor() {
    digitalWrite(SCREW_ENABLE_PIN, LOW);
    digitalWrite(SCREW_DIR_PIN, LOW);
    while (digitalRead(SCREW_ENCODER_PIN) == LOW) stepScrewMotor();
    while (digitalRead(SCREW_ENCODER_PIN) == HIGH) stepScrewMotor();
    for (int i = 0; i < SCREW_EXTRA_STEPS; i++) stepScrewMotor();
    digitalWrite(SCREW_ENABLE_PIN, HIGH);
  }

  void moveDispenserToNextClear() {
    digitalWrite(DISPENSER_ENABLE_PIN, LOW);
    while (digitalRead(DISPENSER_BEAM_PIN) == LOW) stepDispenser();
    delay(DISPENSE_PAUSE_MS);
    for (int i = 0; i < DISPENSER_CLEAR_STEPS; i++) stepDispenser();
    digitalWrite(DISPENSER_ENABLE_PIN, HIGH);
  }

  void moveBoxToEncoderClear() {
    digitalWrite(BOX_ENABLE_PIN, LOW);
    while (digitalRead(BOX_ENCODER_PIN) == HIGH) stepBoxMotor();
    delay(DEBOUNCE_DELAY);
    while (digitalRead(BOX_ENCODER_PIN) == LOW) stepBoxMotor();
    for (int i = 0; i < BOX_CLEAR_EXTRA_STEPS; i++) stepBoxMotor();
    digitalWrite(BOX_ENABLE_PIN, HIGH);
  }

  void moveWheelToNextClearPosition() {
    digitalWrite(WHEEL_ENABLE_PIN, LOW);
    digitalWrite(WHEEL_DIR_PIN, LOW);
    for (int i = 0; i < WHEEL_WIGGLE; i++) stepWheelMotor();
    digitalWrite(WHEEL_DIR_PIN, HIGH);

    unsigned long startWheelMove = millis();
    while (millis() - startWheelMove < WHEEL_BEAM_TIMEOUT_MS) {
      while (digitalRead(WHEEL_ENCODER_PIN) == HIGH) stepWheelMotor();
      delay(DEBOUNCE_DELAY);
      while (digitalRead(WHEEL_ENCODER_PIN) == LOW) stepWheelMotor();
      for (int i = 0; i < WHEEL_CLEAR_EXTRA_STEPS; i++) stepWheelMotor();
      delay(WHEEL_MOVE_PAUSE_MS);
      return;
    }
  digitalWrite(WHEEL_ENABLE_PIN, HIGH);
  }


  void stepDispenser() {
    digitalWrite(DISPENSER_STEP_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(DISPENSER_STEP_PIN, LOW);
    delayMicroseconds(1000 / DISPENSE_SPEED);
  }

  void stepBoxMotor() {
    digitalWrite(BOX_STEP_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(BOX_STEP_PIN, LOW);
    delayMicroseconds(1000 / BOX_SPEED);
  }

  void stepScrewMotor() {
    digitalWrite(SCREW_STEP_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(SCREW_STEP_PIN, LOW);
    delayMicroseconds(1200 / SCREW_SPEED);
  }

  void stepWheelMotor() {
    digitalWrite(WHEEL_STEP_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(WHEEL_STEP_PIN, LOW);
    delayMicroseconds(1200 / WHEEL_SPEED);
  }

