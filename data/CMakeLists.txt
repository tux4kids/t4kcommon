include (T4K_Sugar)

t4k_gentle_set(DESTDIR "${T4K_COMMON_DATA_PREFIX}")

install (DIRECTORY images
    DESTINATION ${DESTDIR}
    PATTERN Makefile* EXCLUDE
    PATTERN .svn EXCLUDE
    PATTERN CMakeLists.txt EXCLUDE
    PATTERN *~ EXCLUDE )
