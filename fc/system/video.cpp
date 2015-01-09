#ifdef SYSTEM_CPP

Video video;

void Video::generate_palette(Emulator::Interface::PaletteMode mode) {
  unsigned emphasis;
  unsigned luma;
  unsigned chroma;
  for(unsigned color = 0; color < (1 << 9); color++) {
    if(mode == Emulator::Interface::PaletteMode::Literal) {
      palette[color] = color;
      continue;
    }
    switch(ppu.revision) {
    case PPU::Revision::RP2C02:
    case PPU::Revision::RP2C07:
      if(mode == Emulator::Interface::PaletteMode::Standard) {
        palette[color] = generate_color(color, 2.0, 0.0, 1.0, 1.0, 2.2);
      } else if(mode == Emulator::Interface::PaletteMode::Channel) {
        emphasis = image::normalize((color >> 6) &  7, 3, 16);
        luma     = image::normalize((color >> 4) &  3, 2, 16);
        chroma   = image::normalize((color >> 0) & 15, 4, 16);
        palette[color] = interface->videoColor(color, 0, emphasis, luma, chroma);
      } else if(mode == Emulator::Interface::PaletteMode::Emulation) {
        palette[color] = generate_color(color, 2.0, 0.0, 1.0, 1.0, 1.8);
      }
      break;
    case PPU::Revision::RP2C03:
    case PPU::Revision::RP2C05:
      palette[color] = interface->videoColor(color, 0,
        uclamp<16>(((PPU::RP2C03[color & 63] >> 6) & 7) * 9362.25),
        uclamp<16>(((PPU::RP2C03[color & 63] >> 3) & 7) * 9362.25),
        uclamp<16>(((PPU::RP2C03[color & 63] >> 0) & 7) * 9362.25)
      );
      break;
    case PPU::Revision::RP2C04_1:
      palette[color] = interface->videoColor(color, 0,
        uclamp<16>(((PPU::RP2C04_1[color & 63] >> 6) & 7) * 9362.25),
        uclamp<16>(((PPU::RP2C04_1[color & 63] >> 3) & 7) * 9362.25),
        uclamp<16>(((PPU::RP2C04_1[color & 63] >> 0) & 7) * 9362.25)
      );
      break;
    case PPU::Revision::RP2C04_2:
      palette[color] = interface->videoColor(color, 0,
        uclamp<16>(((PPU::RP2C04_2[color & 63] >> 6) & 7) * 9362.25),
        uclamp<16>(((PPU::RP2C04_2[color & 63] >> 3) & 7) * 9362.25),
        uclamp<16>(((PPU::RP2C04_2[color & 63] >> 0) & 7) * 9362.25)
      );
      break;
    case PPU::Revision::RP2C04_3:
      palette[color] = interface->videoColor(color, 0,
        uclamp<16>(((PPU::RP2C04_3[color & 63] >> 6) & 7) * 9362.25),
        uclamp<16>(((PPU::RP2C04_3[color & 63] >> 3) & 7) * 9362.25),
        uclamp<16>(((PPU::RP2C04_3[color & 63] >> 0) & 7) * 9362.25)
      );
      break;
    case PPU::Revision::RP2C04_4:
      palette[color] = interface->videoColor(color, 0,
        uclamp<16>(((PPU::RP2C04_4[color & 63] >> 6) & 7) * 9362.25),
        uclamp<16>(((PPU::RP2C04_4[color & 63] >> 3) & 7) * 9362.25),
        uclamp<16>(((PPU::RP2C04_4[color & 63] >> 0) & 7) * 9362.25)
      );
      break;
    } 
  }
}

Video::Video() {
  palette = new uint32_t[1 << 9]();
}

Video::~Video() {
  delete[] palette;
}

//internal

