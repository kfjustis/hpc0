set (SRCS
    dparser.cpp
  )

set (HEADERS
    dparser.h
  )

add_library(dparser SHARED ${SRCS} ${HEADERS})
