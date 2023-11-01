if(NOT CMAKE_BUILD_EARLY_EXPANSION)

    file(STRINGS "src/filelist.txt" UISRCS)
    list(TRANSFORM UISRCS PREPEND "src/")

endif()

idf_component_register(
    SRCS ${UISRCS}
    INCLUDE_DIRS "src"
    REQUIRES lvgl
)