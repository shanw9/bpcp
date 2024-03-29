cmake_minimum_required(VERSION 2.8.12)

PROJECT(uloop C)

SET(CMAKE_INSTALL_PREFIX /)

IF(NOT LUA_CFLAGS)
	pkg_search_module(LUA lua5.1 lua-5.1)
ENDIF()

ADD_DEFINITIONS(-Os -Wall -Werror --std=gnu99 -g3 -I.. ${LUA_CFLAGS})

IF(APPLE)
	SET(CMAKE_SHARED_MODULE_CREATE_C_FLAGS "${CMAKE_SHARED_MODULE_CREATE_C_FLAGS} -undefined dynamic_lookup")
ENDIF(APPLE)

IF(NOT LUAPATH)
	EXECUTE_PROCESS(
		COMMAND  lua -e "for k in string.gmatch(package.cpath .. \";\", \"([^;]+)/..so;\") do if k:sub(1,1) == \"/\" then print(k) break end end"
		OUTPUT_VARIABLE LUAPATH
		RESULT_VARIABLE LUA_CHECK_RES
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	IF(BUILD_LUA)
		IF(NOT ${LUA_CHECK_RES} EQUAL 0 OR "${LUAPATH}" EQUAL "")
			MESSAGE(SEND_ERROR "Lua was not found on your system")
		ENDIF()
	ENDIF()
ENDIF()

IF(BUILD_LUA)
	ADD_LIBRARY(uloop_lua MODULE ../lib/loop/uloop.c)
	SET_TARGET_PROPERTIES(uloop_lua PROPERTIES
		OUTPUT_NAME uloop
		PREFIX ""
	)
	TARGET_LINK_LIBRARIES(uloop_lua ubox)

	INSTALL(TARGETS uloop_lua
		LIBRARY DESTINATION ${LUAPATH}
	)
ENDIF()
