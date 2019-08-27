# add the executable
add_executable(boost_regex_match regex_match.cpp)
target_link_libraries(boost_regex_match boost_regex)

add_executable(boost_regex_search regex_search.cpp)
target_link_libraries(boost_regex_search boost_regex)