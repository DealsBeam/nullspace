#ifndef NULLSPACE_NET_CRYPT_H_
#define NULLSPACE_NET_CRYPT_H_

#include "../Types.h"

namespace null {

struct ContinuumEncrypt {
  void Encrypt(u8* pkt, size_t size);
  void Decrypt(u8* pkt, size_t size);

  bool ExpandKey(u32 key1, u32 key2);

  // Loads the first two steps for key expansion requests
  bool LoadTable(u32* table, u32 key);

  u32 expanded_key[20] = {};
  u32 key1 = 0;
  u32 key2 = 0;
};

}  // namespace null

#endif
