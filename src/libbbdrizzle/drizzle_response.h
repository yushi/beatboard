#ifndef DRIZZLE_RESPONSE_H
#define DRIZZLE_RESPONSE_H

#include <libdrizzle/drizzle_client.h>

namespace BeatBoard {
  struct DrizzleResponse {
    drizzle_result_st result;
    drizzle_return_t ret;
  };
}

#endif DRIZZLE_RESPONSE_H
