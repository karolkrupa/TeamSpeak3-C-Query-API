#include <Server.hpp>
#include <Client.hpp>
#include <Channel.hpp>
#include <Group.hpp>

#ifndef _PERMISSION_HPP
#define _PERMISSION_HPP

using namespace std;

namespace Ts3Api {

  /**
   * @brief      Class for permission.
   */
  class Permission {
    friend class Group;
    friend class Channel;
  public:
    enum PermissionGroupTypes {
      /// Server group premission
      PermGroupTypeServerGroup = 0,
      /// Client specific permission
      PermGroupTypeGlobalClient,
      /// Channel specific permission
      PermGroupTypeChannel,
      /// Channel group permission
      PermGroupTypeChannelGroup,
      /// Channel-client permission
      PermGroupTypeChannelClient
    };
  private:
    Server &server;
    Client *client = NULL;
    Channel *channel = NULL;
    Group *group = NULL;
    string name;
    PermissionGroupTypes type;
    bool checked = false;

  public:
  	/// Permission value
    string value;
    /// Permnegated value
    bool negated = false;
    /// Permskip value
    bool skip = false;

    /**
     * @brief      Constructor for client permission
     *
     * @param      client     The client
     * @param[in]  permName   The permission name
     * @param[in]  permValue  The permission value
     */
    Permission(Client &client, string permName, string permValue = "unknown", bool negated = false, bool skip = false);

    /**
     * @brief      Constructor for channel permission
     *
     * @param      channel    The channel
     * @param[in]  permName   The permission name
     * @param[in]  permValue  The permission value
     */
    Permission(Channel &channel, string permName, string permValue = "unknown", bool negated = false, bool skip = false);

    /**
     * @brief      Constructor for channel group or server group permission
     *
     * @param      group      The group
     * @param[in]  permType   The permission type
     * @param[in]  permName   The permission name
     * @param[in]  permValue  The permission value
     */
    Permission(Group &group, PermissionGroupTypes permType, string permName, string permValue = "unknown", bool negated = false, bool skip = false);

    /**
     * @brief      Constructor for channel-client permission
     *
     * @param      channel    The channel
     * @param      client     The client
     * @param[in]  permName   The permission name
     * @param[in]  permValue  The permission value
     */
    Permission(Channel &channel, Client &client, string permName, string permValue = "unknown", bool negated = false, bool skip = false);

    /**
     * @brief      Gets the value selected permission
     *
     * @return     The value.
     */
    string getValue();

    /**
     * @brief      Update the containg permission value
     *
     * @return     Data returned from server
     */
    ts3Response updateValue();

    /**
     * @brief      Sets new value of selected permission
     *
     * @param[in]  value        The value
     * @param[in]  permSkip     The permission skip
     * @param[in]  permNegated  The permission negated
     *
     * @return     Data returned from server
     */
    ts3Response newValue(string value, bool permSkip = false, bool permNegated = false);

    /**
     * @brief      Removes the selected permission
     *
     * @return     Data returned from server
     */
    ts3Response remove();
  };
}
#endif
