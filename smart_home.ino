#include <Arduino_FreeRTOS.h>
#define SPEAKERS_PIN 3
#define POT_PIN A0
#define ALARM_LED 2

void TaskMain( void *pvParameters );
bool isAlarmingTemperature(int);
void shutdownAlarm();
void raiseAlarm();
int readTemperature();

void setup() {
  Serial.begin(9600);

  pinMode(SPEAKERS_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(POT_PIN, INPUT);

  xTaskCreate(TaskMain, "Main", 128, NULL, 2, NULL);
}

void loop()
{
}

void TaskMain(void *pvParameters)
{
  (void) pvParameters;

  for (;;)
  {
    int temp = readTemperature();
    Serial.println(String(temp));

    if (isAlarmingTemperature(temp)) {
      raiseAlarm();
    } else {
      shutdownAlarm();
    }

    cycleDelay();
  }
}

bool isAlarmingTemperature(int temp) {
  return temp < 10 || temp > 30;
}

void shutdownAlarm() {
  analogWrite(SPEAKERS_PIN, 0);
  digitalWrite(ALARM_LED, LOW);
}

void raiseAlarm() {
  analogWrite(SPEAKERS_PIN, 30);
  digitalWrite(ALARM_LED, HIGH);
}

void cycleDelay() {
  vTaskDelay( 400 / portTICK_PERIOD_MS );
}

int readTemperature() {
  int temp = analogRead(POT_PIN);
  return map(temp, 0, 1024, 0, 50);
}
