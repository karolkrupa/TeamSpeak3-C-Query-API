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

  class Channel {
    friend class Client;
    friend class Server;
    friend class Permission;

    struct channelInfoProperties : public ts3ObjectProperties {
      string& id;

      channelInfoProperties(string &id, Server& server, time_t& updateTime, bool incomleteInit);

      void update();
    };

  public:
    struct channelChangeableParam {
      friend class Server;
    private:
      string name;
      Channel *channel;
    public:
      string value;

      channelChangeableParam(Channel *channel = NULL, string name = "", string value = "");

      ts3Response newValue(string value);
    };

    struct channelName {
      channelChangeableParam name = channelChangeableParam(NULL, "channel_name");
      channelChangeableParam phonetic = channelChangeableParam(NULL, "channel_name_phonetic");
    };

    struct channelCodec {
      channelChangeableParam codec = channelChangeableParam(NULL, "channel_codec");
      channelChangeableParam codec_quality = channelChangeableParam(NULL, "channel_codec_quality");
      channelChangeableParam codec_latency_factor = channelChangeableParam(NULL, "channel_codec_latency_factor");
      channelChangeableParam codec_is_unencrypted = channelChangeableParam(NULL, "channel_codec_is_unencrypted");
    };

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
    Server &server;
    string id;
    time_t updateTime = time(NULL);
    channelInfoProperties channelInfo;

  public:
    Channel(Server &server, string id, bool incomleteInit = false);

    void update();

    channelChangeableParam getPid();
    channelName getName();
    channelChangeableParam getTopic();
    channelChangeableParam getDescription();
    channelChangeableParam getPassword();
    channelCodec getCodec();
    channelChangeableParam getMaxClients();
    channelChangeableParam getMaxFamilyClients();
    channelChangeableParam getOrder();
    channelFlags getFlags();
    property getsecuritySalt();
    property getDeleteDelay();
    property getFilepath();
    channelChangeableParam getNeedTalkPower();
    property getForcedSilence();
    channelChangeableParam getIconId();
    property getSecondsEmpty();
  };
}
