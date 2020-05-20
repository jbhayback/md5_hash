#ifndef MD5_HASH_H
#define MD5_HASH_H
 
#include <cstring>
#include <iostream>

#include "md5_hash_const.h"


class MD5Hash
{
private:
  static void decode(unsigned int output[], const unsigned char  input[], unsigned int len);
  static void encode(unsigned char output[], const unsigned int input[], unsigned int len);
  std::string getCardNumData(const std::string& filePath) const;
  void initialize();
  void transform(const unsigned char block[block_size]);
  void process(const unsigned char *buff, unsigned int length);
  void process(const char *buff, unsigned int length);
  MD5Hash& finalize();
  std::string getMessageDigest() const;
 
  bool is_ok_;
  unsigned int counters_[2];
  unsigned int states_[4];
  unsigned char buff_[block_size];
  unsigned char digest_[16];

public:
  MD5Hash() = default;
  MD5Hash(const std::string& filePath);
  void outputMD5HashedCardNum() const;
};

#endif // MD5_HASH_H
