#pragma once
#include <string>
#include <functions.hpp>

using namespace std;

namespace Ts3Api {
  struct error {
    string msg = "";
    int id = 0;

    error(string msg ="", int id=0);
  };

  struct socketData {
    string data;
    int bytes;

    socketData(string data = "", int bytes = 0);
  };

  struct ts3Response {
    string data = "";
    string errorId = "0";
    string errorMsg = "";
    bool error = true;

    ts3Response(string data = "", string errorId = "0", string errorMsg = "");

    string messageDecode(string message);

    string messageEncode(string message);
  };
}
