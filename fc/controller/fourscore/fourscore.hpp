struct FourScore : Controller {
  uint5 data();
  void latch(bool data);
  FourScore(uint2 port);

private:
  bool latched;
  unsigned counter;
};
