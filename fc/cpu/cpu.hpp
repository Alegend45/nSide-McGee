struct CPU : Processor::R6502, Thread {
  uint8 ram[0x0800];

  enum : bool { Threaded = true };
  alwaysinline void add_clocks(unsigned clocks);
  void synchronize_controllers();

  void enter();
  void main();
  void power();
  void reset();

  uint8 debugger_read(uint16 addr);

  uint8 ram_read(uint16 addr);
  void ram_write(uint16 addr, uint8 data);

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 data);

  void serialize(serializer&);

//privileged:
  struct Status {
    bool interrupt_pending;
    bool nmi_pending;
    bool nmi_line;
    bool irq_line;
    bool irq_apu_line;

    bool rdy_line;
    bool rdy_addr_valid;
    uint16 rdy_addr_value;

    bool oam_dma_pending;
    uint8 oam_dma_page;

    bool controller_latch;
    unsigned controller_port0;
    unsigned controller_port1;
  } status;

  static void Enter();

  //timing.cpp
  uint8 op_read(uint16 addr);
  void op_write(uint16 addr, uint8 data);
  void last_cycle();
  void nmi(uint16 &vector);

  void oam_dma();

  void set_nmi_line(bool);
  void set_irq_line(bool);
  void set_irq_apu_line(bool);

  void set_rdy_line(bool);
  void set_rdy_addr(bool valid, uint16 value = 0);
};

extern CPU cpu;
