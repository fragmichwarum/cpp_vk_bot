#ifndef SHA256_HPP
#define SHA256_HPP

#include <cstring>
#include <sstream>
#include <iomanip>
#include <fstream>

class SHA256 {
protected:
  const char* message;
  int message_size;
  unsigned char* pre_processed_msg;
  int num_tot_bits;
  std::string output;
  void pre_processing();
  void process_512_chunks();

public:
  SHA256(const char* msg);
   ~SHA256();
  std::string hash();
};
#endif //SHA256_HPP
