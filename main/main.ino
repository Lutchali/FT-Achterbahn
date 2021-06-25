#include <Ftduino.h>
#include "engine.h"

#define light_station_begin Ftduino::I1
#define station_motor_1_button Ftduino::I5
#define station_motor_2_button Ftduino::I6
#define elevator_motor_1_button Ftduino::I7

Engine station_motor_1(Ftduino::M1, Ftduino::C1);
Engine station_motor_2(Ftduino::M2, Ftduino::C2);
Engine elevator_motor_1(Ftduino::M3, Ftduino::C3);
Engine buffer_motor_1(Ftduino::M4, Ftduino::C4);

/*Pins:
 * I1:LightsensorStationBegin
 * I2:
 * I3:
 * I4:
 * I5: Station Motor 1 button
 * I6: Station Motor 2 button
 * I7: Elevator Motor button
 * I8:
 * M1: Staion Motor 1
 * M2: Station Motor 2
 * M3: Elevator
 * M4: Buffer Motor
 * M5:
 * M6:
 * M7:
 * M8:
 * C1:
 * C2:
 * C3:
 * C4:
*/

void setup() {
  ftduino.init();
  Serial.begin(9600);
  ftduino.input_set_mode(light_station_begin, Ftduino::RESISTANCE);
}

void loop() {
  if(ftduino.input_get(light_station_begin)>10000){
    station_motor_1.start();
  }
}
