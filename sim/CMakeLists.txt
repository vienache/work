include_directories(
   interface
   engine
   parser
   stubs
)

add_subdirectory (engine)
add_subdirectory (parser)
add_subdirectory (stubs)

add_executable (sim_test main.cpp)

target_link_libraries (sim_test engine parser stubs)

add_custom_command(
        TARGET sim_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/scen/language.xml
                ${CMAKE_CURRENT_BINARY_DIR}/language.xml)