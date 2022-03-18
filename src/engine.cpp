#include "engine.h"
#include "Arduino.h"
#include <Ftduino.h>

Engine::Engine(uint8_t port, uint8_t cport, TriggerType trigger_type,
               uint8_t trigger_port, unsigned int trigger_value,
               uint8_t reset_port, TriggerType finish_type, int finish_port, unsigned int finish_value, bool reversed)
    : direction(false), trigger_type(trigger_type), trigger_port(trigger_port),
      trigger_value(trigger_value), finish_type(finish_type), finish_port(finish_port), finish_value(finish_value),
      reset_port(reset_port), _port(port), _cport(cport), _running(false),
      _resetting(false), _reversed(reversed) {
  blocked = false;
  _delay = 0;
  _timer = 0;
}

void Engine::start() {
  _running = true;
  ftduino.motor_set(_port, (int)direction + 1,
                    Ftduino::MAX); // (int)direction+1 is required because i
                                   // dont want to use an if statement
}

void Engine::stop() {
  _running = false;
  ftduino.motor_set(_port, Ftduino::OFF, Ftduino::MAX);
}

void Engine::change_direction() {
  direction = !direction;
  stop();
  start();
}

unsigned int Engine::get_steps() const { return ftduino.counter_get(_cport); }

void Engine::reset_steps() const { ftduino.counter_clear(_cport); }

void Engine::reset() {
  _running = true;
  _resetting = true;
  direction = true;
  reset_steps();
  start();
}

bool Engine::is_resetting() { return _resetting; }

bool Engine::reset_state() {
  return ((_running && _resetting && ftduino.input_get(reset_port) == 0) || (_reversed && _running && ftduino.input_get(reset_port) == 0));
}

bool Engine::trigger_state() {
  return (trigger_type == TriggerType::lightsensor && ftduino.input_get(trigger_port) >= trigger_value) || (trigger_type == TriggerType::button && ftduino.input_get(trigger_port) == trigger_value) || (trigger_type == TriggerType::steps && ftduino.counter_get(trigger_port) >= trigger_value);
}

bool Engine::finish_state() {
  return (finish_type == TriggerType::steps && ftduino.counter_get(finish_port) >= finish_value) || (finish_type == TriggerType::button && ftduino.input_get(finish_port) == finish_value);
}

void Engine::cycle() {
  if (!_running && !_resetting && trigger_state() && !_reversed && !blocked) {
    reset_steps();
    start();
    _running = true;
  } else if (!_running && !_resetting && trigger_state() && !blocked) {
    reset_steps();
    start();
    _running = true;
  } else if (_running && !_resetting && finish_state() &&
             !_reversed) {
    reset();
  } else if (reset_state()) {
    direction = false;
    _running = false;
    _resetting = false;
    stop();
    reset_steps();
    if (_reversed) {
      _running = true;
      start();
    }
  } else if (_running && _reversed && !_resetting && finish_state()) {
    stop();
    reset_steps();
    _running = false;
    _resetting = false;
    direction = true;
  }
}

void Engine::set_delay(int delay){
  _delay = delay;
}
