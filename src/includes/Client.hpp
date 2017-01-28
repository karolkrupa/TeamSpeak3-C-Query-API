#pragma once

#include <Server.hpp>
#include <Group.hpp>
#include <Channel.hpp>
#include <Permission.hpp>
#include <string>
#include <map>

#include <structs/ts3ObjectProperties.hpp>

using namespace std;

/** @biref Zawiera wszystkie elementy API
*/
namespace Ts3Api {
  struct ts3Response;
  //class Permission;


  
  /**
   * @brief      Contain time in timestamp format
   */
  struct timestampTime {
    string value;

    /**
     * @brief      Return date in seleceted format
     *
     * @param[in]  pattern  The pattern
     *
     * @return     For example: 2016-01-12 23.12.54
     */
    string toDate(string pattern = "%Y-%m-%d %H.%M.%S");

    timestampTime(string time);
  };

  /**
   * @brief      Class for client.
   */
  class Client {
    friend class Permission;
    friend class Group;

  public:
    /**
     * @brief      Contain all user IDs
     */
    struct IDs {
      string dbid   = "unknown";
      string clid   = "unknown";
      string uid    = "unknown";

      IDs(string clid ="unknown", string uid ="unknown", string dbid = "unknown");
    };

    /**
     * @brief      Contain clientInfo data
     */
    struct clientInfoProperties : public ts3ObjectProperties {
      IDs &ids;

      /**
       * @brief      Forces to updata data with next get Action
       */
      void update();

      clientInfoProperties(Client::IDs &ids,  Server &server, time_t &updateTime, bool clientListInit);
    };

    /**
     * @brief      Contain user names
     */
    struct nickname {
      string value;
      string phonetic;

      nickname(string nickname, string phonetic);
    };

    /**
     * @brief      Providing possibility to change selected param
     */
    struct changeableParam {
    private:
      Client &client;
    public:
      /// Parameter name
      string paramName;
      /// Prameter value
      string value;

      changeableParam(Client &client, string value, string paramName);

      /**
       * @brief      Changes selected param
       *
       * @param[in]  value  New param value
       *
       * @return     Received data from server
       */
      ts3Response change(string value);
    };

    /// Contain user transfer info
    struct transferInfo {
      string month_bytes_uploaded;
      string month_bytes_downloaded;
      string total_bytes_uploaded;
      string total_bytes_downloaded;
    };

    /// Contain user connection info
    struct connectionInfo {
      string filetransfer_bandwidth_sent;
      string filetransfer_bandwidth_received;
      string packets_sent_total;
      string bytes_sent_total;
      string packets_received_total;
      string bytes_received_total;
      string bandwidth_sent_last_second_total;
      string bandwidth_sent_last_minute_total;
      string bandwidth_received_last_second_total;
      string bandwidth_received_last_minute_total;
      string connected_time;
      string connection_client_ip;
    };

  private:
    Server &server;
    IDs clientIDs;
    time_t updateTime = time(NULL);

    clientInfoProperties clientInfo;

    string getDbid();
    string getUid();

  public:
    
    /**
     * @brief      Creates new User object
     *
     * @param      server          Reference to Server object
     * @param[in]  clientIDs       Struct containing although one ID
     * @param[in]  clientListInit  Set true if client is creating with incomplete clientInfo property
     */
    Client(Server& server, IDs clientIDs, bool clientListInit = false);

    /**
     * @brief      Forces to updata data with next get Action
     */
    void update();

    /**
     * @brief      Checks whether user is succesful created
     *
     * @return     true if client is OK
     */
    bool good();

    /**
     * @brief      Gets the client ID
     *
     * @return     Client ID
     */
    property getCLID();

    /**
     * @brief      Gets the Client Unique Identifier
     *
     * @return     Client UID
     */
    property getUID();

    /**
     * @brief      Gets the Client Database ID
     *
     * @return     Client DBID
     */
    property getDBID();

    /**
     * @brief      Gets the client idle time.
     *
     * @return     Client idle time
     */
    property getIdleTime();

    /**
     * @brief      Gets the nickname.
     *
     * @return     The nickname.
     */
    nickname getNickname();

    /**
     * @brief      Gets the version.
     *
     * @return     The version.
     */
    property getVersion();

    /**
     * @brief      Gets the version sign.
     *
     * @return     The version sign.
     */
    property getVersionSign();

