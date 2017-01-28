#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <netdb.h>
#include <unistd.h>
#include <queue>

#include <cstdio>
#include <ctime>

#include <regex>

#include <mutex>
#include <thread>

#include <Client.hpp>
#include <functions.hpp>
#include <Channel.hpp>
#include <Group.hpp>
#include <structs/serverStructs.hpp>

#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <structs/ts3ObjectProperties.hpp>

using namespace std;

namespace Ts3Api {
  class Client;
  class Channel;
  class Group;

  /**
   * @brief      Server Class.
   */
  class Server {
    struct serverInfoProperties : public ts3ObjectProperties {

        serverInfoProperties(Server &server, time_t &updateTime) : ts3ObjectProperties(server, updateTime, false) {}

        void update();
    };

  public:
    struct serverChangeableParam {
    private:
        Server* server;
        string name;
    public:
        string value;

        serverChangeableParam(Server* server = NULL, string name = "", string value = "");

        ts3Response change(string value);
    };

    /**
     * @brief      Contains server names
     */
    struct serverNameProperties {
        serverChangeableParam name;
        serverChangeableParam phonetic;
    };

    /**
     * @brief      Contains clients online count
     */
    struct serverClientsOnlineProperties {
        string normal;
        string query;
    };

    /**
     * @brief      Contains server complain settings
     */
    struct serverComplainSettings {
        serverChangeableParam autoban_count;
        serverChangeableParam autoban_time;
        serverChangeableParam remove_time;
    };

    /**
     * @brief      Contains server antiflood settings
     */
    struct serverAntifolldSettings {
        serverChangeableParam points_tick_reduce;
        serverChangeableParam points_needed_command_block;
        serverChangeableParam points_needed_ip_block;
    };

    struct serverHostButtonSettings {
        serverChangeableParam tooltip;
        serverChangeableParam url;
        serverChangeableParam gfx_url;
    };

    struct serverTransferSettings {
        serverChangeableParam download_quota;
        serverChangeableParam upload_quota;
    };

    struct serverTransferProperties {
        string month_bytes_downloaded;
        string month_bytes_uploaded;
        string total_bytes_downloaded;
        string total_bytes_uploaded;
    };

    struct serverLogSettings
    {
        serverChangeableParam log_client;
        serverChangeableParam log_query;
        serverChangeableParam log_channel;
        serverChangeableParam log_permissions;
        serverChangeableParam log_server;
        serverChangeableParam log_filetransfer;
    };

    struct serverPacketLossProperties {
        string speech;
        string keepalive;
        string control;
        string total;
    };

    struct serverClientsMinimumVersion {
        serverChangeableParam android;
        serverChangeableParam ios;
        serverChangeableParam pc;
    };

    struct serverConnectionProperties {
        string filetransfer_bandwidth_sent;
        string filetransfer_bandwidth_received;
        string filetransfer_bytes_sent_total;
        string filetransfer_bytes_received_total;
        string packets_sent_speech;
        string bytes_sent_speech;
        string packets_received_speech;
        string bytes_received_speech;
        string packets_sent_keepalive;
        string bytes_sent_keepalive;
        string packets_received_keepalive;
        string bytes_received_keepalive;
        string packets_sent_control;
        string bytes_sent_control;
        string packets_received_control;
        string bytes_received_control;
        string packets_sent_total;
        string bytes_sent_total;
        string packets_received_total;
        string bytes_received_total;
        string bandwidth_sent_last_second_total;
        string bandwidth_sent_last_minute_total;
        string bandwidth_received_last_second_total;
        string bandwidth_received_last_minute_total;
    };

    enum groupDbType {
        groupTemplate = 0,
        groupRegular,
        groupQuery
    };

  private:
    /// Descriptor ID
    int sock = -1;

    /// Errors queue
    queue<error> errors;

    /// Forces to update data in serverInfo
    time_t updateTime = time(NULL);

    /// Contains informations about selected server
    serverInfoProperties serverInfo;

    /**
     * @brief Receiver var control
     *
     * When is true receiver will break own loop
     */
    bool receiverBreak = false;
    /**
     * @brief Receiver var control
     *
     * When is true receiver it's off
     */
    bool receiverDown = true;

    /// Receiver thread
    thread receiverThd;

    /// Multithread mutex
    mutex mtx;

    /// Contain last message from server received by receiver
    ts3Response receiverLastMsg;

    /// Contain ipAddress[0] and queryPort[1], serverPort[3]
    string connection[3];

