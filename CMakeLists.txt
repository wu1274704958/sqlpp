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

set(LIB_NAME "sqlpp")

if(MYSQL_FOUND AND MINI_TEST_FOUND )

	add_library(${LIB_NAME} STATIC Connect.cpp Drive.cpp)
	target_include_directories(${LIB_NAME} PRIVATE ${MYSQL_INCLUDE_DIR} ${MINI_TEST_INCLUDE_DIR} )
	target_link_libraries(${LIB_NAME} ${MYSQL_LIBRARYS})

	message("msg ${CMAKE_SOURCE_DIR}/build/x64-${CMAKE_BUILD_TYPE}")

	find_library(SQL_PP_LIB sqlpp "${CMAKE_SOURCE_DIR}/build/x64-${CMAKE_BUILD_TYPE}")

	message("msg ${SQL_PP_LIB}")

	message("msg  ${MYSQL_INCLUDE_DIR}   ${MYSQL_LIBRARYS}")

	add_executable(${PROJECT_NAME} main.cpp)
	target_include_directories(${PROJECT_NAME} PRIVATE ${MYSQL_INCLUDE_DIR} ${MINI_TEST_INCLUDE_DIR} )


	if(SQL_PP_LIB)
		
		add_dependencies(${PROJECT_NAME} ${LIB_NAME})
		target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBRARYS} ${SQL_PP_LIB})
	endif()

endif()