#include <chrono>
#include "dparser.h"

int main(int argc, char** argv) {
  // command line args
  if (argc != 2) {
    std::cout << "\n<<MAIN>>" << std::endl;
    std::cout << "USAGE: " << argv[0] << " [data filepath]" << std::endl;
    return -1;
  }

  // parse data file
  Hpc0::DParser parser;
  auto start = std::chrono::system_clock::now();
  parser.LoadMap(argv[1]);
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> time = end - start;
  std::cout << "Total time: " << time.count() << std::endl;

  return 0;
}
