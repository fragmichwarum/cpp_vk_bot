#ifndef CRC32_HPP
#define CRC32_HPP

class CRC32 {
private:
  static const unsigned long crc32_lookup_table[256];
public:
  static unsigned long crc32gen(const char*);
};

#endif // CRC32_HPP
