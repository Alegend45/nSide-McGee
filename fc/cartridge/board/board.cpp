#include "bandai-fcg.cpp"
#include "konami-vrc1.cpp"
#include "konami-vrc2.cpp"
#include "konami-vrc3.cpp"
#include "konami-vrc4.cpp"
#include "konami-vrc6.cpp"
#include "konami-vrc7.cpp"
#include "namco-163.cpp"
#include "namco-34xx.cpp"
#include "nes-axrom.cpp"
#include "nes-bnrom.cpp"
#include "nes-cxrom.cpp"
#include "nes-exrom.cpp"
#include "nes-fxrom.cpp"
#include "nes-gxrom.cpp"
#include "nes-hkrom.cpp"
#include "nes-nrom.cpp"
#include "nes-pxrom.cpp"
#include "nes-sxrom.cpp"
#include "nes-txrom.cpp"
#include "nes-uxrom.cpp"
#include "sunsoft-4.cpp"
#include "sunsoft-5b.cpp"
#include "fds.cpp"

// Unlicensed board definitions; feel free to remove
#include "unlicensed/camerica.cpp"
#include "unlicensed/colordreams-74377.cpp"
#include "unlicensed/mlt-action52.cpp"
#include "unlicensed/nina.cpp"
#include "unlicensed/noconflicts-cnrom.cpp"
#include "unlicensed/single-chip.cpp"

uint8 Board::Memory::read(unsigned addr) const {
  return data[mirror(addr, size)];
}

void Board::Memory::write(unsigned addr, uint8 byte) {
  if(writable) data[mirror(addr, size)] = byte;
}

unsigned Board::mirror(unsigned addr, unsigned size) {
  unsigned base = 0;
  if(size) {
    unsigned mask = 1 << 23;
    while(addr >= size) {
      while(!(addr & mask)) mask >>= 1;
      addr -= mask;
      if(size > mask) {
        size -= mask;
        base += mask;
      }
      mask >>= 1;
    }
    base += addr;
  }
  return base;
}

void Board::main() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    cartridge.clock += (system.region == System::Region::NTSC ? 12 : 16) * 4095;
    tick();
  }
}

