#pragma once

// Macros of platform.
#undef WINDOWS
#ifdef _WIN32
#define WINDOWS
#endif

#undef LINUX
#ifdef __linux__
#define LINUX
#endif

#ifdef WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#include <stdexcept>
#endif

#ifdef __cplusplus
#ifndef EXTERNC
#define EXTERNC extern "C"
#endif
#else
#define EXTERNC
#endif

#ifdef WINDOWS
#ifndef EXPORT
#define EXPORT EXTERNC __declspec(dllexport)
#endif
#else
#define EXPORT EXTERNC
#endif


EXPORT const char *GetOasisVersion();

/// @brief Get OpenOasis linkable component.
/// @param id Id.
/// @param type Type of component.
/// @param task Task yaml/yml file contains configurations of the component.
/// @return Component.
/// @note No deletion is required.
EXPORT void *GetOasisComponent(const char *id, const char *type, const char *task);
