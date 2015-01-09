// NES controller port pinout:
// ┌────
// │(7)  \
// │(2)(1)│
// │(3)(5)│
// │(4)(6)│
// └──────┘
// pin    name     port1                            port2
//  1:    +5v
//  2:    clock    $4016 read                       $4017 read
//  3:    latch    $4016.d0 write                   $4016.d0 write
//  4:    data0    $4016.d0 read                    $4017.d0 read
//  5:    data3    $4016.d3 read                    $4017.d3 read
//  6:    data4    $4016.d4 read                    $4017.d4 read
//  7:    gnd

struct Controller : Thread {
  enum : unsigned { Port1 = 0, Port2 = 1, ExpansionPort = 2 };
  const uint2 port;

  static void Enter();
  virtual void enter();
  void step(unsigned clocks);
  void synchronize_cpu();

  virtual uint5 data() { return 0; }
  virtual uint2 data1() { return 0; }
  virtual uint5 data2() { return 0; }
  virtual void latch(bool data) {}
  Controller(unsigned port);
};

#include "gamepad/gamepad.hpp"
#include "fourscore/fourscore.hpp"
#include "zapper/zapper.hpp"
#include "powerpad/powerpad.hpp"
#include "fourplayers/fourplayers.hpp"
