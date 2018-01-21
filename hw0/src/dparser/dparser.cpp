#include "dparser.h"

namespace Hpc0 {
  DParser::DParser() {

  }

  DParser::~DParser() {

  }

  void DParser::LoadMap(const std::string& filepath) {
    // setup mmap
    std::size_t f_size = GetFileSize(filepath);
    std::cout << "File size: " << f_size << std::endl;

    int fd = open(filepath.c_str(), O_RDONLY, 0);
    if (fd == -1) {
      std::cout << "\n<<DParser::LoadMap>>" << std::endl;
  		std::cout << "Failed to open given file..." << std::endl;
  		return;
    }

    // OSX does not support MAP_POPULATE...
    char* file_data = static_cast<char*>(mmap(NULL, f_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (file_data == MAP_FAILED) {
      std::cout << "\n<<DParser::LoadMap>>" << std::endl;
  		std::cout << "Failed to map file..." << std::endl;
  		return;
    }

    // parse
    int first_line = 0;
    int num_colons = 0;
    char* begin;
    char* end; // these are for keeping track of where the strings are
    char* feature;
    Data xy_data; // holds the data to calculate (X,Y) for current line
    begin = file_data;
    std::cout << "(DParser::LoadMap) Parsing file: \"" << filepath << "\"..." << std::endl;
    for (std::size_t i = 0; i <= f_size; ++i) {
      if (num_colons < 4 && file_data[i] == ':') {
        end = file_data + i;
        std::string column(begin, end);
        std::cout << "Column: " << column << std::endl;
        begin = file_data + i + 1;
        num_colons++;
        if (num_colons == 1) {
          xy_data.lon = atof(column.c_str());
        } else if (num_colons == 2) {
          xy_data.lat = atof(column.c_str());
        } else if (num_colons == 3) {
          xy_data.delta_x = atof(column.c_str());
        }
        if (num_colons == 4) {
          xy_data.delta_y = atof(column.c_str());
          feature = begin;
          // calculate the coord here and insert with feature address
          Coord key = MakeCoord(xy_data);
          this->data_map.insert(std::make_pair(key, feature));
        }
      } else if (file_data[i] == '\n') {
        //std::cout << "feature: " << *feature << std::endl;
        end = file_data + i;
        begin = file_data + i + 1;
        num_colons = 0;
        first_line++;
      }
      if (first_line == 4) {
        break;
      }

      /*if (file_data[i] == '\n') {
        end = file_data + i;

        // perform string stuff here
        std::string line(begin, end);
        //std::cout << "\nFirst line:\n" << line << std::endl;
        //FillDataFromLine(line);

        begin = file_data + i + 1;
        //std::cout << "\tcurrent length: " << this->data_map.size() << std::endl;
      }*/
    }

    std::cout << "\nFinal length: " << this->data_map.size() << std::endl;

    //Testing the data structure.
    Coord item1;
    item1.x = 115.999;
    item1.y = 39.0533;

    Coord item2;
    item2.x = 115.999;
    item2.y = 39.0536;

    Coord item3;
    item3.x = 115.999;
    item3.y = 39.0539;

    if (item1 == item3) {
      std::cout << "does == ?: true" << std::endl;
    } else {
      std::cout << "does == ?: false" << std::endl;
    }

    std::map<Coord, char*>::iterator it = this->data_map.find(item1);
    if (it != this->data_map.end()) {
      std::cout << it->first.x << " | " << it->first.y << std::endl;
      std::cout << "Address of item 1: " << static_cast<void*>(it->second) << std::endl;
    } else {
      std::cout << "PROBLEM :()\n" << std::endl;
    }

    for (it = this->data_map.begin(); it != this->data_map.end(); ++it) {
      std::cout << "Address of item: " << static_cast<void*>(it->second) << std::endl;
    }

    PrintMap(4);

    // cleanup
    int unmap_check = munmap(file_data, f_size);
    if (unmap_check != 0) {
      std::cout << "\n<<DParser::LoadMap>>" << std::endl;
  		std::cout << "Failed to unmap..." << std::endl;
    }
    close(fd);
  }

  DParser::Coord DParser::MakeCoord(const Data& xy_data) {
    Coord result;
    result.x = static_cast<double>((xy_data.delta_x/MAGIC_NUM) * cos(xy_data.lat) + xy_data.lon);
    result.y = static_cast<double>((xy_data.delta_y/MAGIC_NUM) + xy_data.lat);
    std::cout << "Result was: " << result.x << " | "<< result.y << std::endl;
    return result;
  }

  void DParser::PrintMap(int rows) {
    if (rows <= 0) {
      return;
    }

    std::cout << "\n(DParser::PrintMap) Printing map..." << std::endl;
    for (std::map<Coord, char*>::iterator it = this->data_map.begin(); it != this->data_map.end(); ++it) {
      std::cout << it->first.x << " | " << it->first.y << " \n" << std::endl;
      rows--;
      if (rows == 0) {
        return;
      }
    }
  }

  void DParser::FillDataFromLine(std::string& line) {
    /*typedef boost::tokenizer<boost::escaped_list_separator<char>> Tokenizer;
	  boost::escaped_list_separator<char> delim_args("\\",":","\"");

    std::string temp;            // holds current parsed token
    std::deque<std::string> row; // holds parsed tokens

    DParser::Coord coord;        // holds the data from tokens to be stored
    DParser::Datum datum;
    coord.x = 0.0;
    coord.y = 0.0;

    try {
      // make tokens from string
      Tokenizer tok(line, delim_args);
      row.assign(tok.begin(), tok.end());
    } catch (const boost::exception& ex) {
      std::cout << "\n<<DParser::FillDataFromLine -- Boost Exception>>" << std::endl;
      std::cerr << boost::diagnostic_information(ex);
      std::cout << "Line was: " << line << std::endl;
    }

    // lon
    temp = row.front();
    if (temp[0] == 'l') {
      return;
    }
    datum.lon = atof(temp.c_str());
    row.pop_front();

    // lat
    temp = row.front();
    datum.lat = atof(temp.c_str());
    row.pop_front();

    // delta_x
    temp = row.front();
    datum.delta_x = atof(temp.c_str());
    row.pop_front();

    // delta_y
    temp = row.front();
    datum.delta_y = atof(temp.c_str());
    row.pop_front();

    // remaining is just the feature data, toss into the features array
    int i = 0;
    for (std::deque<std::string>::iterator it = row.begin(); it != row.end(); ++it) {
      temp = *it;
      datum.features[i] = atof(temp.c_str());
    }

    // calculate (X / Y) and store in data_map
    coord.x = static_cast<double>((datum.delta_x/MAGIC_NUM) * cos(datum.lat) + datum.lon);
    coord.y = static_cast<double>((datum.delta_y/MAGIC_NUM) + datum.lat);
    this->data_map.insert(std::make_pair(coord, datum));
    //std::cout << "Coord: " << coord.x << " | "<< coord.y << std::endl;*/
  }

  std::size_t DParser::GetFileSize(const std::string& filepath) {
    FILE *fp = NULL;
    fp = fopen(filepath.c_str(),"rb");
    fseek(fp, 0, SEEK_END);
    std::size_t file_size = ftell(fp);
    fclose(fp);
    return file_size;
  }

} // namespace
