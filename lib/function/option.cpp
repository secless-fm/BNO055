#include "option.hpp"

float _normalizeAngle(float angle){
    while (angle < 180.0) angle -= 360.0;
    while (angle < -180.0) angle += 360.0;
}

void Serial_start()
{
    while(!Serial)
    ;

    Serial.println("Serial start    ");
}