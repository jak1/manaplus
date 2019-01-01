/*
 *  The ManaPlus Client
 *  Copyright (C) 2012-2019  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RESOURCES_FBOINFO_H
#define RESOURCES_FBOINFO_H

#ifdef USE_OPENGL

#include "localconsts.h"

#ifdef ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
#else  // ANDROID
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_opengl.h>
PRAGMA48(GCC diagnostic pop)
#if defined(__APPLE__)
#include <OpenGL/glext.h>
#elif !defined(__native_client__)
#include <GL/glext.h>
#endif  // defined(__APPLE__)
#endif  // ANDROID

struct FBOInfo final
{
    constexpr2 FBOInfo() :
        fboId(0),
        textureId(0),
        rboId(0)
    {
    }

    A_DELETE_COPY(FBOInfo)

    GLuint fboId;
    GLuint textureId;
    GLuint rboId;
};

#endif  // USE_OPENGL
#endif  // RESOURCES_FBOINFO_H
