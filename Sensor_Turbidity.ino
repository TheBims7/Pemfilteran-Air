#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int Servo1 = 3;
const int Turbidity = A0;

int turbidity;
bool status = false;
int turbidityValue = 0;

TaskHandle_t taskTurbidityHandle;
TaskHandle_t taskServoHandle;
TaskHandle_t taskLCDHandle;

void loop() {
 
}

void taskTurbidity(void *pvParameter){
  while(1){ 
    turbidityValue = analogRead(A0);
    turbidity = map(turbidityValue,  0, 520, 300, 0);
    if (turbidity > 150){
      status = true;
    }else{
      status = false;
    }
    vTaskDelay(100); 
  }
}

void taskServo(void *pvParameter){
  Servo servo1;
  servo1.attach(Servo1);
  while(1){
    if (status){
      servo1.write(90);
    }else{
      servo1.write(0);
    }
    vTaskDelay(100); 
  }
}

void taskLCD(void *pvParameter){
  Wire.begin();
  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
  lcd.init();
  lcd.backlight();
  while(1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(turbidity);
    lcd.print(" NTU");
    lcd.setCursor(0,1);
    lcd.print(status ? "Kotor" : "Bersih");
    vTaskDelay(100);
  }
}

void setup() {
  xTaskCreate(taskTurbidity, "Turbidity", 128, NULL, 1, &taskTurbidityHandle);
  xTaskCreate(taskServo, "Servo", 128, NULL, 2, &taskServoHandle);
  xTaskCreate(taskLCD, "LCD", 128, NULL, 3, &taskLCDHandle);
}
