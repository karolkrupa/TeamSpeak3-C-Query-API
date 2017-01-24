#include <functions.hpp>

using namespace Ts3Api;

string Ts3Api::messageEncode(string message) {
  size_t pos = 0;

  string unescaped[11] = {"\\", "/", " ", "|", "\a", "\b", "\f", "\n", "\r", "\t", "\v"};
  string escaped[11] = {"\\\\", "\\/", "\\s", "\\p", "\\a", "\\b", "\\f", "\\n", "\\r", "\\t", "\\v"};

  for(int i = 0; i < 11; i++) {
    while((pos = message.find(unescaped[i], pos)) != string::npos) {
      message.replace(pos, 1, escaped[i]);
      pos+1;
    }
    pos = 0;
  }

  return message;
}

string Ts3Api::messageDecode(string message) {
  size_t pos = 0;

  string escaped[11] = {"\\", "/", " ", "|", "\a", "\b", "\f", "\n", "\r", "\t", "\v"};
  string unescaped[11] = {"\\\\", "\\/", "\\s", "\\p", "\\a", "\\b", "\\f", "\\n", "\\r", "\\t", "\\v"};

  for(int i = 0; i < 11; i++) {
    while((pos = message.find(unescaped[i], pos)) != string::npos) {
      message.replace(pos, 2, escaped[i]);
      pos+1;
    }
    pos = 0;
  }

  return message;
}

void Ts3Api::split(map<string, string> &returnedMap, string input) {
  size_t gapPos = 0, prevGapPos = 0, dividerPos = 0;
  string buff;
  input += " ";

  while((gapPos = input.find(" ", gapPos+1)) != string::npos) {
    if(prevGapPos != 0) prevGapPos++;
    buff = input.substr(prevGapPos, gapPos-prevGapPos);
    if((dividerPos = buff.find("=")) != string::npos) {
      returnedMap[buff.substr(0, dividerPos)] = messageDecode(buff.substr(dividerPos+1, string::npos));
    }
    prevGapPos = gapPos;
  }
}

void Ts3Api::split(map<string, map<string, string>> &returnedMap, string input, string mapIndex) {
  size_t pos = 0, prevPos = 0;
  map<string, string> bufferMap;
  int index = 0;
  input += "|";

  while((pos = input.find("|", pos+1)) != string::npos) {
    if(prevPos != 0) prevPos++;

    split(bufferMap, input.substr(prevPos, pos-prevPos));

    if(mapIndex != "intiger") {
      if(index == 0) {
        if(bufferMap.find(mapIndex) == bufferMap.end()) {
          mapIndex = "intiger";
          returnedMap[to_string(index)] = bufferMap;
        }
      }else {
        index++;
        if(bufferMap.find(mapIndex) == bufferMap.end()) continue;

        returnedMap[bufferMap[mapIndex]] = bufferMap;
      }
    }else {
      returnedMap[to_string(index)] = bufferMap;
      index++;
    }
  }
}
