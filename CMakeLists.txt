cmake_minimum_required (VERSION 3.8)
project(wmdb)

if(WIN32)

    add_compile_options("/std:c++17")
	add_compile_options("/permissive-")

else()

    add_compile_options("-std=c++17")

endif() 

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(MySql)
find_package(MINI_TEST)

if(MYSQL_FOUND AND MINI_TEST_FOUND )

	add_executable (${PROJECT_NAME} main.cpp Connect.cpp Drive.cpp)

	message(ss  ${MYSQL_INCLUDE_DIR}   ${MYSQL_LIBRARYS})

	target_include_directories(${PROJECT_NAME} PRIVATE ${MYSQL_INCLUDE_DIR} ${MINI_TEST_INCLUDE_DIR} )
	target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBRARYS})

endif()