#include "md5_hash.h"
 
int main()
{
    MD5Hash md5 = MD5Hash("data.txt");
    md5.outputMD5HashedCardNum();
    return 0;
}
