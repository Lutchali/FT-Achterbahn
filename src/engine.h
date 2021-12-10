#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

enum class TriggerType { lightsensor, button, steps };

class Engine {
public:
  bool direction;
  TriggerType trigger;
  int trigger_port;
  unsigned int trigger_value;
  unsigned int finish_steps;
  int reset_port;
  bool blocked;
  Engine(uint8_t port, uint8_t cport, TriggerType trigger_type,
         uint8_t trigger_port, unsigned int trigger_value, uint8_t reset_port,
         unsigned int finish_steps, bool reversed);
  void start() const;
  void stop() const;
  void change_direction();
  unsigned int get_steps() const;
  void reset_steps() const;

  bool is_resetting();
  void cycle();
  void reset();
  bool reset_state();
  bool trigger_state();

private:
  int _port;
  int _cport;
  bool _running;
  bool _resetting;
  bool _reversed;
};
#endif