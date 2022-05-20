#include <Ftduino.h>

#define buffer_motor Ftduino::M4
#define trigger_button Ftduino::I1
#define finish_button Ftduino::I2
#define start_button Ftduino::I3

#define speed 20

bool direction = true;

void setup(){
    ftduino.init();
    ftduino.input_set_mode(finish_button, Ftduino::SWITCH);
    ftduino.input_set_mode(trigger_button, Ftduino::SWITCH);
    ftduino.input_set_mode(start_button, Ftduino::SWITCH);
    ftduino.motor_set(buffer_motor, direction+1, speed);
    delay(100);
    while (ftduino.input_get(finish_button)) {}
    ftduino.motor_set(buffer_motor, Ftduino::OFF, speed);
    direction = !direction;
}
void loop(){
    if(!ftduino.input_get(trigger_button)){
        ftduino.motor_set(buffer_motor, direction+1, speed);
        while((ftduino.input_get(finish_button) && direction) || (ftduino.input_get(start_button) && !direction)){}
        ftduino.motor_set(buffer_motor, Ftduino::OFF, speed);
        direction = !direction;
}
}