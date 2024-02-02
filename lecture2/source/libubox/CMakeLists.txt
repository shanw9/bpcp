set_property(DIRECTORY . PROPERTY FOLDER "LibUbox")

include(GlobalCommon)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckFunctionExists)

configure_file(include/libubox_version.h.in ${LibUbox_DERIVED_SOURCES_DIR}/libubox_version.h)
configure_file(include/libubox_features.h.in ${LibUbox_DERIVED_SOURCES_DIR}/libubox_features.h)

OPTION(BUILD_LUA "build Lua plugin" ON)
OPTION(BUILD_EXAMPLES "build examples" ON)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(JSONC json-c)
IF(JSONC_FOUND)
  ADD_DEFINITIONS(-DJSONC)
  INCLUDE_DIRECTORIES(${JSONC_INCLUDE_DIRS})
ENDIF()

set(LibUbox_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
    "${LIBUBOX_DIR}"
    "${LIBUBOX_DIR}/include"
    "${LibUbox_DERIVED_SOURCES_DIR}"
    "${FORWARDING_HEADERS_DIR}"
    "${CMAKE_BINARY_DIR}/src/libubox"
)

# List the headers should be installed to system here.
set(LibUbox_INSTALLED_HEADERS
    "${LibUbox_DERIVED_SOURCES_DIR}/libubox_version.h"
    "${LibUbox_DERIVED_SOURCES_DIR}/libubox_features.h"
    "${LIBUBOX_DIR}/include/ubox-avl-cmp.h"
    "${LIBUBOX_DIR}/include/ubox-avl.h"
    "${LIBUBOX_DIR}/include/ubox-blob.h"
    "${LIBUBOX_DIR}/include/ubox-blobmsg_json.h"
    "${LIBUBOX_DIR}/include/ubox-json_script.h"
    "${LIBUBOX_DIR}/include/ubox-kvlist.h"
    "${LIBUBOX_DIR}/include/ubox-list.h"
    "${LIBUBOX_DIR}/include/ubox-md5.h"
    "${LIBUBOX_DIR}/include/ubox-runqueue.h"
    "${LIBUBOX_DIR}/include/ubox-safe_list.h"
    "${LIBUBOX_DIR}/include/ubox-udebug-proto.h"
    "${LIBUBOX_DIR}/include/ubox-udebug.h"
    "${LIBUBOX_DIR}/include/ubox-ulog.h"
    "${LIBUBOX_DIR}/include/ubox-uloop.h"
    "${LIBUBOX_DIR}/include/ubox-usock.h"
    "${LIBUBOX_DIR}/include/ubox-ustream.h"
    "${LIBUBOX_DIR}/include/ubox-utils.h"
    "${LIBUBOX_DIR}/include/ubox-vlist.h"
)

# List the source files individually.
list(APPEND LibUbox_SOURCES
    "${LIBUBOX_DIR}/lib/avl/avl-cmp.c"
    "${LIBUBOX_DIR}/lib/avl/avl.c"
    "${LIBUBOX_DIR}/lib/blob/blob.c"
    "${LIBUBOX_DIR}/lib/blob/blobmsg.c"
    "${LIBUBOX_DIR}/lib/loop/uloop.c"
    "${LIBUBOX_DIR}/lib/sock/usock.c"
    "${LIBUBOX_DIR}/lib/stream/ustream.c"
    "${LIBUBOX_DIR}/lib/stream/ustream-fd.c"
    "${LIBUBOX_DIR}/lib/list/vlist.c"
    "${LIBUBOX_DIR}/lib/utils/utils.c"
    "${LIBUBOX_DIR}/lib/list/safe_list.c"
    "${LIBUBOX_DIR}/lib/runqueue/runqueue.c"
    "${LIBUBOX_DIR}/lib/crypto/base64.c"
    "${LIBUBOX_DIR}/lib/crypto/md5.c"
    "${LIBUBOX_DIR}/lib/list/kvlist.c"
    "${LIBUBOX_DIR}/lib/debug/ulog.c"
    "${LIBUBOX_DIR}/lib/debug/udebug-remote.c"
    "${LIBUBOX_DIR}/lib/debug/udebug.c"
)

include_directories(${LibUbox_PRIVATE_INCLUDE_DIRECTORIES})

set(LibUbox_LIBRARIES)
set(LibUbox_INTERFACE_LIBRARIES LibUbox)
set(LibUbox_INTERFACE_INCLUDE_DIRECTORIES ${LibUbox_PRIVATE_FRAMEWORK_HEADERS_DIR})

LIBUBOX_FRAMEWORK_DECLARE(LibUbox)
LIBUBOX_INCLUDE_CONFIG_FILES_IF_EXISTS()

