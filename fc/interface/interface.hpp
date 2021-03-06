#ifndef FC_HPP
namespace Famicom {
#endif

struct ID {
  enum : unsigned {
    System,
    Famicom,
    PlayChoice10,
  };

  enum : unsigned {
    Manifest,
    ProgramROM,
    ProgramRAM,
    CharacterROM,
    CharacterRAM,
  };

  enum : unsigned {
    Port1 = 1,
    Port2 = 2,
    ExpansionPort = 4,
  };
};

struct Interface : Emulator::Interface {
  string title();
  double videoFrequency();
  double audioFrequency();

  bool loaded();
  string sha256();
  unsigned group(unsigned id);
  void load(unsigned id);
  void save();
  void load(unsigned id, const stream& stream);
  void save(unsigned id, const stream& stream);
  void unload();

  void connect(unsigned port, unsigned device);
  void power();
  void reset();
  void run();

  serializer serialize();
  bool unserialize(serializer&);

  void cheatSet(const lstring&);

  void paletteUpdate(PaletteMode mode);

  //debugger functions
  void exportMemory();

  Interface();

private:
  vector<Device> device;
};

extern Interface* interface;

#ifndef FC_HPP
}
#endif
