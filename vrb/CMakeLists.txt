file(GLOB MANFILES man/man3/*.3)
file(GLOB SRCS src/*.c)

add_library (vrb_shared SHARED ${SRCS})
target_include_directories (vrb_shared PUBLIC include)
set_target_properties (vrb_shared PROPERTIES OUTPUT_NAME vrb)
set_target_properties(vrb_shared PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

add_library (vrb_static STATIC ${SRCS})
target_include_directories (vrb_static PUBLIC include)
set_target_properties (vrb_static PROPERTIES OUTPUT_NAME vrb)
set_target_properties(vrb_static PROPERTIES VERSION ${PROJECT_VERSION} ARVERSION ${PROJECT_VERSION})

install (
        TARGETS vrb_shared vrb_static
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

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

install (FILES ${MANFILES} DESTINATION ${CMAKE_INSTALL_MANDIR}/man3 COMPONENT doc)
install (FILES include/vrb.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT headers)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION lib/pkgconfig)
