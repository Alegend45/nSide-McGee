string Famicom = R"(

database revision=2012-10-22

release
  cartridge region=NTSC
    board type=HVC-TLSROM
    chip type=MMC3B
    prg
      rom name=program.rom size=0x40000
    chr
      rom name=character.rom size=0x20000
  information
    title:    アルマジロ
    name:     Armadillo
    region:   JP
    revision: 1.0
    serial:   IGS-9T
    sha256:   54526dc9444c0eb4b0e5814f98b5e522bcb9881a6f2c0644fc7a21ca8c03502b
    configuration
      rom name=program.rom size=0x40000
      rom name=character.rom size=0x20000

release
  cartridge region=NTSC
    board type=HVC-UNROM
    chip type=74HC08
    mirror mode=horizontal
    prg
      rom name=program.rom size=0x20000
    chr
      ram size=0x2000
  information
    title:    クレイジークライマー
    name:     Crazy Climber
    region:   JP
    revision: 1.0
    serial:   NBF-CY (04)
    sha256:   e24d3c754dce20e27046afeacb2dfc217950d4be766ded80c20283392cb3891e
    configuration
      rom name=program.rom size=0x20000
      ram name=character.ram size=0x2000

release
  cartridge region=NTSC
    board type=FDS
    prg
      rom name=program.rom size=0x2000
      ram size=0x8000
    chr
      ram size=0x2000
  information
    title:    Famicom Disk System
    name:     Famicom Disk System
    region:   JP
    revision: 1.0
    serial:   N/A
    sha256:   99c18490ed9002d9c6d999b9d8d15be5c051bdfa7cc7e73318053c9a994b0178

release
  cartridge region=NTSC
    board type=HVC-CNROM
      security pass=0x1
    mirror mode=vertical
    prg
      rom name=program.rom size=0x8000
    chr
      rom name=character.rom size=0x2000
  information
    title:    スパイVSスパイ
    name:     Spy vs. Spy
    region:   JP
    revision: 1.0
    serial:   KSC-SP
    sha256:   a9cf92ec1a080faa195d0b3dcb48fbb5ee3362f0f2f14e14e4257def48ac4346
    configuration
      rom name=program.rom size=0x8000
      rom name=character.rom size=0x2000

release
  cartridge region=NTSC
    board type=NES-UNROM
    chip type=74HC32
    mirror mode=vertical
    prg
      rom name=program.rom size=0x20000
    chr
      ram size=0x2000
  information
    title:    DuckTales
    name:     DuckTales
    region:   NA
    revision: 1.0
    serial:   NES-UK-USA
    sha256:   8ba8baed01a9fbaf1e9ff29e0c9825db1963ac2aff211d6f1f3bcfd3839e2013
    configuration
      rom name=program.rom size=0x20000
      ram name=character.ram size=0x2000

release
  cartridge region=NTSC
    board type=NES-TQROM
    chip type=MMC3B
    prg
      rom name=program.rom size=0x20000
    chr
      rom name=character.rom size=0x10000
      ram size=0x2000
  information
    title:    High Speed
    name:     High Speed
    region:   NA
    revision: 1.0
    serial:   NES-8H-USA
    sha256:   710e907230bbd82143286b40a56a298b25cf326697a9f07bfd8e043c1936a4b1
    configuration
      rom name=program.rom size=0x20000
      rom name=character.rom size=0x10000
      rom name=nametable.ram size=0x2000

release
  cartridge region=NTSC
    board type=NES-SNROM
    chip type=MMC1B2
    prg
      rom name=program.rom size=0x20000
      ram name=save.ram size=0x2000
    chr
      ram size=0x2000
  information
    title:    The Legend of Zelda
    name:     Legend of Zelda, The
    region:   NA
    revision: 1.1
    serial:   NES-ZL-USA
    serial:   ec0d4ebf6d2fcecd1d95fef7329954efe79676959bc281ea908b226459bc6dc2
    configuration
      rom name=program.rom size=0x20000
      ram name=save.ram size=0x2000
      ram name=character.ram size=0x2000

release
  cartridge region=NTSC
    board type=NES-TQROM
    chip type=MMC3A
    prg
      rom name=program.rom size=0x20000
    chr
      rom name=character.rom size=0x10000
      ram size=0x2000
  information
    title:    Pin-Bot
    name:     Pin-Bot
    region:   NA
    revision: 1.0
    serial:   NES-IO-USA
    sha256:   f4ddb0f1a02f823ebed30b55547344de3c8fb9d87254ff993584373ecadd9141
    configuration
      rom name=program.rom size=0x20000
      rom name=character.rom size=0x10000
      rom name=nametable.ram size=0x2000

