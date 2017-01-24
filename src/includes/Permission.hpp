#include <Server.hpp>
#include <Client.hpp>
#include <Channel.hpp>
#include <Group.hpp>

#ifndef _PERMISSION_HPP
#define _PERMISSION_HPP

using namespace std;

namespace Ts3Api {
  class Permission {
    friend class Group;
  public:
    enum PermissionGroupTypes {
      PermGroupTypeServerGroup = 0, // 0: server group permission
      PermGroupTypeGlobalClient, // 1: client specific permission
      PermGroupTypeChannel, // 2: channel specific permission
      PermGroupTypeChannelGroup, // 3: channel group permission
      PermGroupTypeChannelClient // 4: channel-client specific permission
    };
  private:
    Server &server;
    Client *client = NULL;
    Channel *channel = NULL;
    Group *group = NULL;
    string name;
    bool negated = false;
    bool skip = false;
    PermissionGroupTypes type;
    bool checked = false;

  public:
    string value;

    Permission(Client &client, string permName, string permValue = "unknown");
    Permission(Channel &channel, string permName, string permValue = "unknown");
    Permission(Group &group, PermissionGroupTypes permType, string permName, string permValue = "unknown");
    Permission(Channel &channel, Client &client, string permName, string permValue = "unknown");

    string getValue();

    ts3Response updateValue();

    ts3Response newValue(string value, bool permSkip = false, bool permNegated = false);

    ts3Response remove();
  };
}
#endif
