// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#undef UNICODE
#define UNICODE
#undef _WINSOCKAPI_
#define _WINSOCKAPI_
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include "EventTags.h"
#include "Util.h"




#pragma comment(lib, "Ws2_32.lib")


// TODO: reference additional headers your program requires here
