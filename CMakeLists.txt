# The StrataExtract cmake configuration file, it should
#be well documented. If you have any questions be it in
#design or general inquiry visit the homepage @
#https://github.com/bradc6/StrataExtract
cmake_minimum_required (VERSION 2.6)
project (StrataExtract)
set(StrataExtractVersion "0.1")


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
find_package(Boost REQUIRED COMPONENTS system date_time thread filesystem exception log)
find_package(LibXml2 REQUIRED)
find_package(FFMPEG REQUIRED)
find_package(Magick++ REQUIRED)
find_package(OpenSSL)
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

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

#UnitTest Directories
set (UNITTEST_DIR "./UnitTests")
set (ARCHIVES_UNITTESTS_DIR ${UNITTEST_DIR}/Archives)
set (CONVERSION_UNITTESTS_DIR ${UNITTEST_DIR}/Conversion)
set (CONFIGURATION_UNITTESTS_DIR ${UNITTEST_DIR}/Configuration)
set (AUDIOCONVERSION_UNITTESTS_DIR ${CONVERSION_UNITTESTS_DIR}/Audio)
set (IMAGECONVERSION_UNITTESTS_DIR ${CONVERSION_UNITTESTS_DIR}/Image)
set (VIDEOCONVERSION_UNITTESTS_DIR ${CONVERSION_DIR}/Video)
set (RUNNER_UNITTESTS_DIR ${UNITTEST_DIR}/AssetManager)

#GUI Directories
set (GUI_DIR ${SOURCE_DIR}/GUIs)
set (GUI_QUARTZ_DIR ${SOURCE_DIR}/GUIs/Quartz)
set (GUI_GTKMM_DIR ${SOURCE_DIR}/GUIs/GTKMM)

#Media Directories
set (MEDIA_DIR "./Media")

#Set of included libraries if the user does not have
#a packaged version installed via DEB/RPM/Macports
set (LIBRARIES_DIR "./Libraries")
set (STORMLIB_DIR ${LIBRARIES_DIR}/StormLib)
set (FFMPEG_DIR ${LIBRARIES_DIR}/FFmpeg)

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
    ${CONFIGURATION_DIR}/StrataConfigException.hpp
    ${CONFIGURATION_DIR}/StrataConfigException.cpp
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
    ${AUDIOCONVERSION_DIR}/FFmpeg/AudioFFmpeg.hpp
    ${AUDIOCONVERSION_DIR}/FFmpeg/AudioFFmpeg.cpp
   )
set(StrataExtractImageConversion-Source
    ${IMAGECONVERSION_DIR}/StrataImage.hpp
    ${IMAGECONVERSION_DIR}/StrataImage.cpp
    ${IMAGECONVERSION_DIR}/ImageMagick/MagickImage.hpp
    ${IMAGECONVERSION_DIR}/ImageMagick/MagickImage.cpp
    ${IMAGECONVERSION_DIR}/GRPImage/GRPImage.hpp
    ${IMAGECONVERSION_DIR}/GRPImage/GRPImage.cpp
   )
set(StrataExtractVideoConversion-Source
    ${VIDEOCONVERSION_DIR}/StrataVideo.hpp
    ${VIDEOCONVERSION_DIR}/StrataVideo.cpp
    ${VIDEOCONVERSION_DIR}/FFmpeg/VideoFFmpeg.hpp
    ${VIDEOCONVERSION_DIR}/FFmpeg/VideoFFmpeg.cpp
   )
set(StrataExtractGraphicalInterface-Source)


#UnitTest hpp/cpp files
set(StrataExtractMain-UnitTests-Source
  ${UNITTEST_DIR}/mainUnitTests.hpp
  ${UNITTEST_DIR}/mainUnitTests.cpp
   )
set(StrataExtractConfiguration-UnitTests-Source
    ${CONFIGURATION_UNITTESTS_DIR}/StrataConfigUnitTests.hpp
    ${CONFIGURATION_UNITTESTS_DIR}/StrataConfigUnitTests.cpp
   )
set(StrataExtractArchives-UnitTests-Source
    ${ARCHIVES_UNITTESTS_DIR}/MPQ/MPQArchiveUnitTests.hpp
    ${ARCHIVES_UNITTESTS_DIR}/MPQ/MPQArchiveUnitTests.cpp
   )
set(StrataExtractImageConversion-UnitTests-Source
    ${IMAGECONVERSION_UNITTESTS_DIR}/ImageMagick/MagickImageUnitTests.hpp
    ${IMAGECONVERSION_UNITTESTS_DIR}/ImageMagick/MagickImageUnitTests.cpp
   )


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

#UnitTests groupings
source_group(Main FILES ${StrataExtractMain-UnitTests-Source})
source_group(ConfigurationUnitTests FILES ${StrataExtractConfiguration-UnitTests-Source})
source_group(ArchivesUnitTests\\MPQArchiveUnitTests FILES ${StrataExtractArchives-UnitTests-Source})
source_group(ConversionUnitTests\\ImageUnitTests FILES ${StrataExtractImageConversion-UnitTests-Source})


# on OS X we also have to add '-framework Cocoa' as library.  This is
# actually a bit of an hack but it's easy enough and reliable. (we can
#also add other OS specific flags though the EXTRA_Libs var.
set(EXTRA_LIBS "")
if (APPLE)
    set(EXTRA_LIBS ${EXTRA_LIBS} "-framework Cocoa")
    include_directories("/opt/local/include")
    include_directories("/opt/local/include/libxml2")
    include_directories("/opt/local/include/ImageMagick")
    include_directories("/opt/local/include/ImageMagick-6")
    link_directories("/opt/local/lib")
else()
    include_directories("/usr/include/libxml2")
    include_directories("/usr/include/ImageMagick")
endif()

    #Temporary Includes
    include_directories("~/dev/fakeroot/include")
    link_directories("~/dev/fakeroot/lib")
#Compile the Library for X use
#add_library(Library)



# Now we define what makes our executable.  First thing is the name,
# WIN32 is needed to make this a Win32 GUI application, MACOSX_BUNDLE
# activates bundle mode on OS X and the last two things are our source
# and header files this executable consists of.
#add_executable(
#    WIN32
#    MACOSX_BUNDLE
#)
if(OPENSSL_FOUND)
  add_definitions( -DOPENSSL_INSTALLED=1 )
endif()

add_library(StrataExtractCore SHARED 
    ${StrataExtractConfiguration-Source}
    ${StrataExtractRunner-Source}
    ${StrataExtractGraphicalInterface-Source}
    ${StrataExtractAudioConversion-Source}
    ${StrataExtractImageConversion-Source}
    ${StrataExtractVideoConversion-Source}
    ${StrataExtractArchives-Source}
            )

target_link_libraries(
            StrataExtractCore
            ${Boost_LIBRARIES}
            ${LIBXML2_LIBRARIES}
            ${Magick++_LIBRARIES}
            ${FFMPEG_LIBAVCODEC_LIBRARIES}
            ${FFMPEG_LIBAVFORMAT_LIBRARIES}
            ${FFMPEG_LIBAVUTIL_LIBRARIES}
            ${FFMPEG_LIBSWRESAMPLE_LIBRARIES}
            ${OPENSSL_LIBRARIES}
            storm
)

add_executable(
	StrataExtractCLI
    WIN32
    #MACOSX_BUNDLE
   ${StrataExtractMain-Source}
)

#Creates the target libraries to use with other Frontend applications
target_link_libraries (
	StrataExtractCLI
  StrataExtractCore
  ${Boost_LIBRARIES}
	)

#Unittest configuration settings
option(UNITTESTS "Compile Unittests" OFF)
if(UNITTESTS)
  #Search for the required libraries for the project to properly
  #link and compile.
  find_package(Boost REQUIRED COMPONENTS unit_test_framework)

  add_executable(
    StrataExtractUnitTests
    WIN32
    #MACOSX_BUNDLE 
    #The UnitTests for functionality
    ${StrataExtractMain-UnitTests-Source}
    ${StrataExtractConfiguration-UnitTests-Source}
    ${StrataExtractArchives-UnitTests-Source}
    ${StrataExtractImageConversion-UnitTests-Source}
    )
  target_link_libraries(StrataExtractUnitTests
                        StrataExtractCore
                        ${Boost_LIBRARIES}
                        )
endif()

