project(Algorithm)
#ITK
find_package(ITK REQUIRED)
if (NOT ITK_FOUND)
	message(FATAL_ERROR "error: ITK is not found")
endif()
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

add_subdirectory(LumenExtraction)
add_subdirectory(PolylineToTubularVolume)
add_subdirectory(LumenSegmentationFilter)
add_subdirectory(ReorderPointIdOfContourFilter)
add_subdirectory(MaximumWallThickness)
add_subdirectory(VesselWallHoughTransformCircles)
add_subdirectory(MagneticContourLineInterpolator)
add_subdirectory(ImageSliceBySliceAccumulate)
add_subdirectory(ContourInterpolateFilter)