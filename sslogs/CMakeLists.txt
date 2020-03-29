# Make sure the compiler can find include files from our sslogs library.
INCLUDE_DIRECTORIES(
    ${SSLOGS_SOURCE_DIR}/sslogs
    ${SSLOGS_SOURCE_DIR}/3rd/pthreadVC2/include/pthread
)
# Make sure the linker can find the sslogs library once it is built.
LINK_DIRECTORIES(
    ${SSLOGS_BINARY_DIR}/sslogs
    ${SSLOGS_BINARY_DIR}/3rd/pthreadVC2/libs
)
ADD_DEFINITIONS(-D SSLOGS_EXPORTS)
# Create a library called "sslogs" which includes the source file "sslogs.cpp".
# The extension is already found.  Any number of sources could be listed here.

ADD_LIBRARY (sslogs SHARED sslogs.c)
TARGET_LINK_LIBRARIES (sslogs pthreadVC2)

MESSAGE(STATUS "Project source directory:" ${SSLOGS_SOURCE_DIR}) 
MESSAGE(STATUS "Project libs directory:" ${SSLOGS_BINARY_DIR}) 