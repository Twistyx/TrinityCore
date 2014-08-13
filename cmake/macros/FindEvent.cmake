# find Event (Event loop / http) includes and library
#
# EVENT_INCLUDE_DIR - where the directory containing the EVENT headers can be found
# EVENT_LIBRARY     - full path to the EVENT library
# EVENT_FOUND       - TRUE if EVENT was found

FIND_PATH(EVENT_INCLUDE_DIR event2/event.h)
FIND_LIBRARY(EVENT_LIBRARY NAMES event)

IF (EVENT_INCLUDE_DIR AND EVENT_LIBRARY)
    SET(EVENT_FOUND TRUE)
    MESSAGE(STATUS "Found Event library: ${EVENT_LIBRARY}")
    MESSAGE(STATUS "Include dir is: ${EVENT_INCLUDE_DIR}")
    INCLUDE_DIRECTORIES(${EVENT_INCLUDE_DIR})
ELSE (EVENT_INCLUDE_DIR AND EVENT_LIBRARY)
    SET(EVENT_FOUND FALSE)
    MESSAGE(FATAL_ERROR "** Event library not found!\n** Your distro may provide a binary for Event e.g. for ubuntu try apt-get install libevent-dev")
ENDIF (EVENT_INCLUDE_DIR AND EVENT_LIBRARY)