    /**
     * @brief      Gets the platform.
     *
     * @return     The platform.
     */
    property getPlatform();

    /**
     * @brief      Gets the input muted.
     *
     * @return     The input muted.
     */
    bool getInputMuted();

    /**
     * @brief      Gets the output muted.
     *
     * @return     The output muted.
     */
    bool getOutputMuted();

    /**
     * @brief      Gets the output only muted.
     *
     * @return     The output only muted.
     */
    bool getOutputOnlyMuted();

    /**
     * @brief      Gets the input hardware.
     *
     * @return     The input hardware.
     */
    bool getInputHardware();

    /**
     * @brief      Gets the output hardware.
     *
     * @return     The output hardware.
     */
    bool getOutputHardware();

    /**
     * @brief      Gets the default channel.
     *
     * @return     The default channel.
     */
    property getDefaultChannel();

    /**
     * @brief      Gets the meta data.
     *
     * @return     The meta data.
     */
    property getMetaData();

    /**
     * @brief      Gets the recording status.
     *
     * @return     The recording status.
     */
    bool getRecordingStatus();

    /**
     * @brief      Gets the security hash.
     *
     * @return     The security hash.
     */
    property getSecurityHash();

    /**
     * @brief      Gets the login name.
     *
     * @return     The login name.
     */
    property getLoginName();

    /**
     * @brief      Gets the channel group.
     *
     * @return     The channel group.
     */
    Group getChannelGroup();

    /**
     * @brief      Gets the channel group id.
     *
     * @return     The channel group id.
     */
    property getChannelGroupID();

    /**
     * @brief      Gets the server groups.
     *
     * @return     The server groups.
     */
    vector<Group> getServerGroups();

    /**
     * @brief      Gets the server groups list.
     *
     * @return     The server groups list.
     */
    vector<string> getServerGroupsList();

    /**
     * @brief      Gets the created time.
     *
     * @return     The created time.
     */
    timestampTime getCreatedTime();

    /**
     * @brief      Gets the lastconnected time.
     *
     * @return     The lastconnected time.
     */
    timestampTime getLastconnectedTime();

    /**
     * @brief      Gets the total connections.
     *
     * @return     The total connections.
     */
    property getTotalConnections();

    /**
     * @brief      Gets the away status.
     *
     * @return     The away status.
     */
    bool getAwayStatus();

    /**
     * @brief      Gets the away message.
     *
     * @return     The away message.
     */
    property getAwayMessage();

    /**
     * @brief      Gets the type.
     *
     * @return     The type.
     */
    property getType();

    /**
     * @brief      Gets the avatar flag.
     *
     * @return     The avatar flag.
     */
    property getAvatarFlag();

    /**
     * @brief      Gets the talk power.
     *
     * @return     The talk power.
     */
    Permission getTalkPower();

    /**
     * @brief      Gets the talk request.
     *
     * @return     The talk request.
     */
    property getTalkRequest();

    /**
     * @brief      Gets the talk request message.
     *
     * @return     The talk request message.
     */
    property getTalkRequestMsg();

    /**
     * @brief      Gets the description.
     *
     * @return     The description.
     */
    changeableParam getDescription();

    /**
     * @brief      Gets the talker status.
     *
     * @return     The talker status.
     */
    changeableParam getTalkerStatus();

    /**
     * @brief      Gets the transfer information.
     *
     * @return     The transfer information.
     */
    transferInfo getTransferInfo();

    /**
     * @brief      Gets the priority speaker status.
     *
     * @return     The priority speaker status.
     */
    bool getPrioritySpeakerStatus();

    /**
     * @brief      Gets the needed serverquery view power.
     *
     * @return     The needed serverquery view power.
     */
    Permission getNeededServerqueryViewPower();

    /**
     * @brief      Gets the default token.
     *
     * @return     The default token.
     */
    property getDefaultToken();

    /**
     * @brief      Gets the icon identifier.
     *
     * @return     The icon identifier.
     */
    changeableParam getIconId();

    /**
     * @brief      Gets the channel commander status.
     *
     * @return     The channel commander status.
     */
    changeableParam getChannelCommanderStatus();

    /**
     * @brief      Gets the country.
     *
     * @return     The country.
     */
    property getCountry();

    /**
     * @brief      Gets the channel group inherit channel id.
     *
     * @return     The channel group inherit channel id.
     */
    property getChannelGroupInheritChannelID();

