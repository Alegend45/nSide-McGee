struct Input {
  enum class Device : unsigned {
    // Controller Port Devices
    Joypad = 0,
    FourScore = 1,
    Zapper = 2,
    PowerPad = 3,

    // Famicom Expansion Devices
    FourPlayers = 0,
    BeamGun = 1,

    None = 4,
  };

  enum class JoypadID : unsigned {
    A  =  0, B    =  1, Select =  2, Start =  3,
    Up =  4, Down =  5, Left   =  6, Right =  7,
  };

  enum class ZapperID : unsigned {
    X = 0, Y = 1, Trigger = 2,
  };

  Controller* port1 = nullptr;
  Controller* port2 = nullptr;
  Controller* expansion = nullptr;

  void latch(bool data);
  uint5 data(bool port);
  bool zapper_light();
  void connect(uint2 port, Device device);

  void power();
  void reset();

  void serialize(serializer&);

  Device ports[3];

  bool latchdata;
  unsigned counters[2];

  signed x, y;

  Input();
  ~Input();
};

extern Input input;