    /**
     * @brief      Connects to server.
     *
     * @return     true if connected or false if not connected
     */
    bool connectToServer();

    /**
     * @brief      Checks wheter socket is connected
     *
     * @return     true if connected or false if not connected
     */
    bool socketAvailable();

    /**
     * @brief      Gets the socket data in raw from
     *
     * @return     The socket data.
     *
     * @see socketData
     */
    socketData getSocketData();

    /**
     * @brief      Gets the socket data in normalized form
     *
     * @return     The normal data.
     *
     * @see ts3Response
     */
    ts3Response getNormalData();

    /**
     * @brief      Clears socket buffer
     */
    void clearSocketBuffer();

    /**
     * @brief      Sends a data through socket
     *
     * @param[in]  data  Data to send
     *
     * @return     true if sended or false on error
     */
    bool sendData(string data);

    /**
     * @brief      Converting raw socket data to normalized form
     *
     * @param[in]  data  Raw socket data
     *
     * @return     Normal Ts3Server data
     *
     * @see ts3Response
     * @see socketData
     */
    ts3Response convertSocketDataToTs3Response(socketData data);

    /**
     * @brief      Reciver thread loop
     */
    void receiverLoop();

    /**
     * @brief      Return receiver status
     *
     * @return     true if running or false if down
     */
    bool receiverStatus();

    /**
     * @brief      Stopping receiver thread
     *
     * @return     true if receiver stopped or false
     */
    bool receiverStop();

    /**
     * @brief      Starting receiver thread
     *
     * @return     true if succesful
     */
    bool receiverStart();

    public:
    /**
     * @brief      Creating and connectiong connection with Ts3Server
     *
     * @param[in]  ip    Ts3 server address
     * @param[in]  port  Ts3 server port
     */
    Server(string ip, string port);


    /**
     * @brief      Default constructor
     */
    Server();

    /**
     * @brief      Destroys the object.
     */
    ~Server();

    /**
     * @brief      Performs selected command on Ts3 Server
     *
     * @param[in]  command  The command
     *
     * @return     Data returned from the server
     */
    ts3Response executeCommand(string command);

    /**
     * @brief      Checks connection with Ts3 Server
     *
     * @return     true if connected
     */
    bool connected();

    /**
     * @brief      Connecting to server
     *
     * @param[in]  ip    Server ip
     * @param[in]  port  The port
     *
     * @return     true if connected
     */
    bool serverConnect(string ip, string port);

    /**
     * @brief      Return last error
     *
     * @return     The error.
     */
    error getError();

    /**
     * @brief      Selects Ts3 server
     *
     * @param[in]  port  Ts3 Server port
     *
     * @return     Data returned from the server
     */
    ts3Response selectServer(string port);

    /**
     * @brief      Login to Ts3 Server
     *
     * @param[in]  login     ServerQuery user login
     * @param[in]  password  ServerQuery user password
     *
     * @return     Data returned from the server
     */
    ts3Response login(string login, string password);

    /**
     * @brief      Gets the client by clid.
     *
     * @param[in]  clid  Client ID
     *
     * @return     Client Object
     *
     * @see Client
     */
    Client getClientByClid(string clid);

    /**
     * @brief      Gets the client by dbid.
     *
     * @param[in]  clid  Client Database ID
     *
     * @return     Client Object
     *
     * @see Client
     */
    Client getClientByDbid(string dbid);

    /**
     * @brief      Gets the client by uid.
     *
     * @param[in]  uid  Client Uniquer Identifier
     *
     * @return     Client Object
     *
     * @see Client
     */
    Client getClientByUid(string uid);

    /**
     * @brief      Gets the client by nickname.
     *
     * @param[in]  nickname  Client Nickname
     *
     * @return     Client Object
     *
     * @see Client
     */
    Client getClientByNickname(string nickname);

    /**
     * @brief      Gets the server group by identifier.
     *
     * @param[in]  id    Group ID
     *
     * @return     Group Object
     *
     * @see Group
     */
    Group getServerGroupById(string id);

    /**
     * @brief      Gets the server group by name.
     *
     * @param[in]  name  Group Name
     *
     * @return     Group Object
     *
     * @see Group
     */
    Group getServerGroupByName(string name);

    /**
     * @brief      Gets the channel group by identifier.
     *
     * @param[in]  id    Group Identifier
     *
     * @return     Group Object
     *
     * @see Group
     */
    Group getChannelGroupById(string id);

    /**
     * @brief      Gets the channel group by name.
     *
     * @param[in]  name  Group Name
     *
     * @return     Group Object
     *
     * @see Group
     */
    Group getChannelGroupByName(string name);

