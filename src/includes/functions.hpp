#pragma once

#include <string>
#include <map>

using namespace std;

namespace Ts3Api {
  string messageEncode(string message);

  string messageDecode(string message);

  void split(map<string, string> &returnedMap, string input);

  void split(map<string, map<string, string>> &returnedMap, string input, string mapIndex = "intiger");
}
