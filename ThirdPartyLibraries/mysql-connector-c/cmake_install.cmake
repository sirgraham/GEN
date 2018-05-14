# Install script for directory: /mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Readme")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE OPTIONAL FILES
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/COPYING"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/LICENSE.mysql"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Readme")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/README")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Documentation")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/docs" TYPE FILE FILES
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/Docs/INFO_SRC"
    "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/Docs/INFO_BIN"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "DebugReadme")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE OPTIONAL RENAME "LICENSE.mysql-debug" FILES "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/LICENSE.mysql")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "DebugReadme")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE OPTIONAL RENAME "COPYING-debug" FILES "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/COPYING")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "DebugReadme")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE RENAME "README-debug" FILES "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/README")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/zlib/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/extra/yassl/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/extra/yassl/taocrypt/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/include/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/dbug/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/strings/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/vio/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/regex/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/mysys/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/mysys_ssl/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/libmysql/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/extra/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/scripts/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/testclients/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/support-files/cmake_install.cmake")
  include("/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/packaging/WiX/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "/mnt/GENFrameWork/ThirdPartyLibraries/mysql-connector-c/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
