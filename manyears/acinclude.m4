AC_DEFUN(AC_INST_EXTENSIONS,
[
AC_ARG_ENABLE(sse, [  --enable-sse           enable SSE support (will still work on other processors)], [if test "$enableval" = yes; then AC_DEFINE(_ENABLE_SSE)fi])
AC_ARG_ENABLE(3dnow, [  --enable-3dnow         enable 3DNow! support (will still work on other processors)], [if test "$enableval" = yes; then AC_DEFINE(_ENABLE_3DNOW)fi])
])


AC_DEFUN(AC_MODULE_OPT,
[
AC_ARG_ENABLE($1, [  --enable-$1           enable module $1 (default: $2)], [if test "$enableval" = no; then $1=; else $1=$1; fi], [if test -f $1/Makefile.am && test $2 = yes; then $1=$1; else $1=; fi ])
])

AC_DEFUN(AC_FIND_FILE,
[
$3=NO
for i in $2;
do
  for j in $1;
  do
    if test -r "$i/$j"; then
      $3=$i
      break 2
    fi
  done
done
])


AC_DEFUN(AC_LIBTOOL_KLUDGE,
[
AC_ARG_WITH(libtool-ld,
    [  --with-libtool-ld=<linker>    tells libtool to use <linker> instead of ld to link shared libraries],
    [mv libtool libtool-bak  
export withval
cat libtool-bak | perl -ne 's/\+h /\\\${wl}\+h/; s/ \+b / \\\${wl}\+b/; s/\"\/.*\/ld\"/\"$ENV{"withval"}\"/; print' > libtool
    ])
])


