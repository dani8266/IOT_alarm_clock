#include "Particle.h"
#include "Adafruit_TSL2561_U.h"


void configureSensor();

bool setupLuxSensor();

int getLuxValue();

bool checkLuxValue(int threshold);
