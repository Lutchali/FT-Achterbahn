#ifndef ENGINE_H
#define ENGINE_H

class Engine{
  public:
    bool direction;
    Engine(int port, int cport);
    void start();
    void stop();
    void change_direction();
    unsigned int get_steps();
    void reset_steps();
  private:
    int _port;
    int _cport;
};

#endif