AC_DEFUN(AC_PATH_FFTW,
[
LIBFFTW="-lrfftw -lfftw"

AC_MSG_CHECKING([for fftw])

LIBFFTW="$LIBFFTW"
ac_fftw_includes=NO ac_fftw_libraries=NO ac_fftw_bindir=NO
fftw_libraries=""
fftw_includes=""
AC_ARG_WITH(fftw-dir,
    [  --with-fftw-dir=DIR       where the root of FFTW is installed ],
    [  ac_fftw_includes="$withval"/include
       ac_fftw_libraries="$withval"/lib
       ac_fftw_bindir="$withval"/bin
    ])

AC_ARG_WITH(fftw-includes,
    [  --with-fftw-includes=DIR  where the FFTW includes are. ],
    [  
       ac_fftw_includes="$withval"
    ])
    
fftw_libs_given=no

AC_ARG_WITH(fftw-libraries,
    [  --with-fftw-libraries=DIR where the FFTW library is installed.],
    [  ac_fftw_libraries="$withval"
       fftw_libs_given=yes
    ])
AC_CACHE_VAL(ac_cv_have_fftw,
[#try to guess FFTW locations

fftw_incdirs="/usr/lib/fftw/include /opt/include /usr/local/fftw/include /usr/include/fftw /usr/include /usr/local/include $FFTWINC"
test -n "$FFTWDIR" && fftw_incdirs="$FFTWDIR/include $FFTWDIR $fftw_incdirs"
fftw_incdirs="$ac_fftw_includes $fftw_incdirs"
AC_FIND_FILE(fftw.h, $fftw_incdirs, fftw_incdir)
ac_fftw_includes="$fftw_incdir"

fftw_libdirs="/usr/lib/fftw/lib /usr/lib /opt/lib /usr/local/fftw/lib /usr/local/lib /usr/lib/fftw /usr/local/lib $FFTWLIB"
test -n "$FFTWDIR" && fftw_libdirs="$FFTWDIR/lib $FFTWDIR $fftw_libdirs"
if test ! "$ac_fftw_libraries" = "NO"; then
  fftw_libdirs="$ac_fftw_libraries $fftw_libdirs"
fi

test=NONE
fftw_libdir=NONE
for dir in $fftw_libdirs; do
  try="ls -1 $dir/libfftw*"
  if test=`eval $try 2> /dev/null`; then fftw_libdir=$dir; break; else echo "tried $dir" >&AC_FD_CC ; fi
done

ac_fftw_libraries="$fftw_libdir"

ac_cxxflags_safe="$CXXFLAGS"
ac_ldflags_safe="$LDFLAGS"
ac_libs_safe="$LIBS"

INCLUDE="$INCLUDE -I$fftw_incdir $all_includes"
LDFLAGS="-L$fftw_libdir $all_libraries"
LIBS="$LIBS $LIBFFTW"

CXXFLAGS="$ac_cxxflags_safe"
LDFLAGS="$ac_ldflags_safe"
LIBS="$ac_libs_safe"

if test "$ac_fftw_includes" = NO || test "$ac_fftw_libraries" = NO; then
  ac_cv_have_fftw="have_fftw=no"
  ac_fftw_notfound=""
  if test "$ac_fftw_includes" = NO; then
    if test "$ac_fftw_libraries" = NO; then
      ac_fftw_notfound="(headers and libraries)";
    else
      ac_fftw_notfound="(headers)";
    fi
  else
    ac_fftw_notfound="(libraries)";
  fi

else
  have_fftw="yes"
fi
])

eval "$ac_cv_have_fftw"

if test "$have_fftw" != yes; then
  AC_MSG_RESULT([$have_fftw]);
else
  ac_cv_have_fftw="have_fftw=yes \
    ac_fftw_includes=$ac_fftw_includes ac_fftw_libraries=$ac_fftw_libraries"
  AC_MSG_RESULT([libraries $ac_fftw_libraries, headers $ac_fftw_includes])
  
  fftw_libraries="$ac_fftw_libraries"
  fftw_includes="$ac_fftw_includes"
  AC_DEFINE(HAVE_FFTW)
fi

dnl if test ! "$fftw_libs_given" = "yes"; then
dnl CHECK_FFTW_DIRECT(fftw_libraries= ,[])
dnl fi

AC_SUBST(fftw_libraries)
AC_SUBST(fftw_includes)

if test "$fftw_includes" = "/usr/include" || "$fftw_includes" = "$x_includes" || test -z "$fftw_includes"; then
 FFTW_INCLUDES="";
else
 FFTW_INCLUDES="-I$fftw_includes"
 all_includes="$FFTW_INCLUDES $all_includes"
fi

if test "$fftw_libraries" = "$x_libraries" || test -z "$fftw_libraries"; then
 FFTW_LDFLAGS=""
LIB_FFTW=""
else
 FFTW_LDFLAGS="-L$fftw_libraries"
LIB_FFTW='-lrfftw -lfftw'
 all_libraries="$FFTW_LDFLAGS $all_libraries"
fi

AC_SUBST(FFTW_INCLUDES)
AC_SUBST(FFTW_LDFLAGS)

AC_SUBST(LIB_FFTW)

])

AC_DEFUN(AC_THREAD,
[
G_THREAD_CFLAGS=_REENTRANT
G_THREAD_LIBS=error
AC_CHECK_LIB(pthread, pthread_attr_init,
     G_THREAD_LIBS="-lpthread")
if test "x$G_THREAD_LIBS" = xerror; then
     AC_CHECK_LIB(pthreads, pthread_attr_init,
     	G_THREAD_LIBS="-lpthreads")
fi
if test "x$G_THREAD_LIBS" = xerror; then
     AC_CHECK_LIB(thread, pthread_attr_init,
     	G_THREAD_LIBS="-lthread")
fi
if test "x$G_THREAD_LIBS" = xerror; then
     AC_CHECK_LIB(c_r, pthread_attr_init,
     	G_THREAD_LIBS="-lc_r")
fi
if test "x$G_THREAD_LIBS" = xerror; then
     AC_CHECK_FUNC(pthread_attr_init, 
        G_THREAD_LIBS="")
fi
dnl ********** DG/UX ************
if test "x$G_THREAD_LIBS" = xerror; then
     AC_CHECK_LIB(thread, __d10_pthread_attr_init,
        G_THREAD_LIBS="-lthread"
     G_THREAD_CFLAGS="_POSIX4A_DRAFT10_SOURCE")
fi
dnl ********* HPUX 11 ***********
if test "x$G_THREAD_LIBS" = xerror; then
     AC_CHECK_LIB(pthread, __pthread_attr_init_system,
        G_THREAD_LIBS="-lpthread")
fi
if test "x$G_THREAD_LIBS" = xerror; then
   if test $OS="FREEBSD"; then
     G_THREAD_LIBS="-pthread"
   else
     G_THREAD_LIBS=""
   fi
fi

SEM_LIBS=error
AC_CHECK_FUNC(sem_init, SEM_LIBS="")
if test "x$SEM_LIBS" = xerror; then
     AC_CHECK_LIB(pthread, sem_init,
     	SEM_LIBS="")
fi
if test "x$SEM_LIBS" = xerror; then
     AC_CHECK_LIB(rt, sem_init,
     	SEM_LIBS="-lrt")
fi
if test "x$SEM_LIBS" = xerror; then
     SEM_LIBS=""
fi

G_THREAD_LIBS="$G_THREAD_LIBS $SEM_LIBS"
LIBS="$LIBS $G_THREAD_LIBS"
AC_SUBST(LIBS)
AC_DEFINE_UNQUOTED(${G_THREAD_CFLAGS})
])

AC_DEFUN(AC_HASH_MAP,
[
FOUND_HASH=no
AC_CHECK_HEADERS(hash_map ext/hash_map, FOUND_HASH=yes)
if test $FOUND_HASH = "no"; then
AC_DEFINE_UNQUOTED(NO_HASH_MAP)
fi
])


AC_DEFUN(AC_OVERFLOW_CHECKS,
[

AC_DEFINE_UNQUOTED(OVERFLOW_VERSION, "${VERSION}")

dnl AC_CANONICAL_HOST
dnl AC_DISABLE_STATIC
dnl AM_PROG_LIBTOOL

case "$host_os" in 
hpux*) OS=HPUX ;;
linux*) OS=LINUX ;;
freebsd*) OS=FREEBSD ;;
solaris*) OS=SOLARIS ;;
esac
dnl AC_DEFINE_UNQUOTED(${OS})
dnl Initialize libtool.

dnl AC_LIBTOOL_ACC_KLUDGE
AC_LIBTOOL_KLUDGE

dnl AM_SANITY_CHECK

dnl Checks for programs.

AM_C_PROTOTYPES
AC_PROG_CXX
AC_LANG_CPLUSPLUS
AC_PROG_MAKE_SET
AC_C_BIGENDIAN

AC_INST_EXTENSIONS

dnl Checks for libraries.
AC_CHECK_HEADERS(dlfcn.h dl.h float.h values.h semaphore.h machine/soundcard.h sys/soundcard.h linux/rtc.h linux/mc146818rtc.h)
AC_HASH_MAP

dnl Test for math library, and define LIBS
AC_CHECK_LIB(m, sin)
AC_CHECK_LIB(dl, dlopen)
dnl AC_CHECK_LIB(pthread, pthread_create)
AC_THREAD

echo checking for libxml...
if gnome-config xml --cflags | grep I;then echo libxml found;else echo libxml not found; exit 1; fi
GNOME_XML_LIB=`gnome-config --libs xml`
GNOME_XML_INCLUDE=`gnome-config --cflags xml`
AC_SUBST(GNOME_XML_LIB)
AC_SUBST(GNOME_XML_INCLUDE)

AC_PATH_FFTW

if test "x$prefix" != "xDONE"; then
AC_DEFINE_UNQUOTED(INSTALL_PREFIX, "${prefix}")
AC_DEFINE_UNQUOTED(TOOLBOX_PATH, "${prefix}/toolbox")
else
AC_DEFINE_UNQUOTED(INSTALL_PREFIX, "${ac_default_prefix}")
AC_DEFINE_UNQUOTED(TOOLBOX_PATH, "${ac_default_prefix}/toolbox")
fi



])
