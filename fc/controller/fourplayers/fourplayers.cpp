#ifdef CONTROLLER_CPP

uint2 FourPlayers::data1() {
  if(latched) return interface->inputPoll(port, (unsigned)Input::Device::FourPlayers, 0) << 1;
  uint5 data = 0;

  if(counter >= 8) data |= 2;
  else data |= interface->inputPoll(port, (unsigned)Input::Device::FourPlayers, counter) << 1;
  counter++;

  return data;
}

uint5 FourPlayers::data2() {
  if(latched) return interface->inputPoll(port, (unsigned)Input::Device::FourPlayers, 8) << 1;
  uint5 data = 0;

  if(counter >= 8) data |= 2;
  else data |= interface->inputPoll(port, (unsigned)Input::Device::FourPlayers, counter + 8) << 1;
  counter++;

  return data;
}

void FourPlayers::latch(bool data) {
  if(latched == data) return;
  latched = data;
  counter = 0;
}

FourPlayers::FourPlayers(uint2 port) : Controller(port) {
  latched = 0;
  counter = 0;
}

#endif
