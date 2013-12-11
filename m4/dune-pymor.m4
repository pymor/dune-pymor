AC_DEFUN([DUNE_PYMOR_CHECKS],[
  PKG_CHECK_MODULES([EIGEN],
                    [eigen3],
                    [AC_DEFINE([HAVE_EIGEN],
                               [1],
                               [Define wether the eigen includes were found.])],
                               [AC_MSG_RESULT([Package 'eigen3' was not found in the pkg-config search path.
Perhaps you should add the directory containing `eigen3.pc'
to the PKG_CONFIG_PATH environment variable.])])
  DUNE_CPPFLAGS="$DUNE_CPPFLAGS $EIGEN_CFLAGS"
])

AC_DEFUN([DUNE_PYMOR_CHECK_MODULE],
[
  DUNE_CHECK_MODULES([dune-pymor],[pymor/parameters/base.hh], [Dune::Pymor::Parameter()])
])
