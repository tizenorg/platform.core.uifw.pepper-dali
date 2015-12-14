#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([pepper-dali], [0.0.1])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(1.6)

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL

# Checks for libraries.
PKG_CHECK_MODULES([PEPPER], [pepper xkbcommon])
PKG_CHECK_MODULES([WAYLAND], [wayland-server xdg-shell-server tizen-extension-client])
PKG_CHECK_MODULES([DALI], [dali-core dali dali-toolkit])
PKG_CHECK_MODULES([EFL], [ecore ecore-input ecore-wayland])

AC_CONFIG_FILES([
Makefile
pepper-dali/Makefile
])
AC_OUTPUT([
pepper-dali.pc
])
