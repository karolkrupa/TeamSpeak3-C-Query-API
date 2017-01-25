#include <functions.hpp>
#include <iostream>

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

  while((gapPos = input.find(" ", prevGapPos)) != string::npos) {
    if(gapPos == (prevGapPos+1)) {
      prevGapPos = gapPos+1;
      continue;
    }

    buff = input.substr(prevGapPos, gapPos-prevGapPos);

    if((dividerPos = buff.find("=")) == string::npos) {
      returnedMap[buff] = "unknown";
    }else {
      if(dividerPos == (buff.length()-1)) {
        returnedMap[buff.substr(0, buff.length()-1)] = "unknown";
      }else {
        returnedMap[buff.substr(0, dividerPos)] = buff.substr(dividerPos+1, string::npos);
      }
    }

    prevGapPos = gapPos+1;
  }
}

void Ts3Api::split(map<string, map<string, string>> &returnedMap, string input, string mapIndex) {
  size_t pos = 0, prevPos = 0;
  map<string, string> buffMap;
  int index = 0;
  input += "|";

  while((pos = input.find("|", prevPos)) != string::npos) {
    split(buffMap, input.substr(prevPos, pos-prevPos));


    if(mapIndex == "intiger") 
      returnedMap[to_string(index)] = buffMap;
    else if(buffMap.find(mapIndex) != buffMap.end())
        returnedMap[buffMap[mapIndex]] = buffMap;
    
    prevPos = pos+1;
    index++;
  }
}
