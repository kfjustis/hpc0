# include dparser
include_directories(../dparser)
link_directories(../dparser)

set (SRCS
    main.cpp
  )
set (HEADERS
  )

add_executable(${CMAKE_PROJECT_NAME} ${SRCS} ${HEADERS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE dparser)

# move binary to top level of directory
add_custom_command(TARGET ${CMAKE_PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_PROJECT_NAME} ${PROJECT_BINARY_DIR}/..
)
