// DO NOT MODIFY! This file is autogenerated by gn_to_bp.py.
// If need to change a define, modify SkUserConfigManual.h
#pragma once
#include "SkUserConfigManual.h"

#ifndef SK_CODEC_DECODES_JPEG
#define SK_CODEC_DECODES_JPEG
#endif

#ifndef SK_CODEC_DECODES_PNG
#define SK_CODEC_DECODES_PNG
#endif

#ifndef SK_CODEC_DECODES_RAW
#define SK_CODEC_DECODES_RAW
#endif

#ifndef SK_CODEC_DECODES_WEBP
#define SK_CODEC_DECODES_WEBP
#endif

#ifndef SK_ENABLE_ANDROID_UTILS
#define SK_ENABLE_ANDROID_UTILS
#endif

#ifndef SK_ENCODE_JPEG
#define SK_ENCODE_JPEG
#endif

#ifndef SK_ENCODE_PNG
#define SK_ENCODE_PNG
#endif

#ifndef SK_ENCODE_WEBP
#define SK_ENCODE_WEBP
#endif

#ifndef SK_GAMMA_APPLY_TO_A8
#define SK_GAMMA_APPLY_TO_A8
#endif

#ifndef SK_GAMMA_CONTRAST
#define SK_GAMMA_CONTRAST 0.0
#endif

#ifndef SK_GAMMA_EXPONENT
#define SK_GAMMA_EXPONENT 1.4
#endif

#ifndef SK_HAS_ANDROID_CODEC
#define SK_HAS_ANDROID_CODEC
#endif

#ifndef SK_R32_SHIFT
#define SK_R32_SHIFT 16
#endif

#ifndef SK_SUPPORT_GPU
#define SK_SUPPORT_GPU 0
#endif

#ifndef SK_SUPPORT_PDF
#define SK_SUPPORT_PDF
#endif

#ifndef SK_USE_LIBGIFCODEC
#define SK_USE_LIBGIFCODEC
#endif

#ifndef SK_XML
#define SK_XML
#endif

// Correct SK_BUILD_FOR flags that may have been set by
// SkTypes.h/Android.bp
#ifndef SK_BUILD_FOR_UNIX
    #define SK_BUILD_FOR_UNIX
#endif
#ifdef SK_BUILD_FOR_ANDROID
    #undef SK_BUILD_FOR_ANDROID
#endif
#if defined(SK_BUILD_FOR_ANDROID) || defined(SK_BUILD_FOR_IOS) || \
    defined(SK_BUILD_FOR_MAC) || defined(SK_BUILD_FOR_WIN)
    #error "Only SK_BUILD_FOR_UNIX should be defined!"
#endif
