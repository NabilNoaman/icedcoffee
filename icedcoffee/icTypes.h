/*
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2008-2010 Ricardo Quesada
 * Copyright (c) 2011 Zynga Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 @file icTypes.h
 @brief IcedCoffee types
 */

#pragma once

#import "Platforms/icGL.h"
#import "kazmath/kazmath.h"

/** @name Color Types */

/**
 @brief Defines an RGBA color composed of four bytes
 */
typedef struct icColor4B {
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} icColor4B;

/** @name Texture Coordinate Types */

typedef struct icTex2F {
    GLfloat u;
    GLfloat v;
} icTex2F;

/** @name Vertex Types */

typedef struct icV3F_C4B_T2F {
    kmVec3 vect;                // 12 bytes
	icColor4B color;			// 4 bytes
    kmVec2 texCoords;           // 8 bytes
} icV3F_C4B_T2F;

/**
 @brief A 3D quad defined by four vertices
 */
typedef struct icV3F_C4B_T2FQuad {
	icV3F_C4B_T2F tl;
	icV3F_C4B_T2F bl;
	icV3F_C4B_T2F tr;
	icV3F_C4B_T2F br;
} icV3F_C4B_T2FQuad;

/**
 @brief Short for icV3F_C4B_T2FQuad
 */
typedef icV3F_C4B_T2FQuad icQuad;

/**
 @struct icBlendFunc
 @brief Blend function used for textures
 */
typedef struct icBlendFunc
{
	//! source blend function
	GLenum src;
	//! destination blend function
	GLenum dst;
} icBlendFunc;

/**
 @enum icResolutionType
 @brief Texture resolution type
 */
typedef enum icResolutionType {
	//! Unknonw resolution type
	kICResolutionUnknown,
	//! iPhone resolution type
	kICResolutioniPhone,
	//! RetinaDisplay resolution type
	kICResolutionRetinaDisplay,
	//! iPad resolution type
	kICResolutioniPad,
} icResolutionType;

typedef double icTime;
