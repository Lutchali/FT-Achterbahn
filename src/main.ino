#include <Ftduino.h>
#include "engine.h"

#define light_station_begin Ftduino::I1
#define light_station_end Ftduino::I2
#define station_motor_1_button Ftduino::I4
#define station_motor_2_button Ftduino::I5
#define elevator_motor_1_button_top Ftduino::I6
#define elevator_motor_1_button_bottom Ftduino::I7
#define buffer_motor_1_button Ftduino::I8

#define station_motor_1_counter Ftduino::C1
#define station_motor_2_counter Ftduino::C2
#define elevator_motor_1_counter Ftduino::C3


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
    motor1();
    elevator1();
}

void motor1() {
    if(ftduino.input_get(light_station_begin)>10000){
        station_motor_1.start();
    }
    if(station_motor_1.get_steps() > 100){
        station_motor_1.change_direction();
        station_motor_1.reset_steps();
        motor2();
    }
    if(ftduino.input_get(station_motor_1_button) == 1){
        station_motor_1.stop();
    }
}

void motor2() {
    station_motor_2.start();
    if(ftduino.input_get(station_motor_2_button) == 1){
        station_motor_2.change_direction();
    }
    if(station_motor_2.get_steps() > 100){
        station_motor_2.stop();
        station_motor_2.reset_steps();
    }
}

void elevator1() {
    if(ftduino.input_get(light_station_end)>10000){
        elevator_motor_1.start();
    }
    if(ftduino.input_get(elevator_motor_1_button_top) == 1){
        elevator_motor_1.change_direction();
    }
    if(ftduino.input_get(elevator_motor_1_button_bottom) == 1){
        station_motor_1.stop();
    }
}

void buffer() {
    buffer_motor_1.start();
    if(buffer_motor_1.get_steps() > 100){
        buffer_motor_1.change_direction();
        buffer_motor_1.reset_steps();
    }
    if(ftduino.input_get(buffer_motor_1_button) == 1){
        buffer_motor_1.stop();
    }
}
