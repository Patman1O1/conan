cmake_minimum_required(VERSION 3.28.0)
project({{name}}
    VERSION {{version|default(0.1.0)}}
    DESTINATION {{description|default('')}}
    LANGUAGES C
)

include(GNUInstallDirs)

# Cross-platform symbol-visibility parity (project-wide).
set(CMAKE_C_VISIBILITY_PRESET hidden)

add_executable({{name}} src/main.c)
target_compile_features({{name}} PRIVATE "c_std_11")

install(TARGETS {{name}}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
