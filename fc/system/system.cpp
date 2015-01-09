#include <fc/fc.hpp>

#define SYSTEM_CPP
namespace Famicom {

System system;
Configuration configuration;

#include "video.cpp"
#include "input.cpp"
#include "serialization.cpp"

#include <fc/scheduler/scheduler.cpp>

void System::run() {
  scheduler.sync = Scheduler::SynchronizeMode::None;

  scheduler.enter();
  if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
    video.update();
  }
}

void System::runtosave() {
  scheduler.sync = Scheduler::SynchronizeMode::PPU;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = cpu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = apu.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::All;
  scheduler.thread = cartridge.thread;
  runthreadtosave();

  scheduler.sync = Scheduler::SynchronizeMode::None;
}

void System::runthreadtosave() {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason() == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
      video.update();
    }
  }
}

void System::init() {
  assert(interface != nullptr);
  input.connect(0, configuration.controller_port1);
  input.connect(1, configuration.controller_port1);
  input.connect(2, configuration.expansion_port);
}

void System::term() {
}

void System::load(Revision revision) {
  this->revision = revision;
  string manifest = string::read({interface->path(ID::System), "manifest.bml"});
  auto document = Markup::Document(manifest);

  region = Region::Autodetect;
  if(region == Region::Autodetect) {
    region = (cartridge.region == Cartridge::Region::NTSC ? Region::NTSC : Region::PAL);
  }

  cpu_frequency = region == Region::NTSC ? 21477272 : 26601712;

  switch(revision) {
  case Revision::Famicom:
    if(region == Region::NTSC) ppu.revision = PPU::Revision::RP2C02;
    if(region == Region::PAL)  ppu.revision = PPU::Revision::RP2C07;
    break;
  case Revision::PlayChoice10:
    ppu.revision = PPU::Revision::RP2C03;
    break;
  }

  serialize_init();
}

void System::power() {
  cartridge.power();
  cpu.power();
  apu.power();
  ppu.power();
  input.power();
  scheduler.power();
  reset();
}

void System::reset() {
  cartridge.reset();
  cpu.reset();
  apu.reset();
  ppu.reset();
  scheduler.reset();
  input.connect(0, configuration.controller_port1);
  input.connect(1, configuration.controller_port2);
}

void System::scanline() {
  video.scanline();
  if(ppu.status.ly == 241) scheduler.exit(Scheduler::ExitReason::FrameEvent);
}

void System::frame() {
}

System::System() {
  region = Region::Autodetect;
}

}
