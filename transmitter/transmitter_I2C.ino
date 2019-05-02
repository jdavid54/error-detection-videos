#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
// Create new LCD03 instance
LiquidCrystal_I2C lcd(0x27,20,4);

// Transmit rate in bps
#define TX_RATE 5

// Pin assignments
#define TX_CLOCK 2
#define TX_DATA 3

const char *message = "Hello, world!";

void setup() {
  pinMode(TX_CLOCK, OUTPUT);
  pinMode(TX_DATA, OUTPUT);

  // Initialize the LCD screen
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print(message);

  for (int byte_idx = 0; byte_idx < strlen(message); byte_idx++) {
    char tx_byte = message[byte_idx];

    // Clear the second line of the display
    lcd.noCursor();
    lcd.setCursor(0, 1);
    lcd.print("        ");
    lcd.setCursor(byte_idx, 0);
    lcd.cursor();

    for (int bit_idx = 0; bit_idx < 8; bit_idx++) {
      bool tx_bit = tx_byte & (0x80 >> bit_idx);

      digitalWrite(TX_DATA, tx_bit);
      delay((1000 / TX_RATE) / 2);

      // Update the LCD
      lcd.noCursor();
      lcd.setCursor(bit_idx, 1);
      lcd.print(tx_bit ? "1" : "0");
      lcd.setCursor(byte_idx, 0);
      lcd.cursor();

      // Pulse clock
      digitalWrite(TX_CLOCK, HIGH);
      delay((1000 / TX_RATE) / 2);
      digitalWrite(TX_CLOCK, LOW);      
    }
  }
  digitalWrite(TX_DATA, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

}
