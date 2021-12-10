#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>

enum class TriggerType { lightsensor, button, steps };

class Engine {
public:
  bool direction;
  TriggerType trigger_type;
  int trigger_port;
  unsigned int trigger_value;
  TriggerType finish_type;
  int finish_port;
  unsigned int finish_value;
  int reset_port;
  bool blocked;
  Engine(uint8_t port, uint8_t cport, TriggerType trigger_type,
         uint8_t trigger_port, unsigned int trigger_value, uint8_t reset_port, TriggerType finish_type, int finish_port, unsigned int finish_value, bool reversed);
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
  bool finish_state();

private:
  int _port;
  int _cport;
  bool _running;
  bool _resetting;
  bool _reversed;
};
#endif