struct Zapper : Controller {
  void enter();
  uint5 data();
  uint2 data1();
  uint5 data2();
  bool light();
  void latch(bool data);
  Zapper(uint2 port);

//private:
  bool latched;

  signed x, y;

  bool trigger;

  bool triggerlock;
};