const uint8_t Video::cursor[15 * 15] = {
  0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
  0,0,0,0,1,1,2,2,2,1,1,0,0,0,0,
  0,0,0,1,2,2,1,2,1,2,2,1,0,0,0,
  0,0,1,2,1,1,0,1,0,1,1,2,1,0,0,
  0,1,2,1,0,0,0,1,0,0,0,1,2,1,0,
  0,1,2,1,0,0,1,2,1,0,0,1,2,1,0,
  1,2,1,0,0,1,1,2,1,1,0,0,1,2,1,
  1,2,2,1,1,2,2,2,2,2,1,1,2,2,1,
  1,2,1,0,0,1,1,2,1,1,0,0,1,2,1,
  0,1,2,1,0,0,1,2,1,0,0,1,2,1,0,
  0,1,2,1,0,0,0,1,0,0,0,1,2,1,0,
  0,0,1,2,1,1,0,1,0,1,1,2,1,0,0,
  0,0,0,1,2,2,1,2,1,2,2,1,0,0,0,
  0,0,0,0,1,1,2,2,2,1,1,0,0,0,0,
  0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
};

void Video::draw_cursor(uint16_t color, int x, int y) {
  uint32_t* data = (uint32_t*)ppu.buffer;

  for(int cy = 0; cy < 15; cy++) {
    int vy = y + cy - 7;
    if(vy <= 0 || vy >= 240) continue;  //do not draw offscreen

    for(int cx = 0; cx < 15; cx++) {
      int vx = x + cx - 7;
      if(vx < 0 || vx >= 256) continue;  //do not draw offscreen
      uint8_t pixel = cursor[cy * 15 + cx];
      if(pixel == 0) continue;
      uint32_t pixelcolor = (pixel == 1) ? 0x0f : color;

      *((uint32_t*)data + vy * 256 + vx) = pixelcolor;
    }
  }
}

void Video::update() {
  if(input.ports[1] == Input::Device::Zapper || input.ports[2] == Input::Device::BeamGun) {
    draw_cursor(0x12, input.x, input.y);
  }

  interface->videoRefresh(video.palette, ppu.buffer, 4 * 256, 256, 240);
}

void Video::scanline() {
  unsigned y = ppu.status.ly;
  if(y >= 240) return;
}

uint32_t Video::generate_color(
  unsigned n, double saturation, double hue,
  double contrast, double brightness, double gamma
) {
  signed color = (n & 0x0f), level = color < 0xe ? (n >> 4) & 3 : 1;

  static const double black = 0.518, white = 1.962, attenuation = 0.746;
  static const double levels[8] = {
    0.350, 0.518, 0.962, 1.550,
    1.094, 1.506, 1.962, 1.962,
  };

  double lo_and_hi[2] = {
    levels[level + 4 * (color == 0x0)],
    levels[level + 4 * (color <  0xd)],
  };

  double y = 0.0, i = 0.0, q = 0.0;
  auto wave = [](signed p, signed color) { return (color + p + 8) % 12 < 6; };
  for(signed p = 0; p < 12; p++) {
    double spot = lo_and_hi[wave(p, color)];

    if(((n & 0x040) && wave(p, 12))
    || ((n & 0x080) && wave(p,  4))
    || ((n & 0x100) && wave(p,  8))
    ) spot *= attenuation;

    double v = (spot - black) / (white - black);

    v = (v - 0.5) * contrast + 0.5;
    v *= brightness / 12.0;

    y += v;
    i += v * std::cos((3.141592653 / 6.0) * (p + hue));
    q += v * std::sin((3.141592653 / 6.0) * (p + hue));
  }

  i *= saturation;
  q *= saturation;

  auto gammaAdjust = [=](double f) { return f < 0.0 ? 0.0 : std::pow(f, 2.2 / gamma); };
  unsigned r = 65535.0 * gammaAdjust(y +  0.946882 * i +  0.623557 * q);
  unsigned g = 65535.0 * gammaAdjust(y + -0.274788 * i + -0.635691 * q);
  unsigned b = 65535.0 * gammaAdjust(y + -1.108545 * i +  1.709007 * q);

  return interface->videoColor(n, 0, uclamp<16>(r), uclamp<16>(g), uclamp<16>(b));
}

#endif