    /**
     * @brief      Gets the badges.
     *
     * @return     The badges.
     */
    property getBadges();

    /**
     * @brief      Gets the base 64 hash client uid.
     *
     * @return     The base 64 hash client uid.
     */
    property getBase64HashClientUID();

    /**
     * @brief      Gets the connection information.
     *
     * @return     The connection information.
     */
    connectionInfo getConnectionInfo();

    /**
     * @brief      Adds a group.
     *
     * @param[in]  groupId  The group identifier
     *
     * @return     Data returned from server
     */
    ts3Response addGroup(string groupId);

    /**
     * @brief      Adds a group.
     *
     * @param[in]  group  The group
     *
     * @return     Data returned from server
     */
    ts3Response addGroup(const Group &group);

    /**
     * @brief      Removes a group.
     *
     * @param[in]  groupId  The group identifier
     *
     * @return     Data returned from server
     */
    ts3Response removeGroup(string groupId);

    /**
     * @brief      Removes a group.
     *
     * @param[in]  group  The group
     *
     * @return     Data returned from server
     */
    ts3Response removeGroup(const Group &group);

    /**
     * @brief      Adds a channel group.
     *
     * @param[in]  groupId    The group identifier
     * @param[in]  channelId  The channel identifier
     *
     * @return     Data returned from server
     */
    ts3Response addChannelGroup(string groupId, string channelId);

    /**
     * @brief      Adds a channel group.
     *
     * @param[in]  groupId  The group identifier
     * @param[in]  channel  The channel
     *
     * @return     Data returned from server
     */
    ts3Response addChannelGroup(string groupId, const Channel &channel);

    /**
     * @brief      Adds a channel group.
     *
     * @param[in]  group    The group
     * @param[in]  channel  The channel
     *
     * @return     Data returned from server
     */
    ts3Response addChannelGroup(const Group &group , const Channel &channel);

    /**
     * @brief      Adds a channel group.
     *
     * @param[in]  group      The group
     * @param[in]  channelId  The channel identifier
     *
     * @return     Data returned from server
     */
    ts3Response addChannelGroup(const Group &group , string channelId);

    /**
     * @brief      Gets the permission.
     *
     * @param[in]  permName  The permission name
     *
     * @return     The permission.
     */
    Permission getPermission(string permName);

    /**
     * @brief      Deletes server database
     *
     * @return     Data returned from server
     */
    ts3Response dbDelete();

    /**
     * @brief      Sets the serverquery login.
     *
     * @param[in]  loginName  The login name
     * @param      password   The password
     *
     * @return     Data returned from server
     */
    ts3Response setServerqueryLogin(string loginName, string &password);

    /**
     * @brief      Moves user to another channel
     *
     * @param[in]  cid       The cid
     * @param[in]  password  The password
     *
     * @return     Data returned from server
     */
    ts3Response move(string cid, string password = "");

    /**
     * @brief      Kicks user from channel
     *
     * @param[in]  reason  The reason
     *
     * @return     Data returned from server
     */
    ts3Response kickFromChannel(string reason);

    /**
     * @brief      Kicks user from server
     *
     * @param[in]  reason  The reason
     *
     * @return     Data returned from server
     */
    ts3Response kickFromServer(string reason);

    /**
     * @brief      Poke user
     *
     * @param[in]  message  The message
     *
     * @return     Data returned from server
     */
    ts3Response poke(string message);

    /**
     * @brief      Complains the user
     *
     * @param[in]  message  The message
     *
     * @return     Data returned from server
     */
    ts3Response complain(string message);

    /**
     * @brief      Deletes complain
     *
     * @param[in]  accuserDbid  The accuser dbid
     *
     * @return     Data returned from server
     */
    ts3Response delComplain(string accuserDbid);

    /**
     * @brief      Deletes all complains
     *
     * @return     Data returned from server
     */
    ts3Response delAllComplains();

    /**
     * @brief      Bans user
     *
     * @param[in]  banTime  The ban time
     * @param[in]  reason   The reason
     *
     * @return     Data returned from server
     */
    ts3Response ban(string banTime = "", string reason ="");

    /**
     * @brief      Sends a message.
     *
     * @param[in]  message  The message
     *
     * @return     Data returned from server
     */
    ts3Response sendMessage(string message);
  };
}
