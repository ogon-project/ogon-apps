set(MODULE_NAME "ogon-message")

include(FindQT5)
include(CMakePackageConfigHelpers)
find_package(Qt5LinguistTools REQUIRED)

set(OGON_DEPENDENCY_TYPE "REQUIRED")
set(OGON_DEPENDENCY_PURPOSE "ogon libraries and headers")
set(OGON_DEPENDENCY_DESCRIPTION "OGON")
set(OGON_DEPENDENCY_VERSION "1")
find_dependency(ogon ${OGON_DEPENDENCY_TYPE} ${OGON_DEPENDENCY_PURPOSE} ${OGON_DEPENDENCY_DESCRIPTION} ${OGON_DEPENDENCY_VERSION})

set(OGON_APPS_MESSAGE_VERSION_FULL "1.0.0")
set(OGON_APPS_MESSAGE_VERSION_MAJOR "1")

set(${MODULE_NAME}_SRCS
	ogon-message.cpp
)

set(${MODULE_NAME}_HEADERS
	include/ogon-message/ogon-message.h
)

QT5_WRAP_CPP(${MODULE_NAME}_HEADERS_MOC ${${MODULE_NAME}_HEADERS})

if ( NOT TRANSLATIONS_DIR )
	set( TRANSLATIONS_DIR ${CMAKE_INSTALL_PREFIX}/share/qt5/translations )
endif ( NOT TRANSLATIONS_DIR )

message (STATUS "Translations directory set to: ${TRANSLATIONS_DIR}")
add_definitions(-DTRANSLATIONS_DIR="${TRANSLATIONS_DIR}")

SET(TRANS
	ogon-message_de.ts
	ogon-message_en.ts
)

qt5_add_translation(QM ${TRANS})

add_library(${MODULE_NAME} SHARED ${${MODULE_NAME}_SRCS} ${QM} ${${MODULE_NAME}_HEADERS_MOC})
include_directories(${WinPR_INCLUDE_DIR} ${OGON_INCLUDE_DIR} "include")

set_target_properties(${MODULE_NAME} PROPERTIES VERSION ${OGON_APPS_MESSAGE_VERSION_FULL} SOVERSION ${OGON_APPS_MESSAGE_VERSION_MAJOR})

target_link_libraries(${MODULE_NAME} ${QT_LIBRARIES} winpr)

install(TARGETS ${MODULE_NAME} DESTINATION "${CMAKE_INSTALL_LIBDIR}" EXPORT ogon-message)
install(FILES ${QM} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/qt5/translations)

file(GLOB OGON_APPS_HEADERS "include/ogon-message/*.h")
install(FILES ${OGON_APPS_HEADERS} DESTINATION include/ogon-message${OGON_APPS_MESSAGE_VERSION_MAJOR}/ogon-message COMPONENT headers)


if(${CMAKE_VERSION} VERSION_GREATER "2.8.10")

	export(PACKAGE ogon-message)

	set(OGON_MESSAGE_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/ogon-message${OGON_APPS_MESSAGE_VERSION_MAJOR}")
	set(OGON_MESSAGE_INCLUDE_DIR "include/ogon-message${OGON_APPS_MESSAGE_VERSION_MAJOR}")

	configure_package_config_file(ogon-messageConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/ogon-messageConfig.cmake
		INSTALL_DESTINATION ${OGON_MESSAGE_CMAKE_INSTALL_DIR}
		PATH_VARS OGON_MESSAGE_INCLUDE_DIR)

	write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/ogon-messageConfigVersion.cmake
		VERSION ${OGON_APPS_MESSAGE_VERSION_FULL} COMPATIBILITY SameMajorVersion)

	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ogon-messageConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/ogon-messageConfigVersion.cmake
		DESTINATION ${OGON_MESSAGE_CMAKE_INSTALL_DIR})

	install(EXPORT ogon-message DESTINATION ${OGON_MESSAGE_CMAKE_INSTALL_DIR})

endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ogon-message.pc.in ${CMAKE_CURRENT_BINARY_DIR}/ogon-message${OGON_APPS_MESSAGE_VERSION_MAJOR}.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ogon-message${OGON_APPS_MESSAGE_VERSION_MAJOR}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
