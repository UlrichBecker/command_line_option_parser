cmake_minimum_required(VERSION 3.20)

project(command_line_option_parser VERSION 1.0.0 LANGUAGES C)

# omit static/shared to leave that decision to the invoking process
add_library(command_line_option_parser ${CMAKE_CURRENT_SOURCE_DIR}/src/parse_opts.c)
add_library(command_line_option_parser::command_line_option_parser ALIAS command_line_option_parser)

include(GNUInstallDirs)

target_include_directories(command_line_option_parser 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/command_line_option_parser>
)


# Install the library
install(TARGETS command_line_option_parser
    EXPORT command_line_option_parserTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/command_line_option_parser
)

# Install headers
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/parse_opts.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/command_line_option_parser
)

# Export target configuration
install(EXPORT command_line_option_parserTargets
    FILE command_line_option_parserTargets.cmake
    NAMESPACE command_line_option_parser::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/command_line_option_parser
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/command_line_option_parserConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/command_line_option_parser
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/command_line_option_parserConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/command_line_option_parser
)