void Board::tick() {
  cartridge.clock += (system.region == System::Region::NTSC ? 12 : 16);
  if(cartridge.clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}

uint8 Board::chr_read(unsigned addr) {
  if(chrram.size) return chrram.data[mirror(addr, chrram.size)];
  if(chrrom.size) return chrrom.data[mirror(addr, chrrom.size)];
  return 0u;
}

void Board::chr_write(unsigned addr, uint8 data) {
  if(chrram.size) chrram.data[mirror(addr, chrram.size)] = data;
}

void Board::power() {
}

void Board::reset() {
}

void Board::serialize(serializer& s) {
  if(prgram.size) s.array(prgram.data, prgram.size);
  if(chrram.size) s.array(chrram.data, chrram.size);
}

Board::Board(Markup::Node& document) {
  cartridge.board = this;
  auto cartridge = document["cartridge"];

  information.type = cartridge["board/type"].data;
  information.battery = cartridge["prg/ram/name"].exists();

  auto prom = cartridge["prg/rom"];
  auto pram = cartridge["prg/ram"];
  auto crom = cartridge["chr/rom"];
  auto cram = cartridge["chr/ram"];

  prgrom.size = numeral(prom["size"].data);
  prgram.size = numeral(pram["size"].data);
  chrrom.size = numeral(crom["size"].data);
  chrram.size = numeral(cram["size"].data);

  if(prgrom.size) prgrom.data = new uint8[prgrom.size]();
  if(prgram.size) prgram.data = new uint8[prgram.size]();
  if(chrrom.size) chrrom.data = new uint8[chrrom.size]();
  if(chrram.size) chrram.data = new uint8[chrram.size]();

  if(prom["name"].data) interface->loadRequest(ID::ProgramROM, prom["name"].data);
  if(pram["name"].data) interface->loadRequest(ID::ProgramRAM, pram["name"].data);
  if(crom["name"].data) interface->loadRequest(ID::CharacterROM, crom["name"].data);
  if(cram["name"].data) interface->loadRequest(ID::CharacterRAM, cram["name"].data);

  if(pram["name"].data) Famicom::cartridge.memory.append({ID::ProgramRAM, pram["name"].data});
  if(cram["name"].data) Famicom::cartridge.memory.append({ID::CharacterRAM, cram["name"].data});

  prgram.writable = true;
  chrram.writable = true;
}

Board::~Board() {
}

Board* Board::load(string manifest) {
  auto document = Markup::Document(manifest);
  cartridge.information.title = document["information/title"].text();
  cartridge.region = document["cartridge/region"].data != "PAL" ? Cartridge::Region::NTSC : Cartridge::Region::PAL;

  string type = document["cartridge/board/type"].text();

  if(substr(type,0,4) == "HVC-" || substr(type,0,4) == "NES-") {
    type = substr(type,4);
    if(type == "AMROM"   ) return new NES_AxROM(document);
    if(type == "ANROM"   ) return new NES_AxROM(document);
    if(type == "AN1ROM"  ) return new NES_AxROM(document);
    if(type == "AOROM"   ) return new NES_AxROM(document);

    if(type == "BNROM"   ) return new NES_BNROM(document);

    if(type == "BTR"     ) return new Sunsoft5B(document);

    if(type == "CNROM"   ) return new NES_CxROM(document);
    if(type == "CPROM"   ) return new NES_CxROM(document);

    if(type == "DEROM"   ) return new Namco34xx(document);
    if(type == "DE1ROM"  ) return new Namco34xx(document);
    if(type == "DRROM"   ) return new Namco34xx(document);

    if(type == "EKROM"   ) return new NES_ExROM(document);
    if(type == "ELROM"   ) return new NES_ExROM(document);
    if(type == "ETROM"   ) return new NES_ExROM(document);
    if(type == "EWROM"   ) return new NES_ExROM(document);

    if(type == "FJROM"   ) return new NES_FxROM(document);
    if(type == "FKROM"   ) return new NES_FxROM(document);

    if(type == "GNROM"   ) return new NES_GxROM(document);
    if(type == "MHROM"   ) return new NES_GxROM(document);

    if(type == "HKROM"   ) return new NES_HKROM(document);

    if(type == "JLROM"   ) return new Sunsoft5B(document);
    if(type == "JSROM"   ) return new Sunsoft5B(document);

    if(type == "NROM"    ) return new NES_NROM(document);
    if(type == "NROM-128") return new NES_NROM(document);
    if(type == "NROM-256") return new NES_NROM(document);
    if(type == "HROM"    ) return new NES_NROM(document);
    if(type == "RROM"    ) return new NES_NROM(document);
    if(type == "RROM-128") return new NES_NROM(document);
    if(type == "RTROM"   ) return new NES_NROM(document);
    if(type == "SROM"    ) return new NES_NROM(document);
    if(type == "STROM"   ) return new NES_NROM(document);

    if(type == "PEEOROM" ) return new NES_PxROM(document);
    if(type == "PNROM"   ) return new NES_PxROM(document);

    if(type == "SAROM"   ) return new NES_SxROM(document);
    if(type == "SBROM"   ) return new NES_SxROM(document);
    if(type == "SCROM"   ) return new NES_SxROM(document);
    if(type == "SC1ROM"  ) return new NES_SxROM(document);
    if(type == "SEROM"   ) return new NES_SxROM(document);
    if(type == "SFROM"   ) return new NES_SxROM(document);
    if(type == "SFEXPROM") return new NES_SxROM(document);
    if(type == "SGROM"   ) return new NES_SxROM(document);
    if(type == "SHROM"   ) return new NES_SxROM(document);
    if(type == "SH1ROM"  ) return new NES_SxROM(document);
    if(type == "SIROM"   ) return new NES_SxROM(document);
    if(type == "SJROM"   ) return new NES_SxROM(document);
    if(type == "SKROM"   ) return new NES_SxROM(document);
    if(type == "SLROM"   ) return new NES_SxROM(document);
    if(type == "SL1ROM"  ) return new NES_SxROM(document);
    if(type == "SL2ROM"  ) return new NES_SxROM(document);
    if(type == "SL3ROM"  ) return new NES_SxROM(document);
    if(type == "SLRROM"  ) return new NES_SxROM(document);
    if(type == "SMROM"   ) return new NES_SxROM(document);
    if(type == "SNROM"   ) return new NES_SxROM(document);
    if(type == "SNWEPROM") return new NES_SxROM(document);
    if(type == "SOROM"   ) return new NES_SxROM(document);
    if(type == "SUROM"   ) return new NES_SxROM(document);
    if(type == "SXROM"   ) return new NES_SxROM(document);

    if(type == "TBROM"   ) return new NES_TxROM(document);
    if(type == "TEROM"   ) return new NES_TxROM(document);
    if(type == "TFROM"   ) return new NES_TxROM(document);
    if(type == "TGROM"   ) return new NES_TxROM(document);
    if(type == "TKROM"   ) return new NES_TxROM(document);
    if(type == "TKSROM"  ) return new NES_TxROM(document);
    if(type == "TLROM"   ) return new NES_TxROM(document);
    if(type == "TL1ROM"  ) return new NES_TxROM(document);
    if(type == "TL2ROM"  ) return new NES_TxROM(document);
    if(type == "TLSROM"  ) return new NES_TxROM(document);
    if(type == "TNROM"   ) return new NES_TxROM(document);
    if(type == "TQROM"   ) return new NES_TxROM(document);
    if(type == "TR1ROM"  ) return new NES_TxROM(document);
    if(type == "TSROM"   ) return new NES_TxROM(document);
    if(type == "TVROM"   ) return new NES_TxROM(document);

    if(type == "UNROM"   ) return new NES_UxROM(document);
    if(type == "UOROM"   ) return new NES_UxROM(document);
  }

  if(type == "ACCLAIM-MC-ACC") return new NES_TxROM(document);

  if(type == "AVE-NINA-01") return new Nina(document);
  if(type == "AVE-NINA-02") return new Nina(document);
  if(type == "AVE-NINA-03") return new Nina(document);
  if(type == "AVE-NINA-06") return new Nina(document);

  if(type == "BANDAI-74*161/32") return new NES_CxROM(document);
  if(type == "BANDAI-CNROM"    ) return new NES_CxROM(document);
  if(type == "BANDAI-FCG"      ) return new BandaiFCG(document);
  if(type == "BANDAI-FCG-1"    ) return new BandaiFCG(document);
  if(type == "BANDAI-FCG-2"    ) return new BandaiFCG(document);
  if(type == "BANDAI-GNROM"    ) return new NES_GxROM(document);
  if(type == "BANDAI-JUMP2"    ) return new BandaiFCG(document);
  if(type == "BANDAI-LZ93D50"  ) return new BandaiFCG(document);
  if(type == "BANDAI-NROM-128" ) return new NES_NROM(document);
  if(type == "BANDAI-NROM-256" ) return new NES_NROM(document);
  if(type == "BANDAI-PT-544"   ) return new NES_CxROM(document);

  if(type == "IREM-BNROM"   ) return new NES_BNROM(document);
  if(type == "IREM-FCG-1"   ) return new BandaiFCG(document);
  if(type == "IREM-NROM-128") return new NES_NROM(document);
  if(type == "IREM-NROM-256") return new NES_NROM(document);
  if(type == "IREM-UNROM"   ) return new NES_UxROM(document);
  //TODO:
  //IREM-G101 (iNES 032)
  //IREM-H3001 (iNES 065)
  //IREM-74*161/161/21/138 (iNES 077)
  //IREM-HOLYDIVER (iNES 078)
  //IREM-TAM-S1 (iNES 097)

  if(type == "KONAMI-CNROM"   ) return new NES_CxROM(document);
  if(type == "KONAMI-NROM-128") return new NES_NROM(document);
  if(type == "KONAMI-SLROM"   ) return new NES_SxROM(document);
  if(type == "KONAMI-TLROM"   ) return new NES_TxROM(document);
  if(type == "KONAMI-UNROM"   ) return new NES_UxROM(document);
  if(type == "KONAMI-VRC-1"   ) return new KonamiVRC1(document);
  if(type == "KONAMI-VRC-2"   ) return new KonamiVRC2(document);
  if(type == "KONAMI-VRC-3"   ) return new KonamiVRC3(document);
  if(type == "KONAMI-VRC-4"   ) return new KonamiVRC4(document);
  if(type == "KONAMI-VRC-6"   ) return new KonamiVRC6(document);
  if(type == "KONAMI-VRC-7"   ) return new KonamiVRC7(document);

  if(type == "NAMCOT-163" ) return new Namco163(document);
  if(type == "NAMCOT-3301") return new NES_NROM(document);
  if(type == "NAMCOT-3302") return new NES_NROM(document);
  if(type == "NAMCOT-3305") return new NES_NROM(document);
  if(type == "NAMCOT-3311") return new NES_NROM(document);
  if(type == "NAMCOT-3312") return new NES_NROM(document);
  if(type == "NAMCOT-3401") return new Namco34xx(document);
  if(type == "NAMCOT-3406") return new Namco34xx(document);
  if(type == "NAMCOT-3407") return new Namco34xx(document);
  if(type == "NAMCOT-3416") return new Namco34xx(document);
  if(type == "NAMCOT-3417") return new Namco34xx(document);
  if(type == "NAMCOT-3443") return new Namco34xx(document);
  if(type == "NAMCOT-3446") return new Namco34xx(document);
  if(type == "NAMCOT-3453") return new Namco34xx(document);

  if(type == "SUNSOFT-4" ) return new Sunsoft4(document);
  if(type == "SUNSOFT-5B") return new Sunsoft5B(document);

  if(type == "FDS") return new FDS(document);

  // Unlicensed boards below; feel free to remove
  if(type == "CAMERICA-ALGN"     ) return new Camerica(document);
  if(type == "CAMERICA-ALGQ"     ) return new Camerica(document);
  if(type == "CAMERICA-BF9093"   ) return new Camerica(document);
  if(type == "CAMERICA-BF9096"   ) return new Camerica(document);
  if(type == "CAMERICA-BF9097"   ) return new Camerica(document);
  if(type == "COLORDREAMS-74*377") return new ColorDreams74377(document);
  if(type == "MLT-ACTION52"      ) return new MLT_Action52(document);
  if(type == "TENGEN-800002"     ) return new Namco34xx(document);
  if(type == "TENGEN-800003"     ) return new NES_NROM(document);
  if(type == "TENGEN-800004"     ) return new Namco34xx(document);
  if(type == "TENGEN-800008"     ) return new NES_CxROM(document);
  if(type == "TENGEN-800030"     ) return new Namco34xx(document);
  if(type == "TENGEN-800042"     ) return new Sunsoft4(document);
  // Homebrew boards; feel free to remove
  if(type == "NoConflicts-CNROM") return new NoConflicts_CNROM(document);
  if(type == "SingleChip") return new SingleChip(document);

  return nullptr;
}
