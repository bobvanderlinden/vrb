cmake_minimum_required (VERSION 3.0)
project (vrb)

include(GNUInstallDirs)
find_package(PkgConfig)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 5)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

file(GLOB SRCS vrb/src/lib/vrb/*.c)
file(GLOB MANFILES vrb/man/man3/*.3)

add_library (vrb_shared SHARED ${SRCS})
target_include_directories (vrb_shared PUBLIC include)
set_target_properties (vrb_shared PROPERTIES OUTPUT_NAME vrb)
set_target_properties(vrb_shared PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

add_library (vrb_static STATIC ${SRCS})
target_include_directories (vrb_static PUBLIC include)
set_target_properties (vrb_static PROPERTIES OUTPUT_NAME vrb)
set_target_properties(vrb_static PROPERTIES VERSION ${PROJECT_VERSION} ARVERSION ${PROJECT_VERSION})

add_executable (vbuf vrb/src/bin/vbuf.c)
target_link_libraries (vbuf LINK_PUBLIC vrb_static)
set_target_properties(vbuf PROPERTIES VERSION ${PROJECT_VERSION})

set(PKG_CONFIG_LIBDIR
    "\${prefix}/lib"
)
set(PKG_CONFIG_INCLUDEDIR
    "\${prefix}/include"
)
set(PKG_CONFIG_LIBS
    "-L\${libdir} -lvrb"
)
set(PKG_CONFIG_CFLAGS
    "-I\${includedir}"
)
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/pkg-config.pc.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
)

install (
	TARGETS vbuf vrb_shared vrb_static
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install (FILES ${MANFILES} DESTINATION ${CMAKE_INSTALL_MANDIR}/man3 COMPONENT doc)
install (FILES include/vrb.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT headers)
install (FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION lib/pkgconfig)

