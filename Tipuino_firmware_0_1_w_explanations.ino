/*
The sequence of events/movements for the Tipuino is as flollows:
- Tips in the hopper (chamber)
- Wheel turns and collects one tip (also called impeller scoop)
- The tip is thrown from the wheel into the steep rail
- The tip arrives in the horizontal rail. Here, it triggers a break beam (wheel beam)
- The wheel keeps turning until a tip has triggered the wheel beam
- The screw turns one revolution and pushes all tips in the rail on position forward. The if there is a tip in the final position, it is dropped into the dispenser
- If there is no tip, the wheel starts turning again
- tip inside the dispenser triggers the dispenser beam
- the dispenser moves one position forward
- if there are less than 8 tips in the dispenser, the wheel starts turning again
- when the dispenser has all 8 positions filled, it moves to dispense the tips into the box and returns to home
- the box moves one position further
- this cycle repeats 12 times (until the box is full)
- the Tipuino stops and rests until turned off (and on again) or being reset (via the reset button)

Additional movements:
"extra" or "clear" steps:
- The detection of movements largely depends on "encoders", i.e. small break beam sensors that have a notched disk (for rotary) or rail (for linear) movements to detect specific positions. The initial code had the parts move until the beam is either clear or broken, but being right on the edge makes them sensitive to small movements like vibrations and can lead to false triggers. Additionally, having this value gives the possiblity to calibrate movements through software instead of having to perfectly align the sensors physically. The respective axis moves through (wheel, screw and box) or to the hole (dispenser, since it needs to detect the tip entering the dispenser as well) in the encoder disk or rail (or the dispenser itself in this particular case) and then moves a few extra steps to get clear of the edge and/or to position the axis perfectly.

Wiggles:
- Wheel wiggle: in some cases, the tip being collected by the pin on the wheel is not completely settled. The wheel wiggle is a short backwards movement that dislodges these tips, avoiding a weird orientation when dropped into the rail. With the recently updated rail entry geometry this is may be unnecessary (to be tested).
- Dispenser wiggle: the tips falling from the horizontal rail into the dispenser have some freedom of movement in their drop path and as is the nature of 3D-printed parts, the sufraces are not super smooth. This can lead to tips getting stuck before reaching the dispenser. To shake these loose, the dispenser makes a small movement back and forth. This has proven to be very useful and I think we will keep it long term.
- Dispenser end wiggle and box wiggle: the transfer from the box into the dispenser similarly has a certain degree of freedom, particularly because there needs to be some clearance for the "head" of the tip and for the movement of the box underneath the dispenser. Thus, the tips can land beside the hole. To push these tips into the respective holes, the dispenser and box move a bit back and forth. These wiggles proved to be essential. It has been useful to have two box wiggles. The second dispenser end wiggle is for safety and can maybe be removed since it has never been obeserved to be needed.
*/

#include <TMCStepper.h>		// motor control library specific to the used TMC drivers
#include <SoftwareSerial.h>	// Library needed for serial communication with the motor drivers
#include <U8g2lib.h>		// Library for the communication with the LCD screen, which also contains fonts etc. to write text and make icons or similar
#include <Adafruit_NeoPixel.h>	// Library for the control of the status LEDs
#ifdef __AVR__
#include <avr/power.h>		// not sure why my AI friend included this
#endif

#define NEOPIXEL_PIN                25		// pin to control all 3 LEDs of the screen module
#define LCD_CLOCK                   52		// this and the following pins are for LCD communication, their function is handled automatically by the library
#define LCD_MOSI                    51
#define LCD_RESET                   23
#define LCD_CS                      17
#define LCD_RS                      16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, NEOPIXEL_PIN, NEO_GRB);		// LEDs count as a "strip" and are controlled via the same pin. LED2 is the LCD nack light

U8G2_ST7567_JLX12864_1_4W_SW_SPI u8g2_lcd(U8G2_R2, LCD_CLOCK, LCD_MOSI, LCD_CS, LCD_RS, LCD_RESET); // U8G2 SPI constructor for LCD communication

  // --- Pin Definitions ---
