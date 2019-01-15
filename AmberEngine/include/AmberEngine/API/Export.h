#pragma once
#ifndef _EXPORT_H
#define _EXPORT_H

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef AMBERENGINE_EXPORT
#define API_AMBERENGINE __declspec(dllexport)
#else
#define API_AMBERENGINE __declspec(dllimport)
#endif // AMBERENGINE_EXPORT

#endif // _EXPORT_H