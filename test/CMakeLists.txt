# Make sure the compiler can find include files from our sslogs library.
INCLUDE_DIRECTORIES (${sslogs_SOURCE_DIR}/sslogs)
INCLUDE_DIRECTORIES(
    ${sslogs_SOURCE_DIR}/sslogs
    ${sslogs_SOURCE_DIR}/3rd/pthreadVC2/include/pthread
)
# Make sure the linker can find the sslogs library once it is built.
LINK_DIRECTORIES(
    ${sslogs_BINARY_DIR}/sslogs
    ${sslogs_BINARY_DIR}/3rd/pthreadVC2/libs
)
# Add executable called "test" that is built from the source files
# "test.cpp" and "test.cpp".  The extensions are automatically found.
ADD_EXECUTABLE (test test.cpp)

SET(LIBS 
    sslogs
)
# Link the executable to the test library.
TARGET_LINK_LIBRARIES (test ${LIBS})