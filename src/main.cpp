#include <iostream>
#include <string>
#include <Server.hpp>

using namespace std;

int main() {
  Ts3Api::Server server("e-bot.eu", "10011");
  server.login("serveradmin", "PASSWORD");
  server.selectServer("9987");
  server.setNickname("eBot TRAIL");


  auto client = server.getClientByNickname("Exus");

  //cout << client.getChannel().getID() << endl;


  /*

  //  if(server.receiverStart())
  //    cout << "Receiver Started" << endl;
  //  else
  //    cout << "Receiver Error" << endl;

  // if(server.receiverStop())
  //   cout << "Receiver Stoped" << endl;
  // else
  //   cout << "Receiver stop error" << endl;

  // if(server.receiverStatus())
  //   cout << "Reveiver Runing" << endl;



  auto client = server.getClientByNickname("adasdas");



  if(client.good())
    cout << "OK" << endl;
  else
    cout << "ERROR" << endl;

  //cout << client.clientInfo.getProperty("cid").value << endl;

  //client.sendMessage("Siema ziomuś");
  //client.poke("Siema tutaj też");

  client.addChannelGroup("10", "40");
  //cout << client.getDescription().change("Lol").data << endl;

  //cout << client.clientInfo.getProperty("cid").value << endl;
  // auto ret = server.executeCommand("clientlist2");
  // cout << "Poczatek:" << ret.errorMsg << ":Koniec" << endl;
  // if(ret.error) cout << "ERROR" << endl;
  cout << "Koniec" << endl;
  */
  return 1;
}
