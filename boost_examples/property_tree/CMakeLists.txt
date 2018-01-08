# add the executable
add_executable(debug_settings debug_settings.cpp)

add_custom_command(
        TARGET debug_settings POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/boost_examples/property_tree/debug_settings.xml
                ${CMAKE_CURRENT_BINARY_DIR}/debug_settings.xml)