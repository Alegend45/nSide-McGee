#include "chip/chip.hpp"
#include "board/board.hpp"

struct Cartridge : Thread, property<Cartridge> {
  enum class Region : unsigned {
    NTSC,
    PAL,
  };

  static void Main();
  void main();

  void load(System::Revision revision);
  void unload();

  void power();
  void reset();

  readonly<bool> loaded;
  readonly<string> sha256;

  Region region;

  struct Information {
    string markup;
    string title;
  } information;

  string title();

  struct Memory {
    unsigned id;
    string name;
  };
  vector<Memory> memory;

  void serialize(serializer&);
  Cartridge();

//privileged:
  Board *board;

  uint8 prg_read(unsigned addr);
  void prg_write(unsigned addr, uint8 data);

  uint8 chr_read(unsigned addr);
  void chr_write(unsigned addr, uint8 data);

  //scanline() is for debugging purposes only:
  //boards must detect scanline edges on their own
  void scanline(unsigned y);
};

extern Cartridge cartridge;
