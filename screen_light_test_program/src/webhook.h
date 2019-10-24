#include "Particle.h"

#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)

void myHandler(const char *event, const char *data);

void setupWebhook();

void loopWebhook();

int tempReturn();

String sunriseReturn();

String timeReturn();
