set(MODULE_NAME "ogon-shadow")

include(FindQT5)

set(${MODULE_NAME}_SRCS
	workerthread.cpp
	sessionmodel.cpp
	mainwindow.cpp
	main.cpp
)

add_executable(${MODULE_NAME} ${${MODULE_NAME}_SRCS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QT_EXECUTEABLE_FLAGS}")
include_directories(${WinPR_INCLUDE_DIR})

target_link_libraries(${MODULE_NAME} ${QT_LIBRARIES} winpr)

install(TARGETS ${MODULE_NAME} DESTINATION "${CMAKE_INSTALL_BINDIR}")

