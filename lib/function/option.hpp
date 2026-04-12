#pragma once

#include <Arduino.h>

float NormalizeAngle(float angle); // 角度を 0°~360° → -180°~180° に変換

void Serial_start(); // シリアル起動確認

class MovingAverage
{
private:
    static const int num_elements = 10;      // サイズ定数(回数)
    float storage[num_elements] = {0}; // バッファを0で初期化
    int current_target = 0;
    int count = 0;

public:
    float update(float now_val);
};