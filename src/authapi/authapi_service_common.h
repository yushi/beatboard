#ifndef AUTHAPISERVICECOMMON_H
#define AUTHAPISERVICECOMMON_H

#include <errno.h>

namespace BeatBoard {
  extern unsigned int AUTHAPI_ACCOUNT_INSERT_OK;
  extern unsigned int AUTHAPI_ACCOUNT_EXIST;
  extern unsigned int AUTHAPI_VERIFY_OK;
  extern unsigned int AUTHAPI_VERIFY_ERROR;
  extern unsigned int AUTHAPI_DB_ERROR;

  extern unsigned int AUTHAPI_ADD_USER;
  extern unsigned int AUTHAPI_VERIFY_USER;
}

#endif