/*
motor pins are:
enable -> LOW: motor active; HIGH: motor inactive
dir -> determines direction of motor. Has to be tested for appliaction since depending on set-up (motor mounting, use of gears, etc.), direction can be reversed
step -> receives the pulses for motor movement. HIGH lets the motor make one step, LOW needed to separate step signals.
RX/TX -> pins for UART communication
*/
  // Dispenser Axis
  #define DISPENSER_STEP_PIN         46
  #define DISPENSER_DIR_PIN          48
  #define DISPENSER_ENABLE_PIN       A8
  #define DISPENSER_LIMIT_SWITCH_PIN 21		// Dispenser limit switch for homing
  #define DISPENSER_END_SWITCH_PIN   39		// Dispenser limit switch for the "dispense" position in which tips are transferred to the box
  #define DISPENSER_BEAM_PIN         18		// break beam detecting a tip dropping from the rail into the dispenser
  #define DISPENSER_RX               65
  #define DISPENSER_TX               42

  // Box Axis
  #define BOX_STEP_PIN               26
  #define BOX_DIR_PIN                28
  #define BOX_ENABLE_PIN             24
  #define BOX_LIMIT_SWITCH_PIN       11		// Limit switch for box homing
  #define BOX_ENCODER_PIN            19		// pin for the box encoder sensor
  #define BOX_RX                     A12
  #define BOX_TX                     44

  // Screw Axis
  #define SCREW_STEP_PIN             A6
  #define SCREW_DIR_PIN              A7
  #define SCREW_ENABLE_PIN           A2
  #define SCREW_ENCODER_PIN          15		// pin for the screw encoder sensor
  #define SCREW_RX                   A10
  #define SCREW_TX                   A5

  // Wheel Axis
  #define WHEEL_STEP_PIN             A0
  #define WHEEL_DIR_PIN              A1
  #define WHEEL_ENABLE_PIN           38
  #define WHEEL_ENCODER_PIN          2		// pin for the wheel encoder sensor
  #define WHEEL_BEAM_PIN             14		// pin for the wheel break beam sensor
  #define WHEEL_RX                   A9
  #define WHEEL_TX                   40


/*
We don't run the motor under great load and also only run 1 at a time for now. Thus, the motor drivers should not run very hot. However, they are kept in an enclosure now, so if the device runs for long, cooloing of the motor drivers (which handle all the power routed to the motors) might need active cooling. There is an option to run a fan (already accounted for in enclosure design) via a PWM pin the should allow to control the fan speed. This is a normal function of the 3D printer mainboards, so already set up well in the electronics.

  // motherboard fan
  #define FAN_PIN                 9
*/
  // --- Parameters ---
/* The speeds of the motors are a bit weird and tricky to set. Right now, the speed is mainly determined by the time that the step pin of the respective motor stays LOW in the stepping function. The AI implementation of having some control over the speed is to set a minimum speed by having a maximum pause (1200 µs right now) and deviding this number by the speed variable. This of course limits the speed settings to numbers which give an integer when you devide the maximum pause time by this number.
*/
  // Dispenser
  #define DISPENSE_SPEED             6
  #define DISPENSE_PAUSE_MS          600
  #define MAX_DISPENSE_CYCLES        8		// variable determining that there are 8 dispenser positions
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
  #define BOX_MOVES                  12		// Variable determining that there are 12 rows in the box
  #define DEBOUNCE_DELAY             50
  #define DISPENSER_BEAM_TIMEOUT_MS  1000	// maximum time the dispenser waits until it's decided there is no tip in the dispenser
  #define WHEEL_BEAM_TIMEOUT_MS      500	// maximum time the wheel/screw wait until it's decided that there is no tip in the first position in the horizontal rail


