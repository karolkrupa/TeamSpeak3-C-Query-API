#include <Server.hpp>

#ifndef _TS3OBJECTPROP_HPP
#define _TS3OBJECTPROP_HPP

using namespace std;

namespace Ts3Api {
  class Server;

  struct property {
    string name;
    string value;

    property(string name = "unknown", string value = "unknown");
  };

  struct ts3ObjectProperties {
  public:
    map<string, property> propsBuff;
    string data;
    string command;
    time_t updatedTime = time(NULL);
    time_t &updateTime;
    Server &server;
    bool incompleteInit = false;

    property getProperty(string name);

    ts3ObjectProperties(Server &server, time_t &updateTime, bool incompleteInit = false);

  protected:
    bool inBuff(string name);

    void clearBuff();

    virtual void update()=0;
  };
}
#endif