    /**
     * @brief      Forces to updata data with next get Action
     */
    void update();

    /**
     * @brief      Gets the server list.
     *
     * @return     Server list
     */
    map<string, Server> getServerList();

    /**
     * @brief      Deleting selected server
     *
     * @param[in]  id    Server ID
     *
     * @return     Data returned from server
     */
    ts3Response serverDelete(string id = "");

    /**
     * @brief      Creating new virtual server
     *
     * @param[in]  serverProperties  The server properties
     *
     * @return     Data returned from server
     */
    ts3Response serverCreate(map<string, string> serverProperties);

    /**
     * @brief      Starting selected server
     *
     * @param[in]  id    The identifier
     *
     * @return     Data returned from server
     */
    ts3Response serverStart(string id = "");

    /**
     * @brief      Stopping selected server
     *
     * @param[in]  id    The identifier
     *
     * @return     Data returned from server
     */
    ts3Response serverStop(string id = "");

    /**
     * @brief      Forces stop server
     *
     * @return     Data returned from server
     */
    ts3Response serverProcessStop();

    /**
     * @brief      Gets the server unique idetifier
     *
     * @return     The uid.
     */
    property getUID();

    /**
     * @brief      Gets the server name
     *
     * @return     The name.
     */
    serverNameProperties getName();

    /**
     * @brief      Gets the server welcome message
     *
     * @return     The welcome message.
     */
    serverChangeableParam getWelcomeMessage();

    /**
     * @brief      Gets the server platform.
     *
     * @return     The platform.
     */
    property getPlatform();

    /**
     * @brief      Gets the server version.
     *
     * @return     The version.
     */
    property getVersion();

    /**
     * @brief      Gets the server clients limit.
     *
     * @return     The maximum clients.
     */
    serverChangeableParam getMaxClients();

    /**
     * @brief      Gets the server password.
     *
     * @return     The password.
     */
    serverChangeableParam getPassword();

    /**
     * @brief      Gets the clients online.
     *
     * @return     The clients online.
     */
    serverClientsOnlineProperties getClientsOnline();

    /**
     * @brief      Gets the channels online.
     *
     * @return     The channels online.
     */
    property getChannelsOnline();

    /**
     * @brief      Gets the server created time.
     *
     * @return     The created time.
     */
    property getCreatedTime();

    /**
     * @brief      Gets the server uptime.
     *
     * @return     The uptime.
     */
    property getUptime();

    /**
     * @brief      Gets the codec encryption mode.
     *
     * @return     The codec encryption mode.
     */
    serverChangeableParam getCodecEncryptionMode();

    /**
     * @brief      Gets the host message.
     *
     * @return     The host message.
     */
    serverChangeableParam getHostMessage();

    /**
     * @brief      Gets the host message mode.
     *
     * @return     The host message mode.
     */
    serverChangeableParam getHostMessageMode();

    /**
     * @brief      Gets the file base.
     *
     * @return     The file base.
     */
    property getFileBase();

    /**
     * @brief      Gets the default server group.
     *
     * @return     The default server group.
     */
    serverChangeableParam getDefaultServerGroup();

    /**
     * @brief      Gets the default channel group.
     *
     * @return     The default channel group.
     */
    serverChangeableParam getDefaultChannelGroup();

    /**
     * @brief      Gets the password flag.
     *
     * @return     The password flag.
     */
    serverChangeableParam getPasswordFlag();

    /**
     * @brief      Gets the default channel admin group.
     *
     * @return     The default channel admin group.
     */
    serverChangeableParam getDefaultChannelAdminGroup();

    /**
     * @brief      Gets the host banner url.
     *
     * @return     The host banner url.
     */
    serverChangeableParam getHostBannerURL();

    /**
     * @brief      Gets the host banner graphics url.
     *
     * @return     The host banner graphics url.
     */
    serverChangeableParam getHostBannerGfxURL();

    /**
     * @brief      Gets the host banner graphics interval.
     *
     * @return     The host banner graphics interval.
     */
    serverChangeableParam getHostBannerGfxInterval();

    /**
     * @brief      Gets the complain settings.
     *
     * @return     The complain settings.
     */
    serverComplainSettings getComplainSettings();

    /**
     * @brief      Gets the minimum clients in channel before silence.
     *
     * @return     The minimum clients in channel before silence.
     */
    serverChangeableParam getMinClientsInChannelBeforeSilence();