// Serial communication elements for motor driver UART communication

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
// initialization of status LEDs 
   strip.begin();
    strip.setBrightness(255);
    strip.setPixelColor(0, strip.Color(0, 0, 150));		// blue for status LEDs
    strip.setPixelColor(1, strip.Color(0, 0, 150));
    strip.setPixelColor(2, strip.Color(180, 180, 180));		// white for screen backlight
    strip.show();

// initialization of SPI communication with the LCD screen and initial text
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

// set up of how the pins are handled

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

// set-up of all the motor driver UART communications and setting of the important values. Afterwards, there is currently no further communication. The pauses are there to give the shady communication to have some time to work properly. 
    
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

// start with all motors disabled

    digitalWrite(SCREW_ENABLE_PIN, HIGH);
    digitalWrite(DISPENSER_ENABLE_PIN, HIGH);
    digitalWrite(BOX_ENABLE_PIN, HIGH);
    digitalWrite(WHEEL_ENABLE_PIN, HIGH);

// homing sequences of all motors

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

// here we are checking if during the homing of the screw a tip was dropped into the dispenser and moving it if it did

      unsigned long startWait = millis();
      while (digitalRead(DISPENSER_BEAM_PIN) == HIGH && millis() - startWait < DISPENSER_BEAM_TIMEOUT_MS) {}
      if (digitalRead(DISPENSER_BEAM_PIN) == LOW) {
        delay(DISPENSE_PAUSE_MS);
        moveDispenserToNextClear();
        dispenseCount++;
      }
// turning status LEDs green after successful homing
    strip.setPixelColor(0, strip.Color(150, 0, 0));
    strip.setPixelColor(1, strip.Color(150, 0, 0));
    strip.show();
  }

  void loop() {
    for (cycle = 0; cycle < BOX_MOVES; cycle++) {		// the over-achring cycle is the amount of rows to be filled in the box.
      showStatus();
      if (dispenseCount > 0 and dispenseCount < 2){
      }
      else{
        dispenseCount = 0;
        }
/* The if statement above only comes into effect if the initial homing of the screw pushed a tip into the dispenser. In that casem the dispenseCount is 1 and we leave it that way. If there was no tip pushed into the dispenser, dispenseCount is 0, so effectively stays this way. The only other realistic scenario is that the end of a cylce of filling the dispenser, so dispenseCount is 8 and is reset to 0.*/

      showStatus();

// next, we move the wheel again and again until we have a broken wheel beam. Then we move the screw once.

      while (dispenseCount < MAX_DISPENSE_CYCLES) {
        moveWheelToNextClearPosition();
        if (digitalRead(WHEEL_BEAM_PIN) == LOW) {
          digitalWrite(WHEEL_ENABLE_PIN, HIGH);
          digitalWrite(SCREW_ENABLE_PIN, LOW);
          homeScrewMotor();
          delay(200);

// After screw movement, we wiggle the dispenser and see if we put a tip into it. If yes, we move it, if no, we continue with the wheel movements.

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
// here we need to double check if this isn't a vestige of some programming nonsense. Looks like it's not needed to me right now.
        if (digitalRead(DISPENSER_BEAM_PIN) == LOW) {
          delay(DISPENSE_PAUSE_MS);
          moveDispenserToNextClear();
          dispenseCount++;
          digitalWrite(WHEEL_ENABLE_PIN, LOW);
          delay(10);
        }
      }

/* once the dispenser is full (dispenseCount = 8) we move it until it hits the limit switch on the box side. We do our wiggling of dispenser and box and return the dispenser to home. This essentially has no checks at all, so we are relying on the process being 100 % reliable. Thus, the wiggles are essential to be performed every time. I have not come up with a way of checking success of this process with sensors, since this would need something that checks that all 8 tips are actually in the box.*/

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

/* These cycles are repeated until we meet clyce = 12. Then the Tipuino basically becomes a potato until it is reset.

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
    while (digitalRead(BOX_ENCODER_PIN) == HIGH) stepBoxMotor();
    delay(DEBOUNCE_DELAY);
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
