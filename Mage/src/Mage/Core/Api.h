#pragma once

#ifdef MAGE_LIBRARY_BUILD
#define MAGE_API __declspec(dllexport)
#else
#define MAGE_API __declspec(dllimport)
#endif