    /**
     * @brief      Gets the priority speaker dimm modificator.
     *
     * @return     The priority speaker dimm modificator.
     */
    serverChangeableParam getPrioritySpeakerDimmModificator();

    /**
     * @brief      Gets the server id.
     *
     * @return     The server id.
     */
    property getServerID();

    /**
     * @brief      Gets the antiflood settings.
     *
     * @return     The antiflood settings.
     */
    serverAntifolldSettings getAntifloodSettings();

    /**
     * @brief      Gets the client connections.
     *
     * @return     The client connections.
     */
    property getClientConnections();

    /**
     * @brief      Gets the query client connections.
     *
     * @return     The query client connections.
     */
    property getQueryClientConnections();

    /**
     * @brief      Gets the hostbutton settings.
     *
     * @return     The hostbutton settings.
     */
    serverHostButtonSettings getHostbuttonSettings();

    /**
     * @brief      Gets the transfer settings.
     *
     * @return     The transfer settings.
     */
    serverTransferSettings getTransferSettings();

    /**
     * @brief      Gets the transfer information.
     *
     * @return     The transfer information.
     */
    serverTransferProperties getTransferInfo();

    /**
     * @brief      Gets the port.
     *
     * @return     The port.
     */
    serverChangeableParam getPort();

    /**
     * @brief      Gets the autostart.
     *
     * @return     The autostart.
     */
    serverChangeableParam getAutostart();

    /**
     * @brief      Gets the machine id.
     *
     * @return     The machine id.
     */
    property getMachineID();

    /**
     * @brief      Gets the needed identity security level.
     *
     * @return     The needed identity security level.
     */
    serverChangeableParam getNeededIdentitySecurityLevel();

    /**
     * @brief      Gets the server log settings.
     *
     * @return     The server log settings.
     */
    serverLogSettings getServerLogSettings();

    /**
     * @brief      Gets the icon id.
     *
     * @return     The icon id.
     */
    serverChangeableParam getIconID();

    /**
     * @brief      Gets the reserved slots.
     *
     * @return     The reserved slots.
     */
    serverChangeableParam getReservedSlots();

    /**
     * @brief      Gets the packetloss status.
     *
     * @return     The packetloss status.
     */
    serverPacketLossProperties getPacketlossStatus();

    /**
     * @brief      Gets the ping.
     *
     * @return     The ping.
     */
    property getPing();

    /**
     * @brief      Gets the ip.
     *
     * @return     The ip.
     */
    property getIP();

    /**
     * @brief      Gets the weblist enabled.
     *
     * @return     The weblist enabled.
     */
    serverChangeableParam getWeblistEnabled();

    /**
     * @brief      Gets the ask for privilegekey.
     *
     * @return     The ask for privilegekey.
     */
    property getAskForPrivilegekey();

    /**
     * @brief      Gets the hostbanner mode.
     *
     * @return     The hostbanner mode.
     */
    serverChangeableParam getHostbannerMode();

    /**
     * @brief      Gets the temporary channel delete delay.
     *
     * @return     The temporary channel delete delay.
     */
    serverChangeableParam getTemporaryChannelDeleteDelay();

    /**
     * @brief      Gets the clients minimum version.
     *
     * @return     The clients minimum version.
     */
    serverClientsMinimumVersion getClientsMinimumVersion();

    /**
     * @brief      Gets the status.
     *
     * @return     The status.
     */
    property getStatus();

    /**
     * @brief      Gets the connection information.
     *
     * @return     The connection information.
     */
    serverConnectionProperties getConnectionInfo();

    /**
     * @brief      Creates a channel.
     *
     * @param[in]  channelName  The channel name
     *
     * @return     { description_of_the_return_value }
     */
    Channel createChannel(string channelName);

    /**
     * @brief      Creates a channel.
     *
     * @param[in]  channelName        The channel name
     * @param[in]  channelProperties  The channel properties
     *
     * @return     { description_of_the_return_value }
     */
    Channel createChannel(string channelName, map<string, string> channelProperties);

    /**
     * @brief      { function_description }
     *
     * @param[in]  name  The name
     * @param[in]  type  The type
     *
     * @return     { description_of_the_return_value }
     */
    Group channelGroupCreate(string name, groupDbType type = groupRegular);

    /**
     * @brief      { function_description }
     *
     * @param[in]  name  The name
     * @param[in]  type  The type
     *
     * @return     { description_of_the_return_value }
     */
    Group serverGroupCreate(string name, groupDbType type = groupRegular);

  };
}
#endif
