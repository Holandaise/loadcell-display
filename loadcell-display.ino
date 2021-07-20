#include <HX711.h>
#include <SPI.h>

#define LOADCELL_DOUT 2
#define LOADCELL_SCK 3

#define SSB 4 // SPI slave select

HX711 scale; // global scale object
float calibration_factor = -8000; // this is gathered from calibration sketch

typedef union
{
  float decimal;
  byte bytes[4];
}FLOATUNION_T;

void display(int val) {
  Serial.write(0x76); // clear display
  char str[5] = {0};
  sprintf(str, "%4d", val);
  Serial.write(0x77); // Decimal control
  Serial.write(0b00000100); // Set 3rd digit decimal on
  Serial.print(str);
}

void displaySPI(int val) {
  char str[4] = {0}; //initialize an array of bytes
  sprintf(str, "%4d", val); // format val into str
  digitalWrite(SSB, LOW); // activate slave device
  SPI.transfer(0x76); // clear display
  SPI.transfer(0x77); // decimal control
  SPI.transfer(0x04); // set 3rd decimal on

  // transmit each byte in str 1 at a time
  for (int i = 0; i < 4; i++) {
    SPI.transfer(str[i]);
  }
  digitalWrite(SSB, HIGH); // deactivate slave device
}

void setup() {
  // put your setup code here, to run once:
  scale.begin(LOADCELL_DOUT, LOADCELL_SCK);
  // Serial.begin(9600);
  pinMode(SSB, OUTPUT);
  digitalWrite(SSB, HIGH); // make sure SS is high
  // Serial.write(0x76); //clear display
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV64); // slow down SPI transfers

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
  // display(value);
  displaySPI(value);
  delay(500); // hold display for visibility
}
