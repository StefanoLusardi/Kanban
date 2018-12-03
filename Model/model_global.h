#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MODEL_LIB)
#  define MODEL_EXPORT Q_DECL_EXPORT
# else
#  define MODEL_EXPORT Q_DECL_IMPORT
# endif
#else
# define MODEL_EXPORT
#endif
