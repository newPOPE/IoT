#include <TwiLiquidCrystal.h>

TwiLiquidCrystal lcd(0x27);

namespace Display {

    void intro () {
        lcd.begin(16, 2);
        lcd.backlight();
        // Print a message to the LCD.
        lcd.print("Temp:");
    }

    void printTemp (sensorState_t &state) {
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(8, 0);
        // print the number of seconds since reset:
        lcd.print(state.temperature, 1);
        lcd.setCursor(12, 0);
    }

}