release
  cartridge region=NTSC
    board type=NES-HKROM
    chip type=MMC6B
    prg
      rom name=program.rom size=0x40000
      ram name=save.ram size=0x400
    chr
      rom name=character.rom size=0x40000
  information
    title:    StarTropics
    name:     StarTropics
    region:   NA
    revision: 1.0
    serial:   NES-OC-USA
    sha256:   69de2c7552fa81ca5921da0e457abf1be35f18ffbad159788a76141be59c9f6b
    configuration
      rom name=program.rom size=0x40000
      ram name=save.ram size=0x400
      rom name=character.rom size=0x40000

release
  cartridge region=NTSC
    board type=NES-NROM-256
    mirror mode=vertical
    prg
      rom name=program.rom size=0x8000
    chr
      rom name=character.rom size=0x2000
  information
    title:    Super Mario Bros.
    name:     Super Mario Bros.
    region:   NA
    revision: 1.0
    serial:   NES-SM-USA
    sha256:   fcb6a0ef3a20c19b356005fbb21dc8009563b1cb5a9aaebc8e9386b4a8c5912e
    configuration
      rom name=program.rom size=0x8000
      rom name=character.rom size=0x2000

release
  cartridge region=NTSC
    board type=NES-TSROM
    chip type=MMC3B
    prg
      rom name=program.rom size=0x40000
      ram size=0x2000
    chr
      rom name=character.rom size=0x20000
  information
    title:    Super Mario Bros. 3
    name:     Super Mario Bros. 3
    region:   NA
    revision: 1.1
    serial:   NES-UM-USA
    serial:   NES-UM-CAN
    sha256:   959fdd32c71735d6fb2bd16a646d39f4ee65623273dd035e6a968e991bd13ef8
    configuration
      rom name=program.rom size=0x40000
      ram name=work.ram size=0x2000
      rom name=character.rom size=0x20000

release
  cartridge region=NTSC
    board type=NES-TKROM
    chip type=MMC3C
    prg
      rom name=program.rom size=0x40000
      ram name=save.ram size=0x2000
    chr
      rom name=character.rom size=0x40000
  information
    title:    Wario's Woods
    name:     Wario's Woods
    region:   NA
    revision: 1.0
    serial:   NES-WB-USA
    sha256:   c12771e8155b030eff0081bfabd98e57a162d6592899f29dd16f141f0e6e08a3
    configuration
      rom name=program.rom size=0x40000
      ram name=save.ram size=0x2000
      rom name=character.rom size=0x40000

release
  cartridge region=NTSC
    board type=NES-CNROM
    mirror mode=horizontal
    prg
      rom name=program.rom size=0x8000
    chr
      rom name=character.rom size=0x8000
  information
    title:    World Class Track Meet
    name:     World Class Track Meet
    region:   NA
    revision: 1.1
    serial:   NES-WT-USA
    sha256:   ac766a8d99bfd8e95c02b9a9c68279c72ba5b3307b78edc67b52781ed185fa89
    configuration
      rom name=program.rom size=0x8000
      rom name=character.rom size=0x8000

release
  cartridge region=NTSC
    board type=NES-HKROM
    chip type=MMC6B
    prg
      rom name=program.rom size=0x40000
      ram name=save.ram size=0x400
    chr
      rom name=character.rom size=0x40000
  information
    title:    Zoda's Revenge: StarTropics 2
    name:     Zoda's Revenge - StarTropics 2
    region:   NA
    revision: 1.0
    serial:   NES-6C-USA
    sha256:   d0850075065ecbd125a33accc952de5d012527be45aa14a1b8223a9adf1643ae
    configuration
      rom name=program.rom size=0x40000
      ram name=save.ram size=0x400
      rom name=character.rom size=0x40000

release
  cartridge region=PAL
    board type=NES-TKROM
    chip type=MMC3C
    prg
      rom name=program.rom size=0x40000
      ram name=save.ram size=0x2000
    chr
      rom name=character.rom size=0x40000
  information
    title:    Wario's Woods
    name:     Wario's Woods
    region:   EU
    revision: 1.0
    serial:   NES-WB-NOE
    sha256:   9fe815d8fd175ef9ef03fb010638f2b6b7aa9d11d5a40eda2476450918543e6f
    configuration
      rom name=program.rom size=0x40000
      ram name=save.ram size=0x2000
      rom name=character.rom size=0x40000

)";
