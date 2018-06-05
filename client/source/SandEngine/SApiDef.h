#pragma once

#ifdef SAND_EXPORT
#define SAND_API __declspec(dllexport)
#else
#define SAND_API __declspec(dllimport)
#endif