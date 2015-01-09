struct PowerPad : Controller {
  uint5 data();
  void latch(bool data);
  PowerPad(uint2 port);

private:
  bool latched;
  unsigned counter;

  bool b1, b2, b3, b4;
  bool b5, b6, b7, b8;
  bool b9, b10, b11, b12;
};
