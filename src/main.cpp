#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);

static double val = 0;


static double normalizeangle(double angle)
{
  while (angle < -180.0) angle += 360.0;
  while (angle > 180.0) angle -= 360.0;
  return angle;
}

const int Button_pin = 1;

void setup()
{
  Serial.begin(9600);
  pinMode(Button_pin, INPUT_PULLUP);
  while (!Serial)
    ;
  Serial.print("BNO055 start   ");

  if (!bno.begin())
  {
    Serial.println("BNO not detected");
    while (1)
      ;
  }

  delay(1000);

  // bno.setExtCrystalUse(true);
  Serial.print("bno ready");
}

bool prevButton = HIGH;

void loop()
{
  /*
  // キャリブレーション
  uint8_t sys, gyro, accel, mag;
  bno.getCalibration(&sys, &gyro, &accel, &mag);


  Serial.print("CALIB : ");
  Serial.print("sys : ");
  Serial.print(sys);
  Serial.print(" gyro : ");
  Serial.print(gyro);
  Serial.print(" accel : ");
  Serial.print(accel);
  Serial.print(" mag : ");
  Serial.print(mag);

  if(sys < 3 || gyro < 3 || accel < 3 || mag < 3)
  {
    Serial.println(" Colabrationing...");
    return;
  }
  */

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  double yaw   = euler.x();
  bool nowButton = digitalRead(Button_pin);

  if(prevButton == HIGH && nowButton == LOW)
  {
    Serial.println(1);
    // Serial.println(" Reset ");
    // val = yaw; // 現在の角度(Yaw)を保存
  }
  if(prevButton == HIGH && nowButton == HIGH)
  {
    Serial.println(0);
  }
  

  prevButton = nowButton;
  
  /* リセット時の角度を基準としたbnoの角度=(bno本体の角度-リセット時の角度+360)を360で割った余り */
  /*
  yaw = fmod((yaw - val + 360.0),360.0);


  double pitch = euler.y();
  double roll  = euler.z();

  Serial.print(" reset val : ");
  Serial.print(val);

  Serial.print(" Yaw : ");
  Serial.print(normalizeangle(yaw));

  Serial.print(" Pitch : ");
  Serial.print(normalizeangle(pitch));

  Serial.print(" Roll : ");
  Serial.println(normalizeangle(roll));
  */
  delay(100);
}