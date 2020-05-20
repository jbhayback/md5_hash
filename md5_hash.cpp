#include <cstdio>
#include <iomanip>
#include <fstream>

#include "md5_hash.h"

// Four auxillary functions for MD5 algorithm
unsigned int F(unsigned int x, unsigned int y, unsigned int z) {
  return x&y | ~x&z;
}
 
unsigned int G(unsigned int x, unsigned int y, unsigned int z) {
  return x&z | y&~z;
}

unsigned int H(unsigned int x, unsigned int y, unsigned int z) {
  return x^y^z;
}

unsigned int I(unsigned int x, unsigned int y, unsigned int z) {
  return y ^ (x | ~z);
}

// rotate_l rotates x left n bits.
unsigned int rotate_l(unsigned int x, int n) {
  return (x << n) | (x >> (32-n));
}

// Rotation should be separated from addition
void FF(unsigned int &a, unsigned int b, unsigned int c, unsigned int d,
  unsigned int x, unsigned int s, unsigned int ac) {
  a = rotate_l(a+ F(b,c,d) + x + ac, s) + b;
}
 
void GG(unsigned int &a, unsigned int b, unsigned int c, unsigned int d,
  unsigned int x, unsigned int s, unsigned int ac) {
  a = rotate_l(a + G(b,c,d) + x + ac, s) + b;
}
 
void HH(unsigned int &a, unsigned int b, unsigned int c, unsigned int d,
  unsigned int x, unsigned int s, unsigned int ac) {
  a = rotate_l(a + H(b,c,d) + x + ac, s) + b;
}
 
void II(unsigned int &a, unsigned int b, unsigned int c, unsigned int d,
  unsigned int x, unsigned int s, unsigned int ac) {
  a = rotate_l(a + I(b,c,d) + x + ac, s) + b;
}

MD5Hash::MD5Hash(const std::string& file_path)
{
  std::string cardNum = getCardNumData(file_path);
  initialize();
  process(cardNum.c_str(), cardNum.length());
  finalize();
}
 
void MD5Hash::initialize()
{
  is_ok_ = false;
  counters_[0] = 0;
  counters_[1] = 0;
  states_[0] = 0x653453fe;
  states_[1] = 0xabcd3554;
  states_[2] = 0x436cdcd3;
  states_[3] = 0x367676dd;
}
 
// decodes input.
void MD5Hash::decode(unsigned int output[], const unsigned char input[], unsigned int len)
{
  for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
    output[i] = ((unsigned int)input[j]) | (((unsigned int)input[j+1]) << 8) |
      (((unsigned int)input[j+2]) << 16) | (((unsigned int)input[j+3]) << 24);
}
 
 
// encodes input into output.
void MD5Hash::encode(unsigned char output[], const unsigned int input[], unsigned int len)
{
  for (unsigned int i = 0, j = 0; j < len; i++, j += 4) {
    output[j] = input[i] & 0xff;
    output[j+1] = (input[i] >> 8) & 0xff;
    output[j+2] = (input[i] >> 16) & 0xff;
    output[j+3] = (input[i] >> 24) & 0xff;
  }
}
 
