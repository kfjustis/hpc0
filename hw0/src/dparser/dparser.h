#pragma once

#include <cstddef>
#include <cstdio>
#include <deque>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <map>
#include <unistd.h>
#include <utility>
#include <vector>

#include <boost/exception/diagnostic_information.hpp>
#include <boost/tokenizer.hpp>

#define MAGIC_NUM 221200

namespace Hpc0 {

class DParser {
public:
  struct Coord {
    double x;
    double y;

    bool operator <(const Coord& o) const {
      double e = 0.000001;
      /*if (x != o.x) {
        return x < o.x;
      } else {
        return y < o.y;
      }*/
      if (abs(x - o.x) > e) { // not ==
        return x < o.x;
      } else if (abs(y - o.y) > e) {
        return y < o.y;
      }
      //return x < o.x || y < o.y &&
      //return ((abs(x - o.x) > e) && (x < o.x));
      /*if (x != o.x) {
        return ((abs(x - o.x) > e) && (x < o.x));
      } else {
        return ((abs(y - o.y) > e) && (y < o.y));
      }*/
      //return ((std::tie(x, y) < std::tie(o.x, o.y)));
    }

    bool operator ==(const Coord& o) const {
      double e = 0.0001;
      if (abs(x - o.x) <= e && abs(y - o.y) <= e) {
        return true;
      } else {
        return false;
      }
    }
  };

  struct Data {
    double lon;
    double lat;
    double delta_x;
    double delta_y;
  };

  DParser();

  ~DParser();

  void LoadMap(const std::string& filepath);

  void PrintMap(int rows);

private:
  std::map<Coord,char*> data_map;

  Coord MakeCoord(const Data& row_data);

  void FillDataFromLine(std::string& line);

  std::size_t GetFileSize(const std::string& filepath);

};

} // namespace
