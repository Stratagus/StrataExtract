# The StrataExtract cmake configuration file, it should
#be well documented. If you have any questions be it in
#design or general inquiry visit the homepage @
#https://github.com/bradc6/StrataExtract
cmake_minimum_required (VERSION 2.6)
project (StrataExtract)


#Sets the directory for cmake to find Cmake configration packages,
#these packages allow for cmake to find libraries that are not included
#in the standard installation of Cmake. The standard packages can be
#found @ /usr/share/cmake-2.8/Modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
                      ${CMAKE_MODULE_PATH})

#Search for the required libraries for the project to properly
#link and compile. Boost is absolutly essential as it controls the
#process of Threads, Filesystem, Exception Handling, Hashing and Compression
#FFMPEG is the main library for multimedia transcoding from whatever the
#inputted format maybe to OGG Vorbis(Audio) and OGG Theora(Video).
#Magick++ (C++ Bindings to ImageMagick) allows for image conversion
#from whatever the input type to png (Portable Network Graphic)
find_package(Boost REQUIRED COMPONENTS system thread filesystem)
find_package(LibXml2 REQUIRED)
find_package(FFMPEG REQUIRED)
find_package(Magick++ REQUIRED)

#Source directory variables
#These allow for folders in the source and unittesting to be moved
#without having to manually change each individual filepath, they
#also act as shortcuts.
set (SOURCE_DIR "./Source")
set (ARCHIVES_DIR ${SOURCE_DIR}/Archives)
set (CONVERSION_DIR ${SOURCE_DIR}/Conversion)
set (CONFIGURATION_DIR ${SOURCE_DIR}/Configuration)
set (AUDIOCONVERSION_DIR ${CONVERSION_DIR}/Audio)
set (IMAGECONVERSION_DIR ${CONVERSION_DIR}/Image)
set (VIDEOCONVERSION_DIR ${CONVERSION_DIR}/Video)
set (RUNNER_DIR ${SOURCE_DIR}/AssetManager)
set (UNITTEST_DIR "./UnitTesting")

#Set of included libraries if the user does not have
#a packaged version installed via DEB/RPM/Macports
set (LIBRARIES_DIR "./Libraries")
set (STORMLIB_DIR ${LIBRARIES_DIR}/StormLib)

#Include internal library directories if a package was not found
#grab versions included with this tool.
add_subdirectory("${STORMLIB_DIR}")

include_directories("${STORMLIB_DIR}/src/"
                    "${STORMLIB_DIR}/include/")

#Set the actual Cpp & Hpp files in which are to be included
#into this tool. The actual code of the tool, not outside libraries.
set(StrataExtractMain-Source
	${SOURCE_DIR}/main.hpp
	${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/SimpleOpt.h
	 )
set(StrataExtractConfiguration-Source
    ${CONFIGURATION_DIR}/StrataConfig.hpp
    ${CONFIGURATION_DIR}/StrataConfig.cpp
   )
set(StrataExtractRunner-Source
    ${RUNNER_DIR}/StrataRunner.hpp
    ${RUNNER_DIR}/StrataRunner.cpp
    )
set(StrataExtractArchives-Source
    ${ARCHIVES_DIR}/StrataArchive.hpp
    ${ARCHIVES_DIR}/StrataArchive.cpp
    ${ARCHIVES_DIR}/MPQ/MPQArchive.hpp
    ${ARCHIVES_DIR}/MPQ/MPQArchive.cpp
    ${ARCHIVES_DIR}/GZIP/GZIPArchive.hpp
    ${ARCHIVES_DIR}/GZIP/GZIPArchive.cpp
   )
    
set(StrataExtractAudioConversion-Source
    ${AUDIOCONVERSION_DIR}/StrataAudio.hpp
    ${AUDIOCONVERSION_DIR}/StrataAudio.cpp
    ${AUDIOCONVERSION_DIR}/Libav/AudioLibav.hpp
    ${AUDIOCONVERSION_DIR}/Libav/AudioLibav.cpp
   )
set(StrataExtractImageConversion-Source
    ${IMAGECONVERSION_DIR}/StrataImage.hpp
    ${IMAGECONVERSION_DIR}/StrataImage.cpp
    ${IMAGECONVERSION_DIR}/ImageMagick/MagickImage.hpp
    ${IMAGECONVERSION_DIR}/ImageMagick/MagickImage.cpp
   )
set(StrataExtractVideoConversion-Source
    ${VIDEOCONVERSION_DIR}/StrataVideo.hpp
    ${VIDEOCONVERSION_DIR}/StrataVideo.cpp
    ${VIDEOCONVERSION_DIR}/Libav/VideoLibav.hpp
    ${VIDEOCONVERSION_DIR}/Libav/VideoLibav.cpp
   )
set(StrataExtractGraphicalInterface-Source)


#Code groupings allow for IDEs to put files into collections
#instead of simply throwing all files into a pile. This should
#allow for easy navigation through the code base for novice and
#heavy contributors.
source_group(Main FILES ${StrataExtractMain-Source})
source_group(Configuration FILES ${StrataExtractConfiguration-Source})
source_group(Runner FILES ${StrataExtractRunner-Source})
source_group(GUI FILES ${StrataExtractGraphicalInterface})
source_group(Archives FILES ${StrataExtractArchives-Source})
source_group(Conversion FILES)
source_group(Conversion\\Audio FILES ${StrataExtractAudioConversion-Source})
source_group(Conversion\\Image FILES ${StrataExtractImageConversion-Source})
source_group(Conversion\\Video FILES ${StrataExtractVideoConversion-Source})



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
#add_library(Library)

#Links the libraries to the executable as to actually call
#the library dependancy.
link_libraries (
	  ${Boost_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    ${Magick++_LIBRARIES}
    ${FFMPEG_LIBAVCODEC_LIBRARIES}
    ${FFMPEG_LIBAVFORMAT_LIBRARIES}
    ${FFMPEG_LIBAVUTIL_LIBRARIES}
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
   ${StrataExtractConfiguration-Source}
   ${StrataExtractRunner-Source}
   ${StrataExtractGraphicalInterface-Source}
   ${StrataExtractAudioConversion-Source}
   ${StrataExtractImageConversion-Source}
   ${StrataExtractVideoConversion-Source}
   ${StrataExtractArchives-Source}
)

#Creates the target libraries to use with other Frontend applications
target_link_libraries (
	StrataExtract
  storm
	)


# Set the RPATH (ie Hack Soup for the moment)
if (APPLE)
    set_target_properties(StrataExtract PROPERTIES LINK_FLAGS "-Wl,-rpath,@loader_path/.")
elseif (UNIX)
    set_target_properties(StrataExtract PROPERTIES INSTALL_RPATH ".")
endif()
