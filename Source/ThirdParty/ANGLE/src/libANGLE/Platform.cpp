//
// Copyright 2015 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// Platform.cpp: Implementation methods for angle::Platform.

#include <platform/Platform.h>

#include <cstring>

#include "common/debug.h"

namespace
{
// TODO(jmadill): Make methods owned by egl::Display.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
    angle::PlatformMethods g_platformMethods;
#pragma clang diagnostic pop
}  // anonymous namespace

angle::PlatformMethods::PlatformMethods()
{
}

angle::PlatformMethods *ANGLEPlatformCurrent()
{
    return &g_platformMethods;
}

bool ANGLE_APIENTRY ANGLEGetDisplayPlatform(angle::EGLDisplayType display,
                                            const char *const methodNames[],
                                            unsigned int methodNameCount,
                                            void *context,
                                            void *platformMethods)
{
    angle::PlatformMethods **platformMethodsOut =
        reinterpret_cast<angle::PlatformMethods **>(platformMethods);

    // We allow for a lower input count of impl platform methods if the subset is correct.
    if (methodNameCount > angle::g_NumPlatformMethods)
    {
        ERR() << "Invalid platform method count: " << methodNameCount << ", expected "
              << angle::g_NumPlatformMethods << ".";
        return false;
    }

    for (unsigned int nameIndex = 0; nameIndex < methodNameCount; ++nameIndex)
    {
        const char *expectedName = angle::g_PlatformMethodNames[nameIndex];
        const char *actualName   = methodNames[nameIndex];
        if (strcmp(expectedName, actualName) != 0)
        {
            ERR() << "Invalid platform method name: " << actualName << ", expected " << expectedName
                  << ".";
            return false;
        }
    }

    // TODO(jmadill): Store platform methods in display.
    g_platformMethods.context = context;
    *platformMethodsOut       = &g_platformMethods;
    return true;
}

void ANGLE_APIENTRY ANGLEResetDisplayPlatform(angle::EGLDisplayType display)
{
    // TODO(jmadill): Store platform methods in display.
    g_platformMethods = angle::PlatformMethods();
}
