/*
  Project: Transistortester-Blue
  Board:   Bluepill

  Based on the following Repositorys:
    https://github.com/Mikrocontroller-net/transistortester
    

  Hardware:
    Bluepill - STM32F103C8T6 (64k Flash, 20k RAM) 

  1. Install Board 
  Files > Preferences > Aditional Boards > 
    https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
  2. Select Board
    Tool > Board > Generic STM32f1 Series
    Tool > Port > dev/ttyACM0
      - Variant: STM32F103C8  (64k Flash, 20k RAM)
      - Upload method: OpenOCD STLink (SWD)
      - //Upload method: STM32duino bootloader (or Serial if you don’t have the bootloader installed)
  3. Jumper Settings (Boot Mode - Flash)
    BOOT0 |	BOOT1	| Boot Mode
    ------+-------+----------------------------------------
    0	    | x	    | Flash (normal)
    1	    | 0     |	System memory (for flashing via serial)
  
  Librarys:
  - LiquidCrystal I2C Multilingual by Loc P. Le 
      https://github.com/locple/LCDI2C_Multilingual
      MIT License 
      v2.0.2 (May 2, 2024)
  - STM32encoder by bit.factory
      https://github.com/gianni-carbone/STM32encoder
      GPL-3.0 license
      v0.9.7 (Mar 10, 2022)


  Pinout - Bluepill
  Onboard LED:          PC13
  Button:               PA2
  Encoder A:            PA0
  Encoder B:            PA1
  SDA:                  PB7
  SCL:                  PB6
*/
// Encoder
#include <STM32encoder.h>
// Use TIM2 with PA0 and PA1
STM32encoder encoder(TIM2);

// LCD
#include <LCDI2C_Multilingual.h>
LCDI2C_Generic lcd(0x27, 20, 4); // 20x4 display (often 0x27 or 0x3F)

// Button
const int buttonPin = PA2;     // Button connected to A0
bool lastButtonState = HIGH;

// LED
const int ledPin = PC13;       // Onboard LED
bool ledState = false;

void setup() {
  setupLCD();
  pinMode(buttonPin, INPUT_PULLUP); // Button pulls low when pressed
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);       // LED off (inverted logic on PC13)

  // Serial
  Serial.begin(9600);
  while (!Serial); // Wait for USB Serial to be ready
  Serial.println("Hello from Blue Pill!");

}

void loop() {
  bool buttonState = digitalRead(buttonPin);
  // Detect falling edge (button press)
  if (lastButtonState == HIGH && buttonState == LOW) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState ? LOW : HIGH); // LOW = LED on
    lcd.setCursor(15, 1);
    lcd.print(ledState);
    printEncoderTest();
    delay(200); // Debounce delay
  }
  lastButtonState = buttonState;

  if (encoder.isUpdated()) {
    printEncoderTest();
    lcd.setCursor(9, 2);
    lcd.printf("%7d",encoder.pos());
  } 

}

void setupLCD(){
  lcd.init();          // Initialize the LCD
  lcd.backlight();      // Turn on backlight
  lcd.setCursor(0, 0);  // First column, first row
  lcd.print("TransistorTester");
  lcd.setCursor(0, 1);  // First column, second row
  lcd.print("Button: ");
  lcd.setCursor(15, 1);
  lcd.print(ledState);
  lcd.setCursor(0, 2);  // First column, second row
  lcd.print("Encoder: ");
  lcd.setCursor(9, 2);
  lcd.printf("%7d",encoder.pos());
}

void printEncoderTest(){
  Serial.printf("%ld\t%d\n", encoder.pos(), ledState);
}