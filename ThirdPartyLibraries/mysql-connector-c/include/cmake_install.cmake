# Install script for directory: /mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local/mysql")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/mysql.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/mysql_com.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_command.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/mysql_time.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_list.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_alloc.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/typelib.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/binary_log_types.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_dbug.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/m_string.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_sys.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_xml.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/mysql_embed.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_thread.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_thread_local.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/decimal.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/errmsg.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_global.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_getopt.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/sslopt-longopts.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_dir.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/sslopt-vars.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/sslopt-case.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/sql_common.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/keycache.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/m_ctype.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_compiler.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/mysql_com_server.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_byteorder.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/byte_order_generic.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/byte_order_generic_x86.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/little_endian.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/big_endian.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/thr_cond.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/thr_mutex.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/thr_rwlock.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/mysql_version.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/my_config.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/mysqld_ername.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/mysqld_error.h"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/sql_state.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mysql" TYPE DIRECTORY FILES "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/mysql/" REGEX "/[^/]*\\.h$" REGEX "/psi\\_abi[^/]*$" EXCLUDE)
endif()

