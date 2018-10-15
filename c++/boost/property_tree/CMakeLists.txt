# add the executable
add_executable(debug_settings debug_settings.cpp)

add_executable(custom_data_type custom_data_type.cpp)

add_executable(json json.cpp)

add_custom_command(
        TARGET debug_settings POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/debug_settings.xml
                ${CMAKE_CURRENT_BINARY_DIR}/debug_settings.xml)