# Set the RPATH (ie Hack Soup for the moment)
#if (APPLE)
#    set_target_properties(StrataExtractCLI PROPERTIES LINK_FLAGS "-Wl,-rpath,@loader_path/.")
#elseif (UNIX)
#    set_target_properties(StrataExtractCLI PROPERTIES INSTALL_RPATH ".")
#endif()

#GUI Specific Compilation Processes
#Quartz OSX GUI Target
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(Quartz-Source
    ${GUI_QUARTZ_DIR}/AppDelegate.h
    ${GUI_QUARTZ_DIR}/AppDelegate.mm
    ${GUI_QUARTZ_DIR}/main.mm
)

set(Quartz-Resources
  "./Credits.rtf"
  #${MEDIA_DIR}/Icons/StrataExtract.icns
)

set_source_files_properties(
  ${Quartz-Resources}
  PROPERTIES
  MACOSX_PACKAGE_LOCATION Resources
)
#The XIB file locations 
set(Quartz-Xibs-Source
  ${GUI_QUARTZ_DIR}/Xibs/Main.xib
  )

#Set the XIB Files for compile
set(Quartz-Xibs
    Main
)

# Set the OS X Bundle specific CMake variables which will be used to populate the plist for
# the application bundle
set(MACOSX_BUNDLE_INFO_STRING "StrataExtract")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "net.codeprogrammers.StrataExtract")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version ${STARTOOLVERSION_VERSION}")
set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${StrataExtractVersion})
set(MACOSX_BUNDLE_BUNDLE_VERSION ${StrataExtractVersion})
set(MACOSX_BUNDLE_COPYRIGHT "The StrataExtract Team")

#UnitTests groupings
source_group(Main FILES ${Quartz-Source})
source_group(Resources FILES ${Quartz-Resources})
source_group(Xibs FILES ${Quartz-Xibs-Source})

add_executable(StrataExtractOSX MACOSX_BUNDLE ${Quartz-Source} ${Quartz-Resources} ${Quartz-Xibs-Source})

# Probably a better way to set the framework link libraries.
target_link_libraries(StrataExtractOSX "-framework Cocoa")

# Set a custom plist file for the app bundle
set_target_properties(StrataExtractOSX PROPERTIES MACOSX_BUNDLE_INFO_PLIST .${GUI_QUARTZ_DIR}/StrataExtract-Info.plist)


# Make sure we can find the 'ibtool' program. If we can NOT find it we
# skip generation of this project
find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
if (${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
  message(SEND_ERROR "ibtool can not be found and is needed to compile the .xib files. It should have been installed with 
                    the Apple developer tools. The default system paths were searched in addition to ${OSX_DEVELOPER_ROOT}/usr/bin")
endif()

# Make sure the 'Resources' Directory is correctly created before we build
add_custom_command (TARGET StrataExtractOSX PRE_BUILD
                      COMMAND mkdir -p .${EXECUTABLE_OUTPUT_PATH}/\${CONFIGURATION}/StrataExtractOSX.app/Contents/Resources)
                      
# Compile the .xib files using the 'ibtool' program with the destination being the app package
foreach(xib ${Quartz-Xibs})
  add_custom_command (TARGET StrataExtractOSX POST_BUILD
                    COMMAND ${IBTOOL} --errors --warnings --notices --output-format human-readable-text
                              --compile .${EXECUTABLE_OUTPUT_PATH}/\${CONFIGURATION}/StrataExtractOSX.app/Contents/Resources/${xib}.nib
                             .${GUI_QUARTZ_DIR}/Xibs/${xib}.xib)
                      #COMMENT ("Compiling ${GUI_QUARTZ_DIR}/Xibs/${xib}.xib")

endforeach()

endif()

#Check if the system has GTKMM
IF(NOT "${GTKMM_LIBRARIES}" STREQUAL "")
#Find PackageConfig to get the libraries we want
MESSAGE("Found GTKMM, building GStrataExtract")

set(GTKMM-Source
  ${GUI_GTKMM_DIR}/main.hpp
  ${GUI_GTKMM_DIR}/main.cpp
  )
include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(GStrataExtract ${GTKMM-Source})


target_link_libraries(GStrataExtract 
                      ${GTKMM_LIBRARIES})
ENDIF()

#Tell the developer the current build options
message("\n\nlibgrp Compile Options\n Unit Tests: ${UNITTESTS} [-DUNITESTS=${UNITTESTS}] \n")
