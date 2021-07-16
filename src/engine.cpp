#include "engine.h"
#include <Ftduino.h>

Engine::Engine(uint8_t port, uint8_t cport, TriggerType trigger_type,
               uint8_t trigger_port, unsigned int trigger_value,
               uint8_t reset_port, unsigned int finish_steps)
    : direction(false), trigger(trigger_type), trigger_port(trigger_port),
      trigger_value(trigger_value), finish_steps(finish_steps),
      reset_port(reset_port), _port(port), _cport(cport), _running(false),
      _resetting(false) {}

void Engine::start() const {
  ftduino.motor_set(_port, (int)direction + 1,
                    Ftduino::MAX); // (int)direction+1 is required because i
                                   // dont want to use an if statement
}

void Engine::stop() const {
  ftduino.motor_set(_port, Ftduino::OFF, Ftduino::MAX);
}

void Engine::change_direction() {
  direction = !direction;
  stop();
  start();
}

unsigned int Engine::get_steps() const {
  if(!_resetting){
    return ftduino.counter_get(_cport);
  } else{
    return 0;
  }
}

void Engine::reset_steps() const { ftduino.counter_clear(_cport); }

void Engine::reset() {
  _running = true;
  _resetting = true;
  direction = true;
  reset_steps();
  start();
}

void Engine::cycle() {
  if (!_running && !_resetting &&
      (trigger == TriggerType::lightsensor || trigger == TriggerType::button) &&
      ftduino.input_get(trigger_port) >= trigger_value) {
    reset_steps();
    start();
    _running = true;
  } else if (!_running && !_resetting && trigger == TriggerType::steps &&
             ftduino.counter_get(trigger_port) >= trigger_value) {
    reset_steps();
    start();
    _running = true;
  } else if (_running && !_resetting && get_steps() >= finish_steps) {
    reset();
  } else if (_running && _resetting && ftduino.input_get(reset_port) == 0) {
    direction = false;
    _running = false;
    _resetting = false;
    stop();
    reset_steps();
  }
}
