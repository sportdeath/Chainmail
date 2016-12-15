cmake_minimum_required (VERSION 3.4)
project (sweater)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no c++11 support.")
endif()

#Adding cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/modules/)

#######################
## External Libraries
#######################

# Eigen
find_package(Eigen3 REQUIRED )
include_directories( ${Eigen3_INCLUDE_DIR} )
if(NOT Eigen3_FOUND)
  message(ERROR " Eigen3 not found!")
endif(NOT Eigen3_FOUND)

# OpenCV
find_package( OpenCV 3 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
set(LIBS ${LIBS} ${OpenCV_LIBS})
if(NOT OpenCV_FOUND)
  message(ERROR " OpenCV not found!")
endif(NOT OpenCV_FOUND)

##################
## Google Test
#################

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/googletest-master")
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(LIBS ${LIBS} gtest gtest_main)

enable_testing()

#######################
## Internal Libraries
#######################

set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Splines
set(PROJECT_SOURCES
  #${PROJECT_SOURCES}
  #${PROJECT_SOURCE_DIR}/Splines/BSplineUnit.cpp
  ${PROJECT_SOURCE_DIR}/Splines/Polynomial.cpp
  ${PROJECT_SOURCE_DIR}/Splines/PolyVector3f.cpp
)

#Physics
set(PROJECT_SOURCES
  ${PROJECT_SOURCES}
  ${PROJECT_SOURCE_DIR}/Physics/TimeStepper.cpp
)

# Objects
set(PROJECT_SOURCES
  ${PROJECT_SOURCES}
  ${PROJECT_SOURCE_DIR}/Objects/Object.cpp
  ${PROJECT_SOURCE_DIR}/Objects/ObjectGroup.cpp
  ${PROJECT_SOURCE_DIR}/Objects/Sphere.cpp
  ${PROJECT_SOURCE_DIR}/Objects/Torus.cpp
  ${PROJECT_SOURCE_DIR}/Objects/Chainmail.cpp
  ${PROJECT_SOURCE_DIR}/Objects/Plane.cpp
  #${PROJECT_SOURCE_DIR}/Objects/YarnUnit.cpp
  #${PROJECT_SOURCE_DIR}/Objects/Transform.cpp
)


# Lights
set(PROJECT_SOURCES
  ${PROJECT_SOURCES}
  ${PROJECT_SOURCE_DIR}/Lights/PointLight.cpp
  ${PROJECT_SOURCE_DIR}/Lights/SphericalLight.cpp
)

# Ray Tracing
set(PROJECT_SOURCES
  ${PROJECT_SOURCES}
  ${PROJECT_SOURCE_DIR}/RayTracing/Renderer.cpp
  ${PROJECT_SOURCE_DIR}/RayTracing/Camera.cpp
  ${PROJECT_SOURCE_DIR}/RayTracing/Material.cpp
)

# Video
set(PROJECT_SOURCES
  ${PROJECT_SOURCES}
  ${PROJECT_SOURCE_DIR}/Video/Video.cpp
  ${PROJECT_SOURCE_DIR}/Video/Image.cpp
)

# RangeTree
set(PROJECT_SOURCES
  ${PROJECT_SOURCES}
  ${PROJECT_SOURCE_DIR}/RangeTree/RTZ.cpp
  ${PROJECT_SOURCE_DIR}/RangeTree/RTY.cpp
  ${PROJECT_SOURCE_DIR}/RangeTree/RTX.cpp
  ${PROJECT_SOURCE_DIR}/RangeTree/ObjectRangeTree.cpp
)

include_directories("${PROJECT_INCLUDE_DIR}")

####################
## Library Creation
####################

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})
set(LIBS ${LIBS} ${PROJECT_NAME})


#####################################
## Unit Tests with CTest and GTest
#####################################

macro(MAKE_TEST test_name)
  add_executable(${test_name}
         test/${test_name}.cpp
         )
  target_link_libraries(${test_name} ${LIBS})
  add_test(${test_name} ${test_name})
endmacro()

MAKE_TEST(polynomial_test)
MAKE_TEST(RayTracer_test)
MAKE_TEST(collision_test)
MAKE_TEST(torus_test)
MAKE_TEST(torusRender_test)
MAKE_TEST(chainmail_test)
MAKE_TEST(chainmail_test-med)
MAKE_TEST(chainmail_test-large)
#MAKE_TEST(physics_test)
#MAKE_TEST(yarnUnit_test)
#MAKE_TEST(spline_test)
#MAKE_TEST(video_test)
MAKE_TEST(RTZ_test)
MAKE_TEST(RTY_test)
MAKE_TEST(RTX_test)

######################
## Build Scenes
######################

option(BUILD_SCENES "BUILD_SCENES" OFF)
if(BUILD_SCENES)
  add_subdirectory(scenes)
endif(BUILD_SCENES)
