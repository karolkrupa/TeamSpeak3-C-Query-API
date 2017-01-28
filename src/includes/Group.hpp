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

    /**
     * @brief      Group constructor
     *
     * @param      server         The server
     * @param[in]  id             The identifier
     * @param[in]  groupType      The group type
     * @param[in]  incomleteInit  The incomlete initialize
     */
    Group(Server &server, string id = "unknown", ts3GroupType groupType = SERVER, bool incomleteInit = false);

    /**
     * @brief      Forces to updata data with next get Action
     */
    void update();

    /**
     * @brief      Checks whether object is well constructed
     *
     * @return     true if is OK
     */
    bool good();

    /**
     * @brief      Adds a client to the group
     *
     * @param[in]  clid  The clid
     *
     * @return     Data returned from server
     */
    ts3Response addClient(string clid);

    /**
     * @brief      Adds a client to the group
     *
     * @param[in]  dbid  The dbid
     * @param[in]  cid   The clid
     *
     * @return     Data returned from server
     */
    ts3Response addClient(string dbid, string cid);

    /**
     * @brief      Adds a client to the group
     *
     * @param[in]  client  The clid
     *
     * @return     Data returned from server
     */
    ts3Response addClient(const Client& client);

    /**
     * @brief      Removes client from the group
     *
     * @param[in]  clid  The clid
     *
     * @return     Data returned from server
     */
    ts3Response deleteClient(string clid);

    /**
     * @brief      Removes client from the group
     *
     * @param[in]  client  The clid
     *
     * @return     Data returned from server
     */
    ts3Response deleteClient(const Client& client);

    /**
     * @brief      Adds a permission.
     *
     * @param[in]  name     The name
     * @param[in]  value    The value
     * @param[in]  negated  The negated
     * @param[in]  skip     The skip
     *
     * @return     Data returned from server
     */
    ts3Response addPermission(string name, string value, bool negated = false, bool skip = false);

    /**
     * @brief      Adds a permission.
     *
     * @param[in]  permission  The permission
     *
     * @return     Data returned from server
     */
    ts3Response addPermission(const Permission& permission);

    /**
     * @brief      Delete selected permission
     *
     * @param[in]  name  The name
     *
     * @return     Data returned from server
     */
    ts3Response deletePermission(string name);

    /**
     * @brief      Delete slected permission
     *
     * @param[in]  permission  The permission
     *
     * @return     Data returned from server
     */
    ts3Response deletePermission(const Permission& permission);

    /**
     * @brief      Gets the client list.
     *
     * @return     The client list.
     */
    vector<Client> getClientList();

    /**
     * @brief      Copies the group
     *
     * @param[in]  tgid  The tgid
     * @param[in]  name  The name
     * @param[in]  type  The type
     *
     * @return     Data returned from server
     */
    ts3Response groupCopy(string tgid, string name, string type);

    /**
     * @brief      Deletes the group
     *
     * @param[in]  force  The force
     *
     * @return     Data returned from server
     */
    ts3Response groupDelete(bool force = false);

    /**
     * @brief      Renames the group
     *
     * @param[in]  name  The name
     *
     * @return     Data returned from server
     */
    ts3Response groupRename(string name);

    /**
     * @brief      Gets the permission.
     *
     * @param[in]  permsid  The permsid
     *
     * @return     The permission.
     */
    Permission getPermission(string permsid);

    /**
     * @brief      Gets the permission list.
     *
     * @return     The permission list.
     */
    map<string, Permission> getPermissionList();
  };
}
