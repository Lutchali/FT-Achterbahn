#include "engine.h"
#include <Arduino.h>
#include <Ftduino.h>


#define light_station_begin Ftduino::I1
#define light_station_end Ftduino::I2
#define station_motor_1_button Ftduino::I4
#define station_motor_2_button Ftduino::I5
#define elevator_motor_1_button_top Ftduino::I6
#define elevator_motor_1_button_bottom Ftduino::I7
#define reset_button Ftduino::I8

Engine station_motor_1(Ftduino::M1, Ftduino::C1, TriggerType::lightsensor,
                       light_station_begin, 10000, station_motor_1_button, TriggerType::steps, Ftduino::C1, 1400,
                       false);
Engine station_motor_2(Ftduino::M2, Ftduino::C2, TriggerType::steps,
                       Ftduino::C1, 1350, station_motor_2_button, TriggerType::steps, Ftduino::C2, 650,
                       true);
Engine elevator_motor_1(Ftduino::M3, Ftduino::C3, TriggerType::button,
                        station_motor_2_button, 0, elevator_motor_1_button_bottom, TriggerType::button, elevator_motor_1_button_top, 0, false);
// TODO: Implement buffer engine
// Engine buffer_motor_1(Ftduino::M4, Ftduino::C4);

bool global_reset = false;
unsigned int last_reset = 0;

/*Pins:
 * I1:LightsensorStationBegin
 * I2:
 * I3:
 * I4:Station Motor 1 button
 * I5:Station Motor 2 button
 * I6:
 * I7: Elevator Motor button
 * I8:
 * M1: Station Motor 1
 * M2: Station Motor 2
 * M3: Elevator
 * M4: Buffer Motor
 * M5:
 * M6:
 * M7:
 * M8:
 * C1: Counter Motor 1
 * C2:
 * C3:
 * C4:
 */

void setup() {
  ftduino.init();
  Serial.begin(9600);
  ftduino.input_set_mode(light_station_begin, Ftduino::RESISTANCE);
  ftduino.input_set_mode(station_motor_1_button, Ftduino::SWITCH);
  ftduino.input_set_mode(station_motor_2_button, Ftduino::SWITCH);
  ftduino.input_set_mode(elevator_motor_1_button_bottom, Ftduino::SWITCH);
  ftduino.input_set_mode(reset_button, Ftduino::SWITCH);
  ftduino.counter_set_mode(Ftduino::C1, Ftduino::C_EDGE_ANY);
  ftduino.counter_set_mode(Ftduino::C2, Ftduino::C_EDGE_ANY);
  // TODO: Set pin modes for buttons
  station_motor_1.reset();
  station_motor_2.reset();
  station_motor_2._resetting = false;
  elevator_motor_1.reset();
  elevator_motor_1.blocked = true;
}

void loop() {
  /*if(station_motor_2.is_resetting() || (ftduino.input_get(station_motor_2_button)==0 && !station_motor_2.direction)){
    elevator_motor_1.blocked = true;
  } else{
    elevator_motor_1.blocked = false;
  }*/
  if(station_motor_2.reset_state() && !station_motor_2.is_resetting() && millis() > 5000 && !global_reset){
    elevator_motor_1.blocked = false;
    elevator_motor_1.start();
    station_motor_2.stop();
    station_motor_1.stop();
    delay(5500);
    //station_motor_1.start();
    station_motor_1.reset_steps();
    station_motor_2.reset_steps();
    station_motor_2.direction = false;
    station_motor_2.start();
    delay(500);
  }
  station_motor_1.cycle();
  station_motor_2.cycle();
  elevator_motor_1.cycle();
  if(ftduino.input_get(reset_button) == 0){
    station_motor_1.reset();
    station_motor_2.reset();
    elevator_motor_1.reset();
    elevator_motor_1.blocked = true;
    global_reset = true;
    last_reset = millis();
  } else if(millis()-last_reset > 5000){
    global_reset = false;
  }
}
