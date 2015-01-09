#ifdef SYSTEM_CPP

Input input;

void Input::latch(bool data) {
  latchdata = data;
  //port1->latch(data);
  //port2->latch(data);

  if(latchdata == 1) {
    counters[0] = 0;
    counters[1] = 0;
  }
}

uint5 Input::data(bool port) {
  Controller*& controller = port1;
  switch(port) {
  case Controller::Port1: controller = port1;
  case Controller::Port2: controller = port2;
  }
  uint5 result = 0;
  unsigned device_id = (unsigned)ports[port];
  unsigned expansion_id = (unsigned)ports[2];
  int nx;
  int ny;

  if(ports[port] != Device::None) {
    switch(ports[port]) {
    case Device::Joypad:
      if(counters[port] >= 8) result |= 1;
      else result |= interface->inputPoll(port, device_id, counters[port]);
      break;
    case Device::FourScore:
      if(counters[port] >= 24) result |= 1;
      if(counters[port] >= 16) result |= counters[port] == (19 - port);
      else result |= interface->inputPoll(port, device_id, counters[port]);
      break;
    case Device::Zapper:
      result |= 0x08; // No light detected
      result |= interface->inputPoll(port, device_id, (unsigned)Input::ZapperID::Trigger) << 4u;
      break;
    case Device::PowerPad:
      unsigned input08;
      unsigned input10;
      switch(counters[port]) {
      case 0: input08 =  2; input10 =  4; break;
      case 1: input08 =  1; input10 =  3; break;
      case 2: input08 =  5; input10 = 12; break;
      case 3: input08 =  9; input10 =  8; break;
      case 4: input08 =  6; break;
      case 5: input08 = 10; break;
      case 6: input08 = 11; break;
      case 7: input08 =  7; break;
      }
      if(counters[port] >= 8) result |= 0x08;
      else result |= interface->inputPoll(port, device_id, input08 - 1u) << 3u;
      if(counters[port] >= 4) result |= 0x10;
      else result |= interface->inputPoll(port, device_id, input10 - 1u) << 4u;
      break;
    }
    switch(ports[2]) {
    case Device::FourPlayers:
      if(counters[port] >= 8) result |= 0x02;
      else result |= interface->inputPoll(2u, expansion_id, counters[port] + port * 8u) << 1u;
      break;
    case Device::BeamGun: // Identical to the Zapper except for the port
      if(port == 1) {
        result |= 0x08; // No light detected
        result |= interface->inputPoll(2u, expansion_id, (unsigned)Input::ZapperID::Trigger) << 4u;
      }
      break;
    }
    if(latchdata == 0) counters[port]++;
  }

  return result;
}

void Input::connect(uint2 port, Device id) {
  Controller*& controller = port1;
  switch(port) {
  case Controller::Port1: controller = port1;
  case Controller::Port2: controller = port2;
  case Controller::ExpansionPort: controller = expansion;
  }
  if(controller) {
    delete controller;
    controller = nullptr;
  }

  if(port < 2)
  switch(id) { default:
  case Device::None: controller = new Controller(port); break;
  case Device::Joypad: controller = new Gamepad(port); break;
  case Device::FourScore: controller = new FourScore(port); break;
  case Device::Zapper: controller = new Zapper(port); break;
  case Device::PowerPad: controller = new PowerPad(port); break;
  }
  else
  switch(id) { default:
  case Device::None: controller = new Controller(port); break;
  case Device::FourPlayers: controller = new FourPlayers(port); break;
  case Device::BeamGun: controller = new Zapper(port); break;
  }

  switch(port) {
  case Controller::Port1: configuration.controller_port1 = id; break;
  case Controller::Port2: configuration.controller_port2 = id; break;
  case Controller::ExpansionPort: configuration.expansion_port = id; break;
  }

  ports[port] = id;
  if(port & 0x02 == 0) counters[port] = 0;
  if(id == Device::Zapper || id == Device::BeamGun) {
    x = 256 / 2;
    y = 240 / 2;
  }
  /*if(port == 0) {
    if(id == Device::FourScore) {
      ports[1] = Device::FourScore;
      counters[1] = 0;
    } else if(ports[1] == Device::FourScore) {
      ports[1] = Device::None;
      counters[1] = 0;
    }
  } else {
    if(id == Device::FourScore) {
      ports[0] = Device::FourScore;
      counters[0] = 0;
    } else if(ports[0] == Device::FourScore) {
      ports[0] = Device::None;
      counters[0] = 0;
    }
  }*/
}

void Input::power() {
}

void Input::reset() {
  latchdata = 0;
  counters[0] = 0;
  counters[1] = 0;
}

Input::Input() {
  connect(Controller::Port1, Input::Device::Joypad);
  connect(Controller::Port2, Input::Device::Joypad);
  connect(Controller::ExpansionPort, Input::Device::FourPlayers);
}

Input::~Input() {
  if(port1) delete port1;
  if(port2) delete port2;
  if(expansion) delete expansion;
}

#endif
