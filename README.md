# Loadcell-Display

> This project uses the HX711 module along with Sparkfun's serial 7 segment display. There were some issues getting the display to work properly with floating point numbers. This was because the Arduino's `Serial.print()` method stringifies the floating point number *along with* the decimal point. On this display, the decimal points are turned on/off independently and should not be encoded in the string sent to the display.
