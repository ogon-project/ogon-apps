set(MODULE_NAME "ogon-message-bin")

include(FindQT5)

set(${MODULE_NAME}_SRCS
	qogonmessage.cpp
	main.cpp
)

add_executable(${MODULE_NAME} ${${MODULE_NAME}_SRCS})
set_target_properties(ogon-message-bin PROPERTIES OUTPUT_NAME ogon-message)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QT_EXECUTEABLE_FLAGS}")
include_directories(${WinPR_INCLUDE_DIR})
include_directories("${CMAKE_SOURCE_DIR}/libs/ogon-message/include")

target_link_libraries(${MODULE_NAME} ${QT_LIBRARIES} ogon-message)

install(TARGETS ${MODULE_NAME} DESTINATION "${CMAKE_INSTALL_BINDIR}")

