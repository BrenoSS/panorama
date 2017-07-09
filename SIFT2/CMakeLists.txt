
cmake_minimum_required( VERSION 2.8 )

find_package( OpenCV REQUIRED )
if( OpenCV_FOUND )
	include_directories( ${OpenCV_INCLUDE_DIR} )
endif( OpenCV_FOUND )

set( ProgramName "main" )

set( SOURCES 
	main.cpp
)

add_executable( ${ProgramName} ${SOURCES} )

target_link_libraries( ${ProgramName}
	${OpenCV_LIBS}
)
