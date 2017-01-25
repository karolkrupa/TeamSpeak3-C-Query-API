#pragma once

#include <Server.hpp>
#include <Client.hpp>
#include <Permission.hpp>
#include <structs/ts3ObjectProperties.hpp>
#include <vector>

using namespace std;

namespace Ts3Api {
  class Group {
    friend class Client;
    friend class Server;
    friend class Permission;

    struct groupInfoProperties : public ts3ObjectProperties {
      string& id;

      groupInfoProperties(string &id, Server& server, time_t& updateTime, bool incomleteInit);

      void update();
    };

  public:
    enum ts3GroupType {
      SERVER = 1,
      CHANNEL
    };

  private:
    Server &server;
    string id;
    time_t updateTime = time(NULL);
    groupInfoProperties groupInfo;
    ts3GroupType groupType;

    vector<Permission> splitPerm(string input);
    map<string, Client> splitClient(string input);

  public:
    Group(Server &server, string id = "unknown", ts3GroupType groupType = SERVER, bool incomleteInit = false);

    void update();

    bool good();

    ts3Response addClient(string clid);
    ts3Response addClient(string dbid, string cid);
    ts3Response addClient(const Client& client);

    ts3Response deleteClient(string clid);
    ts3Response deleteClient(const Client& client);

    ts3Response addPermission(string name, string value, bool negated = false, bool skip = false);
    ts3Response addPermission(const Permission& permission);

    ts3Response deletePermission(string name);
    ts3Response deletePermission(const Permission& permission);

    map<string, Permission> getPermissionList();

    vector<Client> getClientList();

    ts3Response groupCopy(string tgid, string name, string type);
    ts3Response groupDelete(bool force = false);
    ts3Response groupRename(string name);
  };
}
