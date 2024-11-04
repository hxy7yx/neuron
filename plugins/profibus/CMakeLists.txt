set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/plugins")

set(PROFIBUS_SRC profibus.c profibus_point.c profibus_req.c profibus_stack.c)

set(CMAKE_BUILD_RPATH ./)
file(COPY ${CMAKE_SOURCE_DIR}/plugins/profibus/profibus.json DESTINATION ${CMAKE_BINARY_DIR}/plugins/schema/)

set(PROFIBUS_PLUGIN plugin-profibus)
set(PROFIBUS_PLUGIN_SOURCES  profibus.c
                               ${PROFIBUS_SRC})
add_library(${PROFIBUS_PLUGIN} SHARED)
target_include_directories(${PROFIBUS_PLUGIN} PRIVATE ${CMAKE_SOURCE_DIR}/include/neuron)
target_sources(${PROFIBUS_PLUGIN} PRIVATE ${PROFIBUS_PLUGIN_SOURCES})
target_link_libraries(${PROFIBUS_PLUGIN} neuron-base)