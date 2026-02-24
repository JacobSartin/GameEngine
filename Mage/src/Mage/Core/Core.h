#pragma once

// If the Mage library is being built, decorate all API as being exported.
// If not, then the library is being used, so decorate all API as being
// imported.
#ifdef MAGE_LIBRARY_BUILD
#define MAGE_API __declspec(dllexport)
#else
#define MAGE_API __declspec(dllimport)
#endif

#include "pch.h"

#include "ICopyDisable.h"

#include "Application.h"
#include "Color.h"
#include "EventEnums.h"
#include "EventListenerBase.h"
#include "EventManager.h"
#include "Exception.h"
#include "Log.h"
#include "Window.h"
