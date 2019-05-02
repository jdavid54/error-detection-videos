#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// Create new LCD03 instance
LiquidCrystal_I2C lcd(0x27,20,4);

// Pin assignments
#define RX_CLOCK 2
#define RX_DATA 3

char message[16];
volatile byte rx_byte = 0;
volatile int bit_position = 0;
volatile bool update_lcd = true;

void setup() {
  pinMode(RX_DATA, INPUT);
  strcpy(message, "");
  // Initialise a 20x4 LCD
  lcd.init();
 
  // Turn on the backlight
  lcd.backlight();
  attachInterrupt(digitalPinToInterrupt(RX_CLOCK), onClockPulse, RISING);
}

void onClockPulse() {
  bool rx_bit = digitalRead(RX_DATA);

  if (bit_position == 8) {
    rx_byte = 0;
    bit_position = 0;
  }
  
  if (rx_bit) {
    rx_byte |= (0x80 >> bit_position);
  }

  bit_position += 1;

  if (bit_position == 8) {
    strncat(message, (const char *)&rx_byte, 1);
  }
  
  update_lcd = true;
}

void loop() {
  if (update_lcd) {
    update_lcd = false;
    
    lcd.noCursor();
    lcd.setCursor(0, 0);
    lcd.print(message);
    lcd.setCursor(0, 1);
    for (int i = 0; i < 8; i += 1) {
      if (i < bit_position) {
        lcd.print(rx_byte & (0x80 >> i) ? "1" : "0");
      } else {
        lcd.print(" ");
      }
    }
    lcd.setCursor(strlen(message), 0);
    lcd.cursor();
  }
}
