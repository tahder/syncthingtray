// Created via CMake from template global.h.in
// WARNING! Any changes to this file will be overwritten by the next CMake run!

#ifndef LIB_SYNCTHING_CONNECTOR_GLOBAL
#define LIB_SYNCTHING_CONNECTOR_GLOBAL

#include <c++utilities/application/global.h>

#ifdef LIB_SYNCTHING_CONNECTOR_STATIC
# define LIB_SYNCTHING_CONNECTOR_EXPORT
# define LIB_SYNCTHING_CONNECTOR_IMPORT
#else
# define LIB_SYNCTHING_CONNECTOR_EXPORT LIB_EXPORT
# define LIB_SYNCTHING_CONNECTOR_IMPORT LIB_IMPORT
#endif

#endif // LIB_SYNCTHING_CONNECTOR_GLOBAL
