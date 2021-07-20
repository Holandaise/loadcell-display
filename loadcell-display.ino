#include <HX711.h>

#define LOADCELL_DOUT 2
#define LOADCELL_SCK 3

HX711 scale; // global scale object
float calibration_factor = -45000; // this is gathered from calibration sketch

typedef union
{
  float decimal;
  byte bytes[4];
}FLOATUNION_T;

void display(int val) {
  Serial.write(0x76); // clear display
  char str[5] = {0};
  //str[4] = '\0';
  sprintf(str, "%4d", val);
  Serial.write(0x77); // Decimal control
  Serial.write(0b00000100); // Set 3rd digit decimal on
  Serial.print(str);
}

void setup() {
  // put your setup code here, to run once:
  scale.begin(LOADCELL_DOUT, LOADCELL_SCK);
  Serial.begin(9600);
  Serial.write(0x76); //clear display
  scale.set_scale(calibration_factor);
  delay(250);
  scale.tare();
}

int apply_mask(float reading){
  float tmp;
  FLOATUNION_T split;
  split.decimal = reading;
  split.bytes[0] = 0;
  tmp = split.decimal * 10;
  return (int)tmp;
}

void loop() {
  float reading = scale.get_units();
  int value = apply_mask(reading);
  display(value);
  delay(500); // hold display for visibility
}
