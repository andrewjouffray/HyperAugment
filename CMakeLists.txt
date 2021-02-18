cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project( maintest )

find_package(OpenMP REQUIRED)
find_package( OpenCV REQUIRED )
add_library(other ./src/randomFunc.cpp ./src/Ooi.cpp ./src/Canvas.cpp)
add_library(tinyxml ./vendors/tinyxml/tinyxml.cpp ./vendors/tinyxml/tinyxmlerror.cpp ./vendors/tinyxml/tinyxmlparser.cpp ./vendors/tinyxml/tinystr.cpp)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( maintest src/MainTest.cpp )
if(OpenMP_CXX_FOUND)
	target_link_libraries(maintest PUBLIC OpenMP::OpenMP_CXX ${OpenCV_LIBS} other tinyxml)
endif()
