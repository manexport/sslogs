# To perform a Windows build using a 64-bit sslogs binary distribution:
#   Using the Visual Studio 2017 IDE:
#     > cmake -G "Visual Studio 15 Win64" ..
#     Open build\sslogs.sln in Visual Studio and select Build > Build Solution.
#
# To perform a Linux build using a 32-bit sslogs binary distribution on a 32-bit
# Linux platform or a 64-bit sslogs binary distribution on a 64-bit Linux platform:
#   Using Unix Makefiles:
#     > cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
#     > make -j4 
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12.1)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	SET(CURRENT_OS "LINUX_OS")
	MESSAGE(STATUS ${CURRENT_OS})
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	SET(CURRENT_OS "WINDOWS_OS")
	MESSAGE(STATUS ${CURRENT_OS})
ENDIF ()
SET(sslogs_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(sslogs_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})
# The name of our project is "sslogs".  CMakeLists files in this project can
# refer to the root source directory of the project as ${sslogs_SOURCE_DIR} and
# to the root binary directory of the project as ${sslogs_BINARY_DIR}.
MESSAGE(STATUS "sslogs_BINARY_DIR:" ${sslogs_BINARY_DIR}) 
MESSAGE(STATUS "sslogs_SOURCE_DIR:" ${CMAKE_CURRENT_SOURCE_DIR}) 

PROJECT (sslogs)

# Recurse into the "sslogs" and "test" subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
ADD_SUBDIRECTORY (sslogs)
ADD_SUBDIRECTORY (test)
