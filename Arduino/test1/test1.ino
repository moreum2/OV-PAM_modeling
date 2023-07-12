#include "HX711.h"
#include <TimeLib.h>

#define DOUT  6 // 데이터 핀 6번핀
#define CLK   7 // 클럭 핀 7번핀

HX711 scale(DOUT, CLK);

float calibration_factor = -207;    // 로드셀 종류나 상황에 따라 적당한 값으로 시작 kg:-248000, g:-214
unsigned long startTime = 0;   // 시작 시간 저장 변수

void setup() {
  Serial.begin(115200);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.set_scale();
  scale.tare(); // Reset the scale to 0

  long zero_factor = scale.read_average(); // Get a baseline reading
  Serial.print("Zero factor: "); // This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);

  startTime = millis(); // 프로그램 시작 시간 저장
}

void loop() {
  long long int currentTime = millis(); // 현재 시간 저장
  long long int elapsedTime = currentTime - startTime; // 경과 시간 계산

  scale.set_scale(calibration_factor); // 보정 계수 적용

  Serial.print("Reading: ");
  Serial.print(scale.get_units()+0.203*elapsedTime / 1000.0, 1);
  Serial.print(" g"); // 단위 출력 (kg 또는 g)
  Serial.print(" Calibration factor: ");
  Serial.print(calibration_factor);
  Serial.print(" Elapsed time: ");
  Serial.print(elapsedTime / 1000.0); // 경과 시간을 초 단위로 출력
  Serial.println();

  if (Serial.available()) {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }
}
