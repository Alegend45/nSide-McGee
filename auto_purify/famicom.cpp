void AutoPurify::copyFamicomSaves(const string &pathname) {
  if(!file::exists({pathname, "save.ram"})) {
    if(file::exists({information.path, nall::basename(information.name), ".sav"})) {
      file::copy({information.path, nall::basename(information.name), ".sav"}, {pathname, "save.ram"});
    }
  }
}

string AutoPurify::createFamicomDatabase(vector<uint8_t> &buffer, Markup::Node &document, const string &manifest) {
  string pathname = {
    libraryPath, "Famicom/",
    document["release/information/name"].text(),
    " (", document["release/information/region"].text(), ")",
    " (", document["release/information/revision"].text(), ")",
    ".fc/"
  };
  directory::create(pathname);

  //strip "release" root node from database entry (since a single game manifest isn't part of a database)
  string markup = manifest;
  markup.replace("\n  ", "\n");
  markup.replace("information", "\ninformation");
  markup.ltrim<1>("release\n");

  file::write({pathname, "manifest.bml"}, markup);

  unsigned offset = 0;
  for(auto &node : document["release/information/configuration"]) {
    if(node.name != "rom") continue;
    string name = node["name"].text();
    unsigned size = node["size"].decimal();
    file::write({pathname, name}, buffer.data() + offset, size);
    offset += size;
  }

  copyFamicomSaves(pathname);
  return pathname;
}

string AutoPurify::createFamicomHeuristic(vector<uint8_t> &buffer) {
  string pathname = {
    libraryPath, "Famicom/",
    nall::basename(information.name),
    ".fc/"
  };
  directory::create(pathname);

  FamicomCartridge info(buffer.data(), buffer.size());
  string markup = {"unverified\n\n", info.markup};
  markup.append("\ninformation\n  title: ", nall::basename(information.name), "\n");
  if(!information.manifest.empty()) markup = information.manifest;  //override with embedded beat manifest, if one exists

  file::write({pathname, "manifest.bml"}, markup);
  file::write({pathname, "program.rom"}, buffer.data() + 16, info.prgrom);
  if(info.chrrom > 0) file::write({pathname, "character.rom"}, buffer.data() + 16 + info.prgrom, info.chrrom);

  copyFamicomSaves(pathname);
  return pathname;
}

string AutoPurify::openFamicom(vector<uint8_t> &buffer) {
  bool ines = true;
  ines = ines && buffer.data()[0] == 'N';
  ines = ines && buffer.data()[1] == 'E';
  ines = ines && buffer.data()[2] == 'S';
  ines = ines && buffer.data()[3] == 0x1A;
  string sha256 = nall::sha256(buffer.data() + (ines ? 16 : 0), buffer.size() - (ines ? 16 : 0));

  string databaseText = string::read({configpath(), "auto_purify/database/Famicom.bml"}).strip();
  if(databaseText.empty()) databaseText = string{Database::Famicom}.strip();
  lstring databaseItem = databaseText.split("\n\n");

  for(auto &item : databaseItem) {
    item.append("\n");
    auto document = Markup::Document(item);

    if(document["release/information/sha256"].text() == sha256) {
      return createFamicomDatabase(buffer, document, item);
    }
  }

  return createFamicomHeuristic(buffer);
}

//this currently cannot work:
//game folders discard iNES header required for heuristic detection
//a games database of all commercial Famicom software will be required
string AutoPurify::syncFamicom(const string &pathname) {
  vector<uint8_t> buffer;

  auto append = [&](string filename) {
    filename = {pathname, filename};
    auto data = file::read(filename);
    if(data.size() == 0) return;  //file does not exist

    unsigned position = buffer.size();
    buffer.resize(buffer.size() + data.size());
    memcpy(buffer.data() + position, data.data(), data.size());
  };

  append("program.rom");
  append("character.rom");

  if(buffer.size() == 0) return "";

  auto save = file::read({pathname, "save.ram"});
  if(save.size() == 0) save = file::read({pathname, "save.rwm"});

  directory::remove(pathname);
  information.path = pathname;
  information.name = notdir(string{pathname}.rtrim<1>("/"));
  string outputPath = openFamicom(buffer);

  if(save.size()) file::write({outputPath, "save.ram"}, save);

  return outputPath;
}
