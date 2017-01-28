#pragma once

#include <Server.hpp>
#include <Client.hpp>
#include <Group.hpp>
#include <Permission.hpp>

#include <Server.hpp>
#include <Client.hpp>
#include <Permission.hpp>
#include <structs/ts3ObjectProperties.hpp>

using namespace std;


namespace Ts3Api {
  struct ts3Response;


  /**
   * @brief      Class for channel.
   */
  class Channel {
    friend class Client;
    friend class Server;
    friend class Permission;

    /**
     * @brief      Contain channelInfo data
     */
    struct channelInfoProperties : public ts3ObjectProperties {
      string& id;

      channelInfoProperties(string &id, Server& server, time_t& updateTime, bool incomleteInit);

      /**
       * @brief      Forces to updata data with next get Action
       */
      void update();
    };

  public:

    /**
     * @brief      Providing possibility to change selected param
     */
    struct channelChangeableParam {
      friend class Server;
    private:
      string name;
      Channel *channel;
    public:
      string value;

      channelChangeableParam(Channel *channel = NULL, string name = "", string value = "");

      /**
       * @brief      Changing slected Param
       *
       * @param[in]  value  New value of the param
       *
       * @return     Data returned from the server
       * 
       * @see ts3Response
       */
      ts3Response change(string value);
    };


    /**
     * @brief      Contain channel names
     */
    struct channelName {
      channelChangeableParam name = channelChangeableParam(NULL, "channel_name");
      channelChangeableParam phonetic = channelChangeableParam(NULL, "channel_name_phonetic");
    };


    /**
     * @brief      Contain channel codec settings
     */
    struct channelCodec {
      channelChangeableParam codec = channelChangeableParam(NULL, "channel_codec");
      channelChangeableParam codec_quality = channelChangeableParam(NULL, "channel_codec_quality");
      channelChangeableParam codec_latency_factor = channelChangeableParam(NULL, "channel_codec_latency_factor");
      channelChangeableParam codec_is_unencrypted = channelChangeableParam(NULL, "channel_codec_is_unencrypted");
    };

    /**
     * @brief      Contain channel flags
     */
    struct channelFlags {
      channelChangeableParam flag_permanent = channelChangeableParam(NULL, "channel_flag_permanent");
      channelChangeableParam flag_semi_permanent = channelChangeableParam(NULL, "channel_flag_semi_permanent");
      channelChangeableParam flag_default = channelChangeableParam(NULL, "channel_flag_default");
      channelChangeableParam flag_password = channelChangeableParam(NULL, "channel_flag_password");
      channelChangeableParam flag_maxclients_unlimited = channelChangeableParam(NULL, "channel_flag_maxclients_unlimited");
      channelChangeableParam flag_maxfamilyclients_unlimited = channelChangeableParam(NULL, "channel_flag_maxfamilyclients_unlimited");
      channelChangeableParam flag_maxfamilyclients_inherited = channelChangeableParam(NULL, "channel_flag_maxfamilyclients_inherited");
      channelChangeableParam flag_private = channelChangeableParam(NULL, "channel_flag_private");
    };

  private:
    /// Connected server object
    Server &server;
    /// Channel id
    string id;
    /// Forces to update data in Info structures
    time_t updateTime = time(NULL);
    /// ChannelInfo data
    channelInfoProperties channelInfo;

  public:
    Channel(Server &server, string id = "unknown", bool incomleteInit = false);

    /**
     * @brief      Check whether object is well build
     *
     * @return     true if OK
     */
    bool good();

    /**
    * @brief      Forces to updata data with next get Action
    */
    void update();

    /**
     * @brief      Gets the channel parent ID
     *
     * @return     Channel PID with possibility to change it
     */
    channelChangeableParam getPid();

    /**
     * @brief      Gets the channel name
     *
     * @return     Channel names with possibility to change them
     */
    channelName getName();

    /**
     * @brief      Gets the channel topic
     *
     * @return     Channel topic with possibility to change it
     */
    channelChangeableParam getTopic();

    /**
     * @brief      Gets the channel description
     *
     * @return     Channel description with possibility to change it
     */
    channelChangeableParam getDescription();

    /**
     * @brief      Gets the channel password.
     *
     * @return     Channel password with possibility to change it
     */
    channelChangeableParam getPassword();

    /**
     * @brief      Gets the channel codec settings
     *
     * @return     Channel codec settings with possibility to change them
     */
    channelCodec getCodec();

    /**
     * @brief      Gets the maximum channel clients
     *
     * @return     Channel client limit with possibility to change it
     */
    channelChangeableParam getMaxClients();

    /**
     * @brief      Gets the channem maximum family clients.
     *
     * @return     Channel family client limit with possibility to change it
     */
    channelChangeableParam getMaxFamilyClients();

    /**
     * @brief      Gets the channel order ID
     *
     * @return     Channel order id with possibility to change it
     */
    channelChangeableParam getOrder();

    /**
     * @brief      Gets the channel flagsflags.
     *
     * @return     Channel flags with possibility to change it
     */
    channelFlags getFlags();

    /**
     * @brief      Gets the channel secutiry salt
     *
     * @return     Channel security salt
     */
    property getSecuritySalt();

    /**
     * @brief      Gets the channel delete delay.
     *
     * @return     Channel delete delay
     */
    property getDeleteDelay();

    /**
     * @brief      Gets the channel filepath.
     *
     * @return     Channel filepath.
     */
    property getFilepath();

    /**
     * @brief      Gets the channel need talk power.
     *
     * @return     Channel need talk power with possibility to change it
     */
    channelChangeableParam getNeedTalkPower();

    /**
     * @brief      Gets the channel forced silence.
     *
     * @return     Channel forced silence
     */
    property getForcedSilence();

    /**
     * @brief      Gets the icon identifier.
     *
     * @return     Icon identifier
     */
    channelChangeableParam getIconId();

    /**
     * @brief      Gets the channel seconds empty.
     *
     * @return     Channel seconds emptydoxywiz
     */
    property getSecondsEmpty();

    /**
     * @brief      Gets the permission.
     *
     * @param[in]  permsid  The permsid
     *
     * @return     The permission.
     */
    Permission getPermission(string permsid);

    /**
     * @brief      Gets the permissions
     *
     * @return     The permission.
     */
    map<string, Permission> getPermissionList();

    /**
     * @brief      Gets the client permission.
     *
     * @param[in]  clientDbid  The client dbid
     * @param[in]  permsid     The permsid
     *
     * @return     The client permission.
     */
    Permission getClientPermission(string clientDbid, string permsid);

    /**
     * @brief      Gets the client permission.
     *
     * @param      client   The client
     * @param[in]  permsid  The permsid
     *
     * @return     The client permission.
     */
    Permission getClientPermission(Client& client, string permsid);

    /**
     * @brief      Gets the client permission list.
     *
     * @param[in]  clientDbid  The client dbid
     * @param[in]  permsid     The permsid
     *
     * @return     The client permission list.
     */
    map<string, Permission> getClientPermissionList(string clientDbid, string permsid);

    /**
     * @brief      Gets the client permission list.
     *
     * @param      client   The client
     * @param[in]  permsid  The permsid
     *
     * @return     The client permission list.
     */
    map<string, Permission> getClientPermissionList(Client& client, string permsid);
  };
}
