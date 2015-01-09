struct FourPlayers : Controller {
  uint2 data1();
  uint5 data2();
  void latch(bool data);
  FourPlayers(uint2 port);

private:
  bool latched;
  unsigned counter;
};
