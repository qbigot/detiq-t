project(GenericInterface C CXX)
cmake_minimum_required(VERSION 2.8.11)

include_directories(../ImageIn)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_library(JPEG_LIB NAMES jpeg)
find_library(PNG_LIB NAMES png)

if(NOT (JPEG_LIB AND PNG_LIB))
	message(FATAL_ERROR "JPEG and PNG librarys hasn't been found. You have to install them before running the compilation of this software")
endif()

find_package(Threads REQUIRED)

find_package(Qwt 6.0 REQUIRED)
include_directories(${QWT_INCLUDE_DIRS})

# Find the Qt libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

set(GenInterface_SOURCES
	Exceptions.h
	GenericInterface.cpp
	GenericInterface.h
	InterfaceExceptions.h
	Service.h
	ServiceExceptions.h
	Services/AlgorithmService.cpp
	Services/AlgorithmService.h
	Services/FileService.cpp
	Services/FileService.h
	Services/Node.h
	Services/UtilityService.cpp
	Services/UtilityService.h
	Services/WindowService.cpp
	Services/WindowService.h
	Utilities/Log.cpp
	Utilities/Log.h
	Widgets/ImageListBox.h
	Widgets/ImageWidgets/AlternativeImageView.cpp
	Widgets/ImageWidgets/AlternativeImageView.h
	Widgets/ImageWidgets/DoubleImageWindow.cpp
	Widgets/ImageWidgets/DoubleImageWindow.h
	Widgets/ImageWidgets/GenericHistogramView.cpp
	Widgets/ImageWidgets/GenericHistogramView.h
	Widgets/ImageWidgets/GenericHistogramWindow.cpp
	Widgets/ImageWidgets/GenericHistogramWindow.h
	Widgets/ImageWidgets/GraphicalHistogram.cpp
	Widgets/ImageWidgets/GraphicalHistogram.h
	Widgets/ImageWidgets/GridView.cpp
	Widgets/ImageWidgets/GridView.h
	Widgets/ImageWidgets/GridWindow.cpp
	Widgets/ImageWidgets/GridWindow.h
	Widgets/ImageWidgets/HistogramPicker.cpp
	Widgets/ImageWidgets/HistogramPicker.h
	Widgets/ImageWidgets/HistogramView.cpp
	Widgets/ImageWidgets/HistogramView.h
	Widgets/ImageWidgets/HistogramWindow.cpp
	Widgets/ImageWidgets/HistogramWindow.h
	Widgets/ImageWidgets/ImageContextMenu.cpp
	Widgets/ImageWidgets/ImageContextMenu.h
	Widgets/ImageWidgets/ImageView.cpp
	Widgets/ImageWidgets/ImageView.h
	Widgets/ImageWidgets/ImageViewer.cpp
	Widgets/ImageWidgets/ImageViewer.h
	Widgets/ImageWidgets/ImageWidget.cpp
	Widgets/ImageWidgets/ImageWidget.h
	Widgets/ImageWidgets/ImageWindow.cpp
	Widgets/ImageWidgets/ImageWindow.h
	Widgets/ImageWidgets/PixelGrid.cpp
	Widgets/ImageWidgets/PixelGrid.h
	Widgets/ImageWidgets/ProjectionHistogramView.cpp
	Widgets/ImageWidgets/ProjectionHistogramView.h
	Widgets/ImageWidgets/ProjectionHistogramWindow.cpp
	Widgets/ImageWidgets/ProjectionHistogramWindow.h
	Widgets/ImageWidgets/RowView.cpp
	Widgets/ImageWidgets/RowView.h
	Widgets/ImageWidgets/RowWindow.cpp
	Widgets/ImageWidgets/RowWindow.h
	Widgets/ImageWidgets/StandardImageWindow.cpp
	Widgets/ImageWidgets/StandardImageWindow.h
	Widgets/ImageWidgets/ThumbnailView.cpp
	Widgets/ImageWidgets/ThumbnailView.h
	Widgets/ImageWidgets/ZoomViewer.cpp
	Widgets/ImageWidgets/ZoomViewer.h
	Widgets/NavBar/ImageDelegate.cpp
	Widgets/NavBar/ImageDelegate.h
	Widgets/NavBar/NavigationBar.cpp
	Widgets/NavBar/NavigationBar.h
	Widgets/NavBar/NavigationDock.cpp
	Widgets/NavBar/NavigationDock.h
	Widgets/NavBar/NodeListModel.cpp
	Widgets/NavBar/NodeListModel.h
)

qt5_add_resources(RCC_FILES "res.qrc")

set(TS_FILES genericinterface_en.ts genericinterface_fr.ts)
# Uncommento only 1 of the 2 folowing commands, depending on your needs
# -1- Use current translations files
qt5_add_translation(QM_FILES ${TS_FILES})
# -2- Update translations files with changes from c++ sources
# qt5_create_translation(QM_FILES ${GenInterface_SOURCES} ${TS_FILES})

add_library(GenericInterface STATIC ${GenInterface_SOURCES} ${QM_FILES} ${RCC_FILES} ${TS_FILES})
target_link_libraries(GenericInterface
	ImageIn
	Qt5::Core
	Qt5::Widgets
	Qt5::Gui
	${JPEG_LIB}
	${PNG_LIB}
	${CMAKE_THREAD_LIBS_INIT}
     ${QWT_LIBRARY}
     ${QWT_LIBRARIES}
)