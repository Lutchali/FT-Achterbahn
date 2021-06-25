#include "engine.h"
#include <Ftduino.h>

Engine::Engine(int port, int cport){
  _port = port;
  _cport = cport;
  direction = 0;
}

void Engine::start(){
  ftduino.motor_set(_port, (int)direction+1, Ftduino::MAX); // (int)direction+1 is required because i dont want to use an if statement
}

void Engine::stop(){
  ftduino.motor_set(_port, Ftduino::OFF, Ftduino::MAX);
}

void Engine::change_direction(){
  direction = !direction;
  stop();
  start();
}

unsigned int Engine::get_steps(){
  return ftduino.counter_get(_cport);
}

void Engine::reset_steps(){
  ftduino.counter_clear(_cport);
}
