set(MOD_NAME "BOOST")

message( "finding ${MOD_NAME}!"  )

    set(BOOST_PATH $ENV{BOOST_PATH})
    if( BOOST_PATH )

        message("Find BOOST_PATH env!")
        message(${BOOST_PATH})

        find_path( BOOST_INCLUDE_DIR boost "${BOOST_PATH}" )
		

        if( BOOST_INCLUDE_DIR)

            set( BOOST_FOUND TRUE )

        else()

            set( BOOST_FOUND FALSE )

        endif()

    else()

        set( BOOST_FOUND FALSE )
        message("Not Find BOOST_PATH env!")

    endif()
message("................................................................")