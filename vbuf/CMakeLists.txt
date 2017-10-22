add_executable (vbuf src/vbuf.c)
target_link_libraries (vbuf LINK_PUBLIC vrb_static)
set_target_properties(vbuf PROPERTIES VERSION ${PROJECT_VERSION})

install (
        TARGETS vbuf
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
