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

  private:
    /// Descriptor ID
    int sock = -1;

    /// Errors queue
    queue<error> errors;

    time_t updateTime = time(NULL);

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

    Server();

    /**
     * @brief      Performs selected command on Ts3 Server
     *
     * @param[in]  command  The command
     *
     * @return     Data returned from the server
     */
    ts3Response executeCommand(string command);

    /**
     * @brief      Destroys the object.
     */
    ~Server();

    /**
     * @brief      Checks connection with Ts3 Server
     *
     * @return     true if connected
     */
    bool connected();

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

    void update();

    map<string, Server> getServerList();

    ts3Response serverDelete(string id = "");

    ts3Response serverCreate(map<string, string> serverProperties);

    ts3Response serverStart(string id = "");

    ts3Response serverStop(string id = "");

    ts3Response serverProcessStop();

    struct serverNameProperties {
        serverChangeableParam name;
        serverChangeableParam phonetic;
    };

    struct serverClientsOnlineProperties {
        string normal;
        string query;
    };

    struct serverComplainSettings {
        serverChangeableParam autoban_count;
        serverChangeableParam autoban_time;
        serverChangeableParam remove_time;
    };

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

    property getUID();
    serverNameProperties getName();
    serverChangeableParam getWelcomeMessage();
    property getPlatform();
    property getVersion();
    serverChangeableParam getMaxClients();
    serverChangeableParam getPassword();
    serverClientsOnlineProperties getClientsOnline(); //queryclientsonline
    property getChannelsOnline();
    property getCreatedTime();
    property getUptime();
    serverChangeableParam getCodecEncryptionMode();
    serverChangeableParam getHostMessage();
    serverChangeableParam getHostMessageMode();
    property getFileBase();
    serverChangeableParam getDefaultServerGroup();
    serverChangeableParam getDefaultChannelGroup();
    serverChangeableParam getPasswordFlag();
    serverChangeableParam getDefaultChannelAdminGroup();
    
    serverChangeableParam getHostBannerURL();
    serverChangeableParam getHostBannerGfxURL();
    serverChangeableParam getHostBannerGfxInterval();
    serverComplainSettings getComplainSettings();
    serverChangeableParam getMinClientsInChannelBeforeSilence();
    serverChangeableParam getPrioritySpeakerDimmModificator();
    property getServerID();
    serverAntifolldSettings getAntifloodSettings();
    property getClientConnections();
    property getQueryClientConnections();
    serverHostButtonSettings getHostbuttonSettings();
    serverTransferSettings getTransferSettings(); //serverChangeableParam getMaxDownloadTotalBandwidth(); serverChangeableParam getMaxUploadTotalBandwidth();
    serverTransferProperties getTransferInfo();
    serverChangeableParam getPort();
    serverChangeableParam getAutostart();
    property getMachineID();
    serverChangeableParam getNeededIdentitySecurityLevel();
    serverLogSettings getServerLogSettings();
    serverChangeableParam getIconID();
    serverChangeableParam getReservedSlots();
    serverPacketLossProperties getPacketlossStatus();
    property getPing();
    property getIP();
    serverChangeableParam getWeblistEnabled();
    property getAskForPrivilegekey();
    serverChangeableParam getHostbannerMode();
    serverChangeableParam getTemporaryChannelDeleteDelay();
    serverClientsMinimumVersion getClientsMinimumVersion();
    property getStatus();
    serverConnectionProperties getConnectionInfo();



    Channel createChannel(string channelName);
    Channel createChannel(string channelName, map<string, string> channelProperties);

    enum groupDbType {
        groupTemplate = 0,
        groupRegular,
        groupQuery
    };

    Group channelGroupCreate(string name, groupDbType type = groupRegular);
    Group serverGroupCreate(string name, groupDbType type = groupRegular);

  };
}
#endif
