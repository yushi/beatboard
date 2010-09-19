#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(beatboard, 1, yushi.nakai@gmail.com)
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_PROG_LIBTOOL
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for third-party software
AC_CHECK_HEADERS(json/json.h, break,
                         AC_MSG_ERROR([json-c is required for beatboard to build]))

AC_CHECK_HEADERS(event.h, break,
                         AC_MSG_ERROR([libevent is required for beatboard to build]))

AC_CHECK_HEADERS(libmemcached/memcached.h, break,
                         AC_MSG_ERROR([libmemcached is required for beatboard to build]))

AC_CHECK_HEADERS(libdrizzle/drizzle_client.h, break,
                         AC_MSG_ERROR([libmemcached is required for beatboard to build]))

AC_CHECK_HEADERS(uuid/uuid.h, break,
                         AC_MSG_ERROR([uuid is required for beatboard to build]))

AC_PATH_TOOL(PROTOC, protoc)
AS_IF([test "$PROTOC" = ""],
  AC_MSG_ERROR(["protobuf is required for beatboard to build"]))

AC_PATH_TOOL(RAGEL, ragel)
AS_IF([test "$RAGEL" = ""],
  AC_MSG_ERROR(["ragel is required for beatboard to build"]))

# Checks for programs.
AC_PROG_CXX
#AC_PROG_CC
#AC_PROG_INSTALL
#AC_PROG_MAKE_SET

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lapicommon':
#AC_CHECK_LIB([apicommon], [main])
# FIXME: Replace `main' with a function in `-lbbdrizzle':
#AC_CHECK_LIB([bbdrizzle], [main])
# FIXME: Replace `main' with a function in `-lbbevqueue':
#AC_CHECK_LIB([bbevqueue], [main])
# FIXME: Replace `main' with a function in `-lbbirc':
#AC_CHECK_LIB([bbirc], [main])
# FIXME: Replace `main' with a function in `-lbbrpc':
#AC_CHECK_LIB([bbrpc], [main])
# FIXME: Replace `main' with a function in `-ldrizzle':
#AC_CHECK_LIB([drizzle], [main])
# FIXME: Replace `main' with a function in `-levent':
#AC_CHECK_LIB([event], [main])
# FIXME: Replace `main' with a function in `-lgtest':
#AC_CHECK_LIB([gtest], [main])
# FIXME: Replace `main' with a function in `-ljson':
#AC_CHECK_LIB([json], [main])
# FIXME: Replace `main' with a function in `-lmemcached':
#AC_CHECK_LIB([memcached], [main])
# FIXME: Replace `main' with a function in `-lprotobuf':
#AC_CHECK_LIB([protobuf], [main])
# FIXME: Replace `main' with a function in `-lpthread':
#AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lpython':
#AC_CHECK_LIB([python], [main])
# FIXME: Replace `main' with a function in `-luuid':
#AC_CHECK_LIB([uuid], [main])

# Checks for header files.
#AC_HEADER_STDC
#AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
#AC_HEADER_STDBOOL
#AC_C_CONST
#AC_C_INLINE
#AC_TYPE_SIZE_T
#AC_TYPE_SSIZE_T
#AC_HEADER_TIME
#AC_STRUCT_TM
#AC_TYPE_UINT32_T
#AC_TYPE_UINT64_T

# Checks for library functions.
#AC_FUNC_ERROR_AT_LINE
#AC_FUNC_FORK
#AC_FUNC_MALLOC
#AC_FUNC_SELECT_ARGTYPES
#AC_TYPE_SIGNAL
#AC_FUNC_STRFTIME
#AC_CHECK_FUNCS([bzero gettimeofday memset select socket strerror])

AC_CONFIG_FILES([Makefile
                   src/libbbevqueue/Makefile
                   src/libapicommon/Makefile
                   src/libbbdrizzle/Makefile
                   src/libbbrpc/Makefile
                   src/logapi/Makefile
                   src/libbbirc/Makefile
                   src/bbircd/Makefile
                   src/searchapi/Makefile
                ])

#AC_CONFIG_FILES([Makefile
#                 etc/Makefile
#                 service/Makefile
#                 src/authapi/Makefile
#                 src/bbircd/Makefile
#                 src/bbui/Makefile
#                 src/libapicommon/Makefile
#                 src/libapicommon/tests/Makefile
#                 src/libbbdrizzle/Makefile
#                 src/libbbdrizzle/tests/Makefile
#                 src/libbbevqueue/Makefile
#                 src/libbbirc/Makefile
#                 src/libbbirc/tests/Makefile
#                 src/libbbrpc/Makefile
#                 src/libbbrpc/tests/Makefile
#                 src/logapi/Makefile
#                 src/searchapi/Makefile])
AC_OUTPUT
