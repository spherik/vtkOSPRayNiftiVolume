cmake_minimum_required(VERSION 2.8)

project(vtkNiiVolume)

# Find VTK.
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set(CV_TFM_CONVEX_HULL_SRC main.cxx)

add_executable(vtkNiiVolume ${CV_TFM_CONVEX_HULL_SRC})

target_link_libraries(vtkNiiVolume ${VTK_LIBRARIES})
