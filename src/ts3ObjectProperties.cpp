#include <structs/ts3ObjectProperties.hpp>

using namespace Ts3Api;

property::property(string name, string value) {
  this->name = name;
  this->value = value;
}

ts3ObjectProperties::ts3ObjectProperties(Server &server, time_t &updateTime, bool clientListInit) :
  updateTime(updateTime),
  server(server)
{
  this->incompleteInit = incompleteInit;
}

bool ts3ObjectProperties::inBuff(string name) {
  return propsBuff.find(name) != propsBuff.end();
}

property ts3ObjectProperties::getProperty(string name) {
  size_t pos;
  string value;

  if(updateTime > updatedTime) update();

  if(inBuff(name)) return propsBuff[name];

  if(data[data.length()-1] != ' ') data += " ";

  if(incompleteInit) {
    if((pos = data.find(name+"=")) == string::npos) {
      update();
      if((pos = data.find(name+"=")) == string::npos) return property("unknown", "unknown");
    }
  }else {
    if((pos = data.find(name+"=")) == string::npos) return property("unknown", "unknown");
  }

  pos += name.length()+1;

  value = data.substr(pos, data.find(" ", pos)-pos);
  propsBuff[name] = property(name, value);
  return property(name, value);
}

void ts3ObjectProperties::clearBuff() {
  propsBuff.clear();
}
