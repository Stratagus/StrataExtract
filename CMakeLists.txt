# StrataExtract
cmake_minimum_required (VERSION 2.6)
project (StrataExtract)


#Sets the directory for cmake to search for packages
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

#Source directory variable
set (SOURCE_DIR "./Source")
set (UNITTEST_DIR "./UnitTesting")

# Recurse into the "Hello" and "Demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
#add_subdirectory (Hello)

#List out the different source files
set(StrataExtractMain-Source
	${SOURCE_DIR}/main.cpp
	)

#Simplify Source Code Groupings
    source_group(Main FILES ${StrataExtractMain-Source})


#Find required libraries for the project
find_package(Boost REQUIRED)
find_package(FFMPEG REQUIRED)
find_package(Magick++ REQUIRED)


# on OS X we also have to add '-framework Cocoa' as library.  This is
# actually a bit of an hack but it's easy enough and reliable. (we can
#also add other OS specific flags though the EXTRA_Libs var.
set(EXTRA_LIBS "")
if (APPLE)
    set(EXTRA_LIBS ${EXTRA_LIBS} "-framework Cocoa")
    include_directories("/opt/local/include")
    link_directories("/opt/local/lib")

endif()

#Compile the Library for X use
#add_library(Library
#			
#			)

#Allows us to link dependancies to StrataExtract
link_libraries (
	boost_system-mt
	boost_filesystem-mt
)

# Now we define what makes our executable.  First thing is the name,
# WIN32 is needed to make this a Win32 GUI application, MACOSX_BUNDLE
# activates bundle mode on OS X and the last two things are our source
# and header files this executable consists of.
#add_executable(
#    WIN32
#    MACOSX_BUNDLE
#)

add_executable(
	StrataExtract
    WIN32
    #MACOSX_BUNDLE
   ${StrataExtractMain-Source}
)

#Creates the target libraries to use with other Frontend applications
#target_link_libraries (
#	StrataExtract
#	)

