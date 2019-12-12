#include "lm35.h"

int IndoorTemp(){
  int adc_value = analogRead(lm35_pin);
  int temp_val = adc_value*0.80;
  temp_val = temp_val/10;
  return temp_val;
}
