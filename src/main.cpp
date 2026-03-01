#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);

static float val = 0;
const int gyro_pin = 0;

/*
static float normalizeangle(float angle)
{
  while (angle < -180.0) angle += 360.0;
  while (angle > 180.0) angle -= 360.0;
  return angle;
}
*/

const int Button_pin = 1;
bool BNO_start = false;

void setup()
{
  Serial.begin(115200);
  pinMode(Button_pin, INPUT_PULLDOWN);
  while (!Serial)
    ;
  Serial.print("BNO055 start   ");

  if (!bno.begin())
  {
    Serial.println("BNO not detected");
    BNO_start = false;
    while (1)
      ;
  }

  delay(1000);

  BNO_start = true;
  bno.setExtCrystalUse(true);
  Serial.print("bno ready");

  analogWriteResolution(10); // 2^10=1024
}

void loop()
{
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  float yaw   = euler.x();
  bool nowButton = digitalRead(Button_pin);
  static bool prevButton = LOW;

  if(prevButton == LOW && nowButton == HIGH)
  {
    Serial.println(" Reset ");
    val = yaw; // 現在の角度(Yaw)を保存
  }
  Serial.print(" ");
  prevButton = nowButton;
  
  /* リセット時の角度を基準としたbnoの角度=(bno本体の角度-リセット時の角度+360)を360で割った余り */
  yaw = fmod((yaw - val + 360.0),360.0);

  int robotAngle = (yaw / 360.0) * 1024; // 角度：0 ~ 360 -> 0 ~ 1023 -> 0v ~ 3.3v


  /*
  float pitch = euler.y();
  float roll  = euler.z();
  */

  Serial.print(" reset val : ");
  Serial.print(val);

  Serial.print(" Yaw : ");
  Serial.print((yaw));

  /*
  Serial.print(" Pitch : ");
  Serial.print((pitch));

  Serial.print(" Roll : ");
  Serial.print((roll));
  */

  Serial.print(" manman ");
  Serial.println(robotAngle);

  if (BNO_start){
      analogWrite(gyro_pin, robotAngle); // gyro_pinに値を送る。
  }else{
      Serial.print("BNOが起動してないのかもです!（妹）");
  }

  delay(10);
}