// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#include <Windows.h>
#include <stdio.h>
#include <dbghelp.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <ole2.h>
#include <initguid.h>
#include <time.h>
#include <math.h>
#include <float.h>


#endif //PCH_H