LIBUBOX_WRAP_SOURCELIST(${LibUbox_SOURCES})

LIBUBOX_COPY_FILES(LibUbox_CopyPrivateHeaders
    DESTINATION ${LibUbox_PRIVATE_FRAMEWORK_HEADERS_DIR}/libubox
    FILES ${LibUbox_PRIVATE_FRAMEWORK_HEADERS}
    FLATTENED
)
list(APPEND LibUbox_INTERFACE_DEPENDENCIES LibUbox_CopyPrivateHeaders)
# LibUbox_CopyPrivateHeaders needs to have a direct or indirect
# dependency of LibUbox target fo# The -ftree-sra optimization (implicit with -O2) causes crashes when
# allocating large chunks of memory using bmalloc on Intel 32bit.
# See https://bugs.webkit.org/show_bug.cgi?id=146440
string(TOLOWER ${CMAKE_HOST_SYSTEM_PROCESSOR} LOWERCASE_CMAKE_HOST_SYSTEM_PROCESSOR)
if (CMAKE_COMPILER_IS_GNUCXX AND "${LOWERCASE_CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES "(i[3-6]86|x86)$")
    LIBUBOX_ADD_TARGET_CXX_FLAGS(LibUbox -fno-tree-sra)
endif ()

LIBUBOX_COMPUTE_SOURCES(LibUbox)
LIBUBOX_FRAMEWORK(LibUbox)
LIBUBOX_FRAMEWORK_TARGET(LibUbox)

ADD_LIBRARY(ubox SHARED ${LibUbox_SOURCES})
ADD_LIBRARY(ubox-static STATIC ${LibUbox_SOURCES})
SET_TARGET_PROPERTIES(ubox-static PROPERTIES OUTPUT_NAME ubox)

SET(LIBS)
CHECK_FUNCTION_EXISTS(clock_gettime HAVE_GETTIME)
CHECK_FUNCTION_EXISTS(shm_open HAVE_SHM)
IF(NOT HAVE_GETTIME OR NOT HAVE_SHM)
	CHECK_LIBRARY_EXISTS(rt clock_gettime "" NEED_GETTIME)
	CHECK_LIBRARY_EXISTS(rt shm_open "" NEED_SHM)
	IF(NEED_GETTIME OR NEED_SHM)
		TARGET_LINK_LIBRARIES(ubox rt)
	ENDIF()
ENDIF()

install(TARGETS LibUbox
        DESTINATION "${LIB_INSTALL_DIR}/"
)

install(FILES ${LibUbox_INSTALLED_HEADERS}
        DESTINATION "${HEADER_INSTALL_DIR}/libubox"
)

ADD_SUBDIRECTORY(lua)
ADD_SUBDIRECTORY(examples)

MACRO(ADD_UNIT_TEST_SAN name)
  ADD_EXECUTABLE(${name}-san ${name}.c)
  TARGET_COMPILE_OPTIONS(${name}-san PRIVATE -g -fno-omit-frame-pointer -fsanitize=undefined,address,leak -fno-sanitize-recover=all)
  TARGET_LINK_OPTIONS(${name}-san PRIVATE -fsanitize=undefined,address,leak)
  TARGET_LINK_LIBRARIES(${name}-san ubox blobmsg_json json_script ${json})
  TARGET_INCLUDE_DIRECTORIES(${name}-san PRIVATE ${PROJECT_SOURCE_DIR}/include)
ENDMACRO(ADD_UNIT_TEST_SAN)

IF(UNIT_TESTING)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(tests)
ENDIF()

find_library(json NAMES json-c)
IF(EXISTS ${json})
	ADD_LIBRARY(blobmsg_json SHARED lib/blob/blobmsg_json.c)
	TARGET_LINK_LIBRARIES(blobmsg_json ubox ${json})

	ADD_LIBRARY(blobmsg_json-static STATIC lib/blob/blobmsg_json.c)
	SET_TARGET_PROPERTIES(blobmsg_json-static
			      PROPERTIES OUTPUT_NAME blobmsg_json)

	IF(UNIT_TESTING)
		ADD_UNIT_TEST_SAN(jshn)
	ENDIF(UNIT_TESTING)

	ADD_EXECUTABLE(jshn lib/json/jshn.c)
	TARGET_LINK_LIBRARIES(jshn blobmsg_json ${json})

	ADD_LIBRARY(json_script SHARED lib/json/json_script.c)
	TARGET_LINK_LIBRARIES(json_script ubox)

	INSTALL(TARGETS blobmsg_json blobmsg_json-static jshn json_script
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin
	)

	FILE(GLOB scripts sh/*.sh)
	INSTALL(FILES ${scripts}
		DESTINATION share/libubox
	)

ENDIF()