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


  /** @brief Pozwala odczytać datę zapisaną w formacie timestamp
   *
   *  Dzięki tej strukturze możliwa jest konwersja czasu z formatu timestamp na dowolny inny format daty
   */
  struct timestampTime {
    string value;

    /** @brief Zwraca zapisaną datę w wybranym formacie
     *
     */
    string toDate(string pattern = "%Y-%m-%d %H.%M.%S");

    timestampTime(string time);
  };

  /** @brief Pozwala na manupulacje wybranym użytkownikiem
   *
   */
  class Client {
    friend class Permission;
    friend class Group;
  public:
    /// Zbiór indentyfikatorów użytkownika
    struct IDs {
      string dbid   = "unknown";
      string clid   = "unknown";
      string uid    = "unknown";

      IDs(string clid ="unknown", string uid ="unknown", string dbid = "unknown");
    };

    /// Struktura zawierająca informacje z polecenia query: "clientinfo"
    struct clientInfoProperties : public ts3ObjectProperties {
      IDs &ids;

      void update();

      clientInfoProperties(Client::IDs &ids,  Server &server, time_t &updateTime, bool clientListInit);
    };

    struct nickname {
      string value;
      string phonetic;

      nickname(string nickname, string phonetic);
    };

    /** @brief Struktura pozwala na sprawdzenie oraz zmianę wybranej właściwości użytkownika
    */
    struct changeableParam {
    private:
      /// Referencja do właściciela właściwości
      Client &client;
    public:
      /// Nazwa właściwości
      string paramName;
      /// Wartość wybranego parametru
      string value;

      changeableParam(Client &client, string value, string paramName);

      /** @brief Ustawia wybraną właściwośc na wartość parametru value
       *  @param value Nowa wartość wybranego parametru
      */
      ts3Response change(string value);
    };

    /// Zawiera informacje o transferze dengo użytkownika
    struct transferInfo {
      string month_bytes_uploaded;
      string month_bytes_downloaded;
      string total_bytes_uploaded;
      string total_bytes_downloaded;
    };

    /// Zawiera informacje o połączeniu danego użykownika
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
    /** @brief Tworzy nowy obiekt użytkownika
     *  @param server Obiekt Server z którego pochodzić ma użytkownik
     *  @param clientIDs Struktura IDs zawierająca conajmniej jeden identyfikator użytkownika
     *  @param clientListInit Informuje o niepełnych właściwościach użytkownika pochodzących z listy użytkowników
    */
    Client(Server& server, IDs clientIDs, bool clientListInit = false);

    /** @brief Wymusza aktualizację informacji o użytkowniku
     *
     *  Po wywołaniu tej metody przy następnej próbie pobrania jakiejkolwiek właściwości użytkownika wszystkie informacje zostaną zaaktualizowane
    */
    void update();

    /** @brief Sprawdza czy dany obiekt jest prawidłowy
     *
     *  Pozwala sprawdzić czy pobrany obiekt użytkownika jest poprawny tzn. zawiera jego identyfikator pozwalający pobrać o nim informacje.<br>
     *  Jeżeli funkcja zwraca wartość false nie należy używać danego obiektu ponieważ nie będzie on w stanie zwrócić żadnych informacji.<br>
     *  Zalecane jest wywołanie tej funkcji po pobraniu użytkownika za pomocą getClientByNickname().
    */
    bool good();

    /// Zwraca "Client ID" użytkownika
    property getCLID();

    /// Zwraca "Unique Identifiter" użytkownika
    property getUID();

    /// Zwraca "Database ID" użytkownika
    property getDBID();

    /// Zwraca "Idle Time" użytkownika
    property getIdleTime();

    nickname getNickname();

    property getVersion();

    property getVersionSign();

    property getPlatform();

    bool getInputMuted();

    bool getOutputMuted();

    bool getOutputOnlyMuted();

    bool getInputHardware();

    bool getOutputHardware();

    property getDefaultChannel();

    property getMetaData();

    bool getRecordingStatus();

    property getSecurityHash();

    property getLoginName();

    Group getChannelGroup();

    property getChannelGroupID();

    vector<Group> getServerGroups();

    vector<string> getServerGroupsList();

    timestampTime getCreatedTime();

    timestampTime getLastconnectedTime();

    property getTotalConnections();

    bool getAwayStatus();

    property getAwayMessage();

    property getType(); // enum

    property getAvatarFlag();

    Permission getTalkPower();

    property getTalkRequest();

    property getTalkRequestMsg();

    changeableParam getDescription();

    changeableParam getTalkerStatus();

    transferInfo getTransferInfo();

    bool getPrioritySpeakerStatus();

    Permission getNeededServerqueryViewPower();

    property getDefaultToken();

    changeableParam getIconId();

    changeableParam getChannelCommanderStatus();

    /** @brief Zwraca kraj użytkownika
     */
    property getCountry();

    property getChannelGroupInheritChannelID();

    property getBadges();

    property getBase64HashClientUID();

    connectionInfo getConnectionInfo();

    ts3Response addGroup(string groupId);
    ts3Response addGroup(const Group &group);
    ts3Response removeGroup(string groupId);
    ts3Response removeGroup(const Group &group);
    ts3Response addChannelGroup(string groupId, string channelId);
    ts3Response addChannelGroup(string groupId, const Channel &channel);
    ts3Response addChannelGroup(const Group &group , const Channel &channel);
    ts3Response addChannelGroup(const Group &group , string channelId);
    Permission getPermission(string permName);
    ts3Response dbDelete();
    ts3Response setServerqueryLogin(string loginName, string &password);
    ts3Response move(string cid, string password = "");
    ts3Response kickFromChannel(string reason);
    ts3Response kickFromServer(string reason);
    ts3Response poke(string message);
    ts3Response complain(string message);
    ts3Response delComplain(string accuserDbid);
    ts3Response delAllComplains();
    ts3Response ban(string banTime = "", string reason ="");
    ts3Response sendMessage(string message);
  };
}
