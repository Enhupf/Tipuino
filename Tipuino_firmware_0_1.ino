#include <TMCStepper.h>
#include <SoftwareSerial.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define NEOPIXEL_PIN                25
#define LCD_CLOCK                   52
#define LCD_MOSI                    51
#define LCD_RESET                   23
#define LCD_CS                      17
#define LCD_RS                      16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, NEOPIXEL_PIN, NEO_GRB);

U8G2_ST7567_JLX12864_1_4W_SW_SPI u8g2_lcd(U8G2_R2, LCD_CLOCK, LCD_MOSI, LCD_CS, LCD_RS, LCD_RESET);

  // --- Pin Definitions ---
  // Dispenser Axis
  #define DISPENSER_STEP_PIN         46
  #define DISPENSER_DIR_PIN          48
  #define DISPENSER_ENABLE_PIN       A8
  #define DISPENSER_LIMIT_SWITCH_PIN 21
  #define DISPENSER_END_SWITCH_PIN   39
  #define DISPENSER_BEAM_PIN         18
  #define DISPENSER_RX               65
  #define DISPENSER_TX               42

  // Box Axis
  #define BOX_STEP_PIN               26
  #define BOX_DIR_PIN                28
  #define BOX_ENABLE_PIN             24
  #define BOX_LIMIT_SWITCH_PIN       11
  #define BOX_ENCODER_PIN            19
  #define BOX_RX                     A12
  #define BOX_TX                     44

  // Screw Axis
  #define SCREW_STEP_PIN             A6
  #define SCREW_DIR_PIN              A7
  #define SCREW_ENABLE_PIN           A2
  #define SCREW_ENCODER_PIN          15
  #define SCREW_RX                   A10
  #define SCREW_TX                   A5

  // Wheel Axis
  #define WHEEL_STEP_PIN             A0
  #define WHEEL_DIR_PIN              A1
  #define WHEEL_ENABLE_PIN           38
  #define WHEEL_ENCODER_PIN          2
  #define WHEEL_BEAM_PIN             14
  #define WHEEL_RX                   A9
  #define WHEEL_TX                   40


  // motherboard fan
  // #define FAN_PIN                 9

  // --- Parameters ---
  // Dispenser
  #define DISPENSE_SPEED             6
  #define DISPENSE_PAUSE_MS          600
  #define MAX_DISPENSE_CYCLES        8
  #define DISPENSE_EXTRA_STEPS       10650
  #define DISPENSER_END_WIGGLE       2000
  #define DISPENSER_WIGGLE           400
  #define FINAL_HOMING_DELAY_MS      1000
  #define DISPENSER_CLEAR_STEPS	     300

  //Screw
  #define SCREW_EXTRA_STEPS          20
  #define SCREW_SPEED                3

  // Box
  #define BOX_SPEED                  5
  #define BOX_CLEAR_EXTRA_STEPS      50
  #define BOX_WIGGLE                 400

  // Wheel
  #define WHEEL_SPEED                4
  #define WHEEL_CLEAR_EXTRA_STEPS    200
  #define WHEEL_MOVE_PAUSE_MS        1000
  #define WHEEL_WIGGLE               50

  // Sequencing
  #define BOX_MOVES                  12
  #define DEBOUNCE_DELAY             50
  #define DISPENSER_BEAM_TIMEOUT_MS  1000
  #define WHEEL_BEAM_TIMEOUT_MS      500

  #define R_SENSE 0.11f

  SoftwareSerial DISPENSER_UART(DISPENSER_RX, DISPENSER_TX); 
  TMC2209Stepper dispenserDriver(&DISPENSER_UART, R_SENSE, 0b00);

  SoftwareSerial BOX_UART(BOX_RX, BOX_TX);
  TMC2209Stepper boxDriver(&BOX_UART, R_SENSE, 0b00);

  SoftwareSerial SCREW_UART(SCREW_RX, SCREW_TX);
  TMC2209Stepper screwDriver(&SCREW_UART, R_SENSE, 0b00);

  SoftwareSerial WHEEL_UART(WHEEL_RX, WHEEL_TX);
  TMC2209Stepper wheelDriver(&WHEEL_UART, R_SENSE, 0b00);

  // --- Globals ---
  bool dispenserHomed = false;
  int dispenseCount = 0;
  int cycle = 0;

  #define MICROSTEPS               16

  void setup() {
    strip.begin();
    strip.setBrightness(255);
    strip.setPixelColor(0, strip.Color(0, 0, 150));
    strip.setPixelColor(1, strip.Color(0, 0, 150));
    strip.setPixelColor(2, strip.Color(180, 180, 180));
    strip.show();

    u8g2_lcd.begin();
    delay(100);
    u8g2_lcd.setContrast(200);
    u8g2_lcd.clearBuffer();
    u8g2_lcd.firstPage();
    do {
        u8g2_lcd.setFont(u8g2_font_6x12_tr);
        u8g2_lcd.setFontMode(0);
        u8g2_lcd.drawStr(10, 10, "Tipuino starting...");
    } while ( u8g2_lcd.nextPage() );


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
    
    DISPENSER_UART.begin(57600);
    delay(20);
    BOX_UART.begin(57600);
    delay(20);
    SCREW_UART.begin(57600);
    delay(20);
    WHEEL_UART.begin(57600);
    delay(20);

    DISPENSER_UART.listen();
    delay(20);
    dispenserDriver.begin();     // init driver
    delay(20);
    dispenserDriver.toff(4);     // enable driver
    delay(20);
    dispenserDriver.rms_current(800); // motor RMS current in mA
    delay(20);
    dispenserDriver.microsteps(MICROSTEPS);   // microstepping
    delay(20);
    dispenserDriver.pwm_autoscale(true); // stealthChop
    delay(20);
    dispenserDriver.pdn_disable(true); // use PDN/UART pin
    delay(20);
    dispenserDriver.I_scale_analog(false); // use internal current reference
    delay(50);

    BOX_UART.listen();
    delay(20);
    boxDriver.begin();     // init driver
    delay(20);
    boxDriver.toff(4);     // enable driver
    delay(20);
    boxDriver.rms_current(800); // motor RMS current in mA
    delay(20);
    boxDriver.microsteps(MICROSTEPS);   // microstepping
    delay(20);
    boxDriver.pwm_autoscale(true); // stealthChop
    delay(20);
    boxDriver.pdn_disable(true); // use PDN/UART pin
    delay(20);
    boxDriver.I_scale_analog(false); // use internal current reference
    delay(50);

    SCREW_UART.listen();
    delay(50);
    screwDriver.begin();     // init driver
    delay(20);
    screwDriver.toff(4);     // enable driver
    delay(20);
    screwDriver.rms_current(800); // motor RMS current in mA
    delay(20);
    screwDriver.microsteps(MICROSTEPS);   // microstepping
    delay(20);
    screwDriver.pwm_autoscale(true); // stealthChop
    delay(20);
    screwDriver.pdn_disable(true); // use PDN/UART pin
    delay(20);
    screwDriver.I_scale_analog(false); // use internal current reference
    delay(50);

    WHEEL_UART.listen();
    delay(50);
    wheelDriver.begin();     // init driver
    delay(20);
    wheelDriver.toff(4);     // enable driver
    delay(20);
    wheelDriver.rms_current(800); // motor RMS current in mA
    delay(20);
    wheelDriver.microsteps(MICROSTEPS);   // microstepping
    delay(20);
    wheelDriver.pwm_autoscale(true); // stealthChop
    delay(20);
    wheelDriver.pdn_disable(true); // use PDN/UART pin
    delay(20);
    wheelDriver.I_scale_analog(false); // use internal current reference

    digitalWrite(SCREW_ENABLE_PIN, HIGH);
    digitalWrite(DISPENSER_ENABLE_PIN, HIGH);
    digitalWrite(BOX_ENABLE_PIN, HIGH);
    digitalWrite(WHEEL_ENABLE_PIN, HIGH);

    homeDispenserMotor();
    u8g2_lcd.firstPage();
      do {
        u8g2_lcd.setFont(u8g2_font_6x12_tr);
        u8g2_lcd.setFontMode(0);
        u8g2_lcd.drawStr(10, 10, "Tipuino starting...");
        u8g2_lcd.drawStr(10, 20, "homing dispenser");
        } while ( u8g2_lcd.nextPage() );
    dispenserHomed = true;
    u8g2_lcd.firstPage();
      do {
        u8g2_lcd.setFont(u8g2_font_6x12_tr);
        u8g2_lcd.setFontMode(0);
        u8g2_lcd.drawStr(10, 10, "Tipuino starting...");
        u8g2_lcd.drawStr(10, 20, "homing box");
        } while ( u8g2_lcd.nextPage() );
    homeBoxMotor();
    moveBoxToEncoderClear();
    digitalWrite(BOX_ENABLE_PIN, HIGH);
    dispenseCount = 0;
      u8g2_lcd.firstPage();
      do {
        u8g2_lcd.setFont(u8g2_font_6x12_tr);
        u8g2_lcd.setFontMode(0);
        u8g2_lcd.drawStr(10, 10, "Tipuino starting...");
        u8g2_lcd.drawStr(10, 20, "homing screw");
        } while ( u8g2_lcd.nextPage() );
      homeScrewMotor();
      digitalWrite(DISPENSER_ENABLE_PIN, LOW);
      digitalWrite(DISPENSER_DIR_PIN, LOW);
      for (int i = 0; i < DISPENSER_WIGGLE; i++) stepDispenser();
      delay(50);
      digitalWrite(DISPENSER_DIR_PIN, HIGH);
      for (int i = 0; i < DISPENSER_WIGGLE; i++) stepDispenser();
      digitalWrite(DISPENSER_DIR_PIN, LOW);
      digitalWrite(DISPENSER_ENABLE_PIN, HIGH);

      unsigned long startWait = millis();
      while (digitalRead(DISPENSER_BEAM_PIN) == HIGH && millis() - startWait < DISPENSER_BEAM_TIMEOUT_MS) {}
      if (digitalRead(DISPENSER_BEAM_PIN) == LOW) {
        delay(DISPENSE_PAUSE_MS);
        moveDispenserToNextClear();
        dispenseCount++;
      }
    strip.setPixelColor(0, strip.Color(150, 0, 0));
    strip.setPixelColor(1, strip.Color(150, 0, 0));
    strip.show();
  }

  void loop() {
    for (cycle = 0; cycle < BOX_MOVES; cycle++) {
      showStatus();
      if (dispenseCount > 0 and dispenseCount < 2){
      }
      else{
        dispenseCount = 0;
        }
      showStatus();
      while (dispenseCount < MAX_DISPENSE_CYCLES) {
        moveWheelToNextClearPosition();
        if (digitalRead(WHEEL_BEAM_PIN) == LOW) {
          digitalWrite(WHEEL_ENABLE_PIN, HIGH);
          digitalWrite(SCREW_ENABLE_PIN, LOW);
          homeScrewMotor();

          delay(200);
          digitalWrite(DISPENSER_ENABLE_PIN, LOW);
          digitalWrite(DISPENSER_DIR_PIN, LOW);
          for (int i = 0; i < DISPENSER_WIGGLE; i++) stepDispenser();
          delay(50);
          digitalWrite(DISPENSER_DIR_PIN, HIGH);
          for (int i = 0; i < DISPENSER_WIGGLE; i++) stepDispenser();
          digitalWrite(DISPENSER_DIR_PIN, LOW);
          digitalWrite(DISPENSER_ENABLE_PIN, HIGH);

          unsigned long startWait = millis();
          while (digitalRead(DISPENSER_BEAM_PIN) == HIGH && millis() - startWait < DISPENSER_BEAM_TIMEOUT_MS) {}
          if (digitalRead(DISPENSER_BEAM_PIN) == LOW) {
            delay(DISPENSE_PAUSE_MS);
            moveDispenserToNextClear();
            dispenseCount++;
            showStatus();
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
      digitalWrite(BOX_DIR_PIN, LOW);
      for (int i = 0; i < BOX_WIGGLE; i++) stepBoxMotor();
      digitalWrite(BOX_ENABLE_PIN, HIGH);
      digitalWrite(BOX_DIR_PIN, HIGH);

      digitalWrite(DISPENSER_DIR_PIN, HIGH);
      for (int i = 0; i < DISPENSER_END_WIGGLE; i++) stepDispenser();
      delay(50);
      digitalWrite(DISPENSER_DIR_PIN, LOW);
      for (int i = 0; i < DISPENSER_END_WIGGLE; i++) stepDispenser();
      delay(FINAL_HOMING_DELAY_MS);

      digitalWrite(BOX_ENABLE_PIN, LOW);
      for (int i = 0; i < BOX_WIGGLE; i++) stepBoxMotor();
      delay(50);
      digitalWrite(BOX_DIR_PIN, LOW);
      for (int i = 0; i < BOX_WIGGLE; i++) stepBoxMotor();
      digitalWrite(BOX_ENABLE_PIN, HIGH);
      digitalWrite(BOX_DIR_PIN, HIGH);

      digitalWrite(DISPENSER_DIR_PIN, HIGH);
      for (int i = 0; i < DISPENSER_END_WIGGLE; i++) stepDispenser();
      delay(50);
      digitalWrite(DISPENSER_DIR_PIN, LOW);
      for (int i = 0; i < DISPENSER_END_WIGGLE; i++) stepDispenser();
      delay(FINAL_HOMING_DELAY_MS);
      homeDispenserMotor();
      digitalWrite(BOX_ENABLE_PIN, LOW);
      moveBoxToEncoderClear();
      digitalWrite(BOX_ENABLE_PIN, HIGH);
      digitalWrite(WHEEL_ENABLE_PIN, LOW);
      delay(500);
    }

    u8g2_lcd.firstPage();
      do {
        u8g2_lcd.setFont(u8g2_font_6x12_tr);
        u8g2_lcd.setFontMode(0);
        u8g2_lcd.drawStr(10, 10, "Box full.");
        u8g2_lcd.drawStr(10, 20, "Happy pipetting!");
        } while ( u8g2_lcd.nextPage() );

    while (true) {}
  }

  void homeDispenserMotor() {
    digitalWrite(DISPENSER_ENABLE_PIN, LOW);
    digitalWrite(DISPENSER_DIR_PIN, HIGH);
    while (digitalRead(DISPENSER_LIMIT_SWITCH_PIN) == HIGH) stepDispenser();
    digitalWrite(DISPENSER_DIR_PIN, LOW);
    while (digitalRead(DISPENSER_BEAM_PIN) == LOW) stepDispenser();
    delay(DISPENSE_PAUSE_MS);
    for (int i = 0; i < DISPENSER_CLEAR_STEPS; i++) stepDispenser();
    digitalWrite(DISPENSER_ENABLE_PIN, HIGH);
  }

  void homeBoxMotor() {
    digitalWrite(BOX_ENABLE_PIN, LOW);
    digitalWrite(BOX_DIR_PIN, LOW);
    while (digitalRead(BOX_LIMIT_SWITCH_PIN) == HIGH) stepBoxMotor();
    digitalWrite(BOX_DIR_PIN, HIGH);
    while (digitalRead(BOX_ENCODER_PIN) == LOW) stepBoxMotor();
    for (int i = 0; i < BOX_CLEAR_EXTRA_STEPS; i++) stepBoxMotor();
    digitalWrite(BOX_ENABLE_PIN, HIGH);
  }

  void homeScrewMotor() {
    digitalWrite(SCREW_ENABLE_PIN, LOW);
    digitalWrite(SCREW_DIR_PIN, HIGH);
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
    digitalWrite(WHEEL_DIR_PIN, HIGH);
    for (int i = 0; i < WHEEL_WIGGLE; i++) stepWheelMotor();
    digitalWrite(WHEEL_DIR_PIN, LOW);

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
    delayMicroseconds(1200 / DISPENSE_SPEED);
  }

  void stepBoxMotor() {
    digitalWrite(BOX_STEP_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(BOX_STEP_PIN, LOW);
    delayMicroseconds(1200 / BOX_SPEED);
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
    delayMicroseconds(1000 / WHEEL_SPEED);
  }

 void showStatus() {
    u8g2_lcd.firstPage();
    do {
      u8g2_lcd.setFont(u8g2_font_6x12_tr);
      u8g2_lcd.setCursor(0, 12);
      u8g2_lcd.print("Filling box...");
      u8g2_lcd.setCursor(0, 28);
      u8g2_lcd.print("rows: ");
      u8g2_lcd.print(cycle);
      u8g2_lcd.setCursor(0, 44);
      u8g2_lcd.print("tips: ");
      u8g2_lcd.print(dispenseCount);
    } while (u8g2_lcd.nextPage());

 }


