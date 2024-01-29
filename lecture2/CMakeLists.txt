cmake_minimum_required(VERSION 3.13)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckFunctionExists)

PROJECT(ubox C)

ADD_DEFINITIONS(-Wall -Wextra -Werror)

INCLUDE_DIRECTORIES(source/include)

OPTION(BUILD_LUA "build Lua plugin" ON)
OPTION(BUILD_EXAMPLES "build examples" ON)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(JSONC json-c)
IF(JSONC_FOUND)
  ADD_DEFINITIONS(-DJSONC)
  INCLUDE_DIRECTORIES(${JSONC_INCLUDE_DIRS})
ENDIF()

SET(SOURCES source/avl/avl.c 
			source/avl/avl-cmp.c 
			source/blob/blob.c 
			source/blob/blobmsg.c 
			source/loop/uloop.c 
			source/sock/usock.c 
			source/stream/ustream.c 
			source/stream/ustream-fd.c
			source/list/vlist.c 
			source/utils/utils.c 
			source/list/safe_list.c
			source/runqueue/runqueue.c 
			source/crypto/md5.c 
			source/list/kvlist.c 
			source/debug/ulog.c 
			source/crypto/base64.c 
			source/debug/udebug.c 
			source/debug/udebug-remote.c)

ADD_LIBRARY(ubox SHARED ${SOURCES})
ADD_LIBRARY(ubox-static STATIC ${SOURCES})
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

FILE(GLOB headers source/include/*.h)
# LIST(FILTER headers EXCLUDE REGEX "-priv.h$" )
INSTALL(FILES ${headers}
	DESTINATION include/libubox
)
INSTALL(TARGETS ubox ubox-static
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)

ADD_SUBDIRECTORY(lua)
ADD_SUBDIRECTORY(examples)

MACRO(ADD_UNIT_TEST_SAN name)
  ADD_EXECUTABLE(${name}-san ${name}.c)
  TARGET_COMPILE_OPTIONS(${name}-san PRIVATE -g -fno-omit-frame-pointer -fsanitize=undefined,address,leak -fno-sanitize-recover=all)
  TARGET_LINK_OPTIONS(${name}-san PRIVATE -fsanitize=undefined,address,leak)
  TARGET_LINK_LIBRARIES(${name}-san ubox blobmsg_json json_script ${json})
  TARGET_INCLUDE_DIRECTORIES(${name}-san PRIVATE ${PROJECT_SOURCE_DIR}/source/include)
ENDMACRO(ADD_UNIT_TEST_SAN)

IF(UNIT_TESTING)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(tests)
ENDIF()

find_library(json NAMES json-c)
IF(EXISTS ${json})
	ADD_LIBRARY(blobmsg_json SHARED source/blob/blobmsg_json.c)
	TARGET_LINK_LIBRARIES(blobmsg_json ubox ${json})

	ADD_LIBRARY(blobmsg_json-static STATIC source/blob/blobmsg_json.c)
	SET_TARGET_PROPERTIES(blobmsg_json-static
			      PROPERTIES OUTPUT_NAME blobmsg_json)

	IF(UNIT_TESTING)
		ADD_UNIT_TEST_SAN(jshn)
	ENDIF(UNIT_TESTING)

	ADD_EXECUTABLE(jshn source/json/jshn.c)
	TARGET_LINK_LIBRARIES(jshn blobmsg_json ${json})

	ADD_LIBRARY(json_script SHARED source/json/json_script.c)
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

IF(ABIVERSION)
	SET_TARGET_PROPERTIES(ubox PROPERTIES VERSION ${ABIVERSION})
	SET_TARGET_PROPERTIES(json_script PROPERTIES VERSION ${ABIVERSION})
	SET_TARGET_PROPERTIES(blobmsg_json PROPERTIES VERSION ${ABIVERSION})
ENDIF()
