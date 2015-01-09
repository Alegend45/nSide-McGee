#ifdef CONTROLLER_CPP

void Zapper::enter() {
  unsigned prev = 0;
  unsigned id = port < 2 ? (unsigned)Input::Device::Zapper : (unsigned)Input::Device::BeamGun;
  while(true) {
    unsigned next = ppu.status.ly * 256 + ppu.status.lx;

    if(next < prev) {
      //Vcounter wrapped back to zero; update cursor coordinates for start of new frame
      int nx = interface->inputPoll(port, id, (unsigned)Input::ZapperID::X);
      int ny = interface->inputPoll(port, id, (unsigned)Input::ZapperID::Y);
      nx += x;
      ny += y;
      x = max(-16, min(256 + 16, nx));
      y = max(-16, min(240 + 16, ny));
    }

    prev = next;
    step(2);
  }
}

uint5 Zapper::data() {
  unsigned id = port < 2 ? (unsigned)Input::Device::Zapper : (unsigned)Input::Device::BeamGun;
  //trigger is a button; it is edge sensitive
  trigger = false;
  bool newtrigger = interface->inputPoll(port, id, (unsigned)Input::ZapperID::Trigger);
  if(newtrigger && !triggerlock) {
    trigger = true;
    triggerlock = true;
  } else if(!newtrigger) {
    triggerlock = false;
  }

  return (trigger << 4) | (light() << 3);
}

uint2 Zapper::data1() {
  return 0;
}

uint5 Zapper::data2() {
  return data();
}

bool Zapper::light() {
  print("Begin Zapper Light Report\n");
  print(ppu.status.ly >= 240);
  print("\n");
  bool offscreen = (x < 0 || y < 0 || x >= 256 || y >= 240);
  if(offscreen || ppu.status.ly >= 240) return false;
  uint32 color = ppu.buffer[y * 256 + x];
  print(hex(color));
  print(" -> ");
  print(hex(color & 0x3F));
  print("\n");
  print("\n");
  if(color & 0x20) return true;
  return false;
}

void Zapper::latch(bool data) {
  if(latched == data) return;
  latched = data;
}

Zapper::Zapper(uint2 port) : Controller(port) {
  create(Controller::Enter, system.cpu_frequency);
  latched = 0;

  //center cursor onscreen
  x = 256 / 2;
  y = 240 / 2;

  trigger = false;

  triggerlock = false;
}

#endif
