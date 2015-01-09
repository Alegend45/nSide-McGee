struct Video {
  uint32_t* palette;
  void generate_palette(Emulator::Interface::PaletteMode mode);
  Video();
  ~Video();

private:
  void update();
  void scanline();
  uint32_t generate_color(unsigned, double, double, double, double, double);
  static const uint8_t cursor[15 * 15];
  void draw_cursor(uint16_t color, int x, int y);

  friend class System;
};

extern Video video;
