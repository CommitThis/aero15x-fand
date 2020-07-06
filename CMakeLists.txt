project(aero15x_fand)
cmake_minimum_required(VERSION 3.5)

add_subdirectory(bin)
add_subdirectory(src)
add_subdirectory(systemd)
add_subdirectory(test)

enable_testing()

set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "CommitThis") #required
include(CPack)