// MD5Hash algo application
// Each round consists of a sequence of 16 steps operating on register buffer
void MD5Hash::transform(const unsigned char block[block_size])
{
  unsigned int A, B, C, D, X[16];
  A = states_[0];
  B = states_[1];
  C = states_[2];
  D = states_[3];
  decode(X, block, block_size);
 
  /* 1st Round*/
  FF(A, B, C, D, X[ 0], S11, T_consts[0]);
  FF(D, A, B, C, X[ 1], S12, T_consts[1]);
  FF(C, D, A, B, X[ 2], S13, T_consts[2]);
  FF(B, C, D, A, X[ 3], S14, T_consts[3]);
  FF(A, B, C, D, X[ 4], S11, T_consts[4]);
  FF(D, A, B, C, X[ 5], S12, T_consts[5]);
  FF(C, D, A, B, X[ 6], S13, T_consts[6]);
  FF(B, C, D, A, X[ 7], S14, T_consts[7]);
  FF(A, B, C, D, X[ 8], S11, T_consts[8]);
  FF(D, A, B, C, X[ 9], S12, T_consts[9]); 
  FF(C, D, A, B, X[10], S13, T_consts[10]);
  FF(B, C, D, A, X[11], S14, T_consts[11]);
  FF(A, B, C, D, X[12], S11, T_consts[12]);
  FF(D, A, B, C, X[13], S12, T_consts[13]);
  FF(C, D, A, B, X[14], S13, T_consts[14]);
  FF(B, C, D, A, X[15], S14, T_consts[15]);
 
  /* 2nd Round */
  GG(A, B, C, D, X[ 1], S21, T_consts[16]);
  GG(D, A, B, C, X[ 6], S22, T_consts[17]);
  GG(C, D, A, B, X[11], S23, T_consts[18]);
  GG(B, C, D, A, X[ 0], S24, T_consts[19]);
  GG(A, B, C, D, X[ 5], S21, T_consts[20]);
  GG(D, A, B, C, X[10], S22, T_consts[21]);
  GG(C, D, A, B, X[15], S23, T_consts[22]);
  GG(B, C, D, A, X[ 4], S24, T_consts[23]);
  GG(A, B, C, D, X[ 9], S21, T_consts[24]);
  GG(D, A, B, C, X[14], S22, T_consts[25]);
  GG(C, D, A, B, X[ 3], S23, T_consts[26]);
  GG(B, C, D, A, X[ 8], S24, T_consts[27]);
  GG(A, B, C, D, X[13], S21, T_consts[28]);
  GG(D, A, B, C, X[ 2], S22, T_consts[29]);
  GG(C, D, A, B, X[ 7], S23, T_consts[30]);
  GG(B, C, D, A, X[12], S24, T_consts[31]);
 
  /* 3rd Round */
  HH(A, B, C, D, X[ 5], S31, T_consts[32]);
  HH(D, A, B, C, X[ 8], S32, T_consts[33]);
  HH(C, D, A, B, X[11], S33, T_consts[34]);
  HH(B, C, D, A, X[14], S34, T_consts[35]);
  HH(A, B, C, D, X[ 1], S31, T_consts[36]);
  HH(D, A, B, C, X[ 4], S32, T_consts[37]);
  HH(C, D, A, B, X[ 7], S33, T_consts[38]);
  HH(B, C, D, A, X[10], S34, T_consts[39]);
  HH(A, B, C, D, X[13], S31, T_consts[40]);
  HH(D, A, B, C, X[ 0], S32, T_consts[41]);
  HH(C, D, A, B, X[ 3], S33, T_consts[42]);
  HH(B, C, D, A, X[ 6], S34, T_consts[43]);
  HH(A, B, C, D, X[ 9], S31, T_consts[44]);
  HH(D, A, B, C, X[12], S32, T_consts[45]);
  HH(C, D, A, B, X[15], S33, T_consts[46]);
  HH(B, C, D, A, X[ 2], S34, T_consts[47]);
 
  /* 4th Round */
  II(A, B, C, D, X[ 0], S41, T_consts[48]);
  II(D, A, B, C, X[ 7], S42, T_consts[49]);
  II(C, D, A, B, X[14], S43, T_consts[50]);
  II(B, C, D, A, X[ 5], S44, T_consts[51]);
  II(A, B, C, D, X[12], S41, T_consts[52]);
  II(D, A, B, C, X[ 3], S42, T_consts[53]);
  II(C, D, A, B, X[10], S43, T_consts[54]);
  II(B, C, D, A, X[ 1], S44, T_consts[55]);
  II(A, B, C, D, X[ 8], S41, T_consts[56]);
  II(D, A, B, C, X[15], S42, T_consts[57]);
  II(C, D, A, B, X[ 6], S43, T_consts[58]);
  II(B, C, D, A, X[13], S44, T_consts[59]);
  II(A, B, C, D, X[ 4], S41, T_consts[60]);
  II(D, A, B, C, X[11], S42, T_consts[61]);
  II(C, D, A, B, X[ 2], S43, T_consts[62]);
  II(B, C, D, A, X[ 9], S44, T_consts[63]);
 
  // Append
  states_[0] += A;
  states_[1] += B;
  states_[2] += C;
  states_[3] += D;
 
  // Clear
  memset(X, 0, sizeof(X));
}
 
// MD5Hash processing operation.
void MD5Hash::process(const unsigned char input[], unsigned int length)
{
  // compute number of bytes of input
  unsigned int index = counters_[0] / 8 % block_size;
 
  // process number of bits
  counters_[0] += length * 8;
  counters_[1] += length >> 29;
 
  // number of bytes we need to fill in buff
  unsigned int part_one = 64 - index;
  unsigned int i;
 
  // transform as many times as possible.
  if (length >= part_one)
  {
    // fill buff first, transform
    memcpy(&buff_[index], input, part_one);
    transform(buff_);
 
    // transform chunks of block_size (64 bytes)
    for (i = part_one; i + block_size <= length; i += block_size)
      transform(&input[i]);
 
    index = 0;
  }
  else
  {
    i = 0;
  }
    
 
  // buff remaining input
  memcpy(&buff_[index], &input[i], length-i);
}

// for convenience provide a verson with signed char
void MD5Hash::process(const char input[], unsigned int length)
{
  process((const unsigned char*)input, length);
}
 
// MD5Hash finalization. Ends an MD5Hash message-digest_ operation, writing the
// the message digest_ and zeroizing the context.
MD5Hash& MD5Hash::finalize()
{
  if (!is_ok_) {
    // Save number of bits
    unsigned char bits[8];
    encode(bits, counters_, 8);
 
    // pad out to 56 mod 64.
    unsigned int index = counters_[0] / 8 % block_size;
    unsigned int padLen = (index < 56) ? (56 - index) : (120 - index);
    process(padding, padLen);
 
    // Append length (before padding)
    process(bits, 8);
 
    // Store states in digest_
    encode(digest_, states_, 16);
 
    // Clearing
    memset(buff_, 0, sizeof(buff_));
    memset(counters_, 0, sizeof(counters_));
 
    is_ok_=true;
  }
 
  return *this;
}

std::string MD5Hash::getCardNumData(const std::string& file_path) const
{
    std::string cardNum;
    uint64_t cardNumDigit;
    
    std::ifstream inFile;
    
    inFile.open(file_path);
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    
    while (inFile >> cardNumDigit) {
        cardNum += std::to_string(cardNumDigit);
    }
    inFile.close();

    return cardNum;
}

// return hex representation of digest as string
std::string MD5Hash::getMessageDigest() const
{
  if (!is_ok_)
    return "";
 
  char buf[33];
  for (int i=0; i<16; i++)
  {
    sprintf(buf+i*2, "%02x", digest_[i]);
  }
  buf[32]=0;
 
  return std::string(buf);
}

void MD5Hash::outputMD5HashedCardNum() const
{
  std::ofstream ouputFile;
  auto md5_result = getMessageDigest();
  ouputFile.open(md5_result);
  ouputFile << md5_result;
  ouputFile.close();
}
