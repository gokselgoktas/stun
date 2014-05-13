/*
 *                     :::::::: ::::::::::: :::    ::: ::::    :::
 *                   :+:    :+:    :+:     :+:    :+: :+:+:   :+:
 *                  +:+           +:+     +:+    +:+ :+:+:+  +:+
 *                 +#++:++#++    +#+     +#+    +:+ +#+ +:+ +#+
 *                       +#+    +#+     +#+    +#+ +#+  +#+#+#
 *               #+#    #+#    #+#     #+#    #+# #+#   #+#+#
 *               ########     ###      ########  ###    ####
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *
 *        http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an AS IS basis,
 * without warranty of any kind, either express or implied.  See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 3 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 3 or later (the "LGPL"), in
 * which case the provisions of the GPL or the LGPL are applicable instead of
 * those above.
 *
 * If you wish to allow use of your version of this file only under the terms
 * of either the GPL or the LGPL, and not to allow others to use your version
 * of this file under the terms of the MPL, indicate your decision by deleting
 * the provisions above and replace them with the notice and other provisions
 * required by the GPL or the LGPL. If you do not delete the provisions above,
 * a recipient may use your version of this file under the terms of any one of
 * the MPL, the GPL or the LGPL. */

#ifndef STUN_H_S4YXGLYO
#define STUN_H_S4YXGLYO

/* Presence of STUN indicates that Stun is available within the scope of the
 * project.  Library users can take this into account and introduce further
 * optimizations or choose a better code path.
 *
 * Stun itself depends on this macro to pick the correctly formatted versions
 * of certain external resources (such as Stitches). */
#define STUN                                    0x01

/* To compile Stun as a standalone tool, define the TOOL symbol.  This results
 * in a content generation toolkit to be produced.  Otherwise, Stun is compiled
 * as an embeddable object file, serving only as a framework.
 *
 * Attention:  The standalone flavor of Stun depends on Qt, for its user
 * interface and a bit more.  You can obtain Qt from the following address:
 *
 *        http://qt.nokia.com/
 *
 * Specifically speaking, Stun depends on version 4.7.1 of Qt.  However, any
 * version above that should function just as well. */

/* #define TOOL                                 0x01 */

/* Stun can use the newer OpenGL core profiles, whenever possible, if it is
 * asked to do so.  To enable this behavior define the USE_BLEEDING_EDGE_OPENGL
 * symbol.
 *
 * Attention:  Defining this might break behavior and / or code compatibility
 * between various operating-systems.  Be extremely careful!  In addition, this
 * macro has no meaning if Stun is using another renderer than OpenGL.
 *
 * For example, due to Apple not implementing the core profiles of later
 * versions of OpenGL to the fullest, bleeding-edge OpenGL versions are not
 * supported on Mac OS X! */

/* #define USE_BLEEDING_EDGE_OPENGL             0x01 */

/* Stun is portable to numerous operating systems.  Below is an enumeration of
 * these systems that Stun supports. */
#define MAC_OS_X                                0x00
#define LINUX                                   0x01
#define WINDOWS                                 0x02

/* Here, Stun attempts to figure out the operating system.  It relies on the
 * presence of certain compiler-supplied macros for this. */
#if defined (__APPLE__) && defined (__MACH__)
#       define SYSTEM                           MAC_OS_X
#elif defined (linux) || defined (__linux)
#       define SYSTEM                           LINUX
#elif defined (_WIN32)
#       define SYSTEM                           WINDOWS
#else
#       error "Unknown operating system."
#endif /* defined (__APPLE__) && defined (__MACH__) */

/* Stun supports various cross-platform frameworks as back-ends for interfacing
 * the target operating system.
 *
 * Below is an enumeration of these frameworks that can be used by Stun.
 *
 * From these, MINIMAL and GHOST deserve special mention.  The MINIMAL
 * framework is meant for producing extremely lean code for size-coding
 * purposes.
 *
 * On the other hand, the GHOST framework provides hollow implementations of
 * FRAMEWORK-specific functionality.  This feature is extremely useful while
 * using Stun as higher-level API, building on top of OpenGL.
 *
 * Attention:  If you're using the since macro with the GHOST framework, then,
 * an implementation of the seconds macro *must* be provided before including
 * this header. */
#define MINIMAL                                 0x00 /* platform-specific */
#define GHOST                                   0x01
#define QT                                      0x02
#define GLFW                                    0x03
#define SDL                                     0x04

/* The FRAMEWORK macro specifies which back-end to use.  However, if Stun is
 * compiled as a standalone tool, then the back-end will always be GHOST. */
#define FRAMEWORK                               GLFW

#if !defined (FRAMEWORK)
#       define FRAMEWORK                        MINIMAL
#endif /* !defined (FRAMEWORK) */

#if defined (TOOL)
#       undef FRAMEWORK
#       define FRAMEWORK                        GHOST
#endif /* defined (TOOL) */

#if (FRAMEWORK < MINIMAL || FRAMEWORK > SDL)
#       error "Unknown framework."
#elif (FRAMEWORK == MINIMAL)
#endif /* (FRAMEWORK < MINIMAL || FRAMEWORK > SDL) */

/* By default, whenever possible, processing of OS messages are avoided in
 * order to save some more space.
 *
 * However, there might be times when this is desired or in fact required.  By
 * defining the FORCE_MESSAGE_PROCESSING symbol, you can get the flush macro to
 * include extra message processing code on platforms where it is avoided by
 * default.
 *
 * This flag has no meaning when enabled with the GHOST framework. */
#define FORCE_MESSAGE_PROCESSING                0x01

/* For tasks like memory management (and others), Stun uses the APIs local to
 * the platform it's targetting.
 *
 * However, standard library functions are accessible from all of these
 * environments, as well.  Hence, if desired, one can force the use of standard
 * library functions in place of the functions that perform the same task.
 *
 * Defining the FORCE_STANDARD_LIBRARY does exactly that.
 *
 * But, these functions are not available with the MINIMAL framework; due to a
 * fundamental lack of a CRT.  In those cases, Stun reverts back to the its
 * default behavior. */
#define FORCE_STANDARD_LIBRARY                  0x01

#if (FRAMEWORK == MINIMAL)
#       undef FORCE_STANDARD_LIBRARY
#endif /* FRAMEWORK == MINIMAL */

/* Stun supports optional resource handling features that allow one to free any
 * resources consumed by the application.  However, by default this feature is
 * turned off in order to squeeze some more bytes.
 *
 * By defining the EXTEND_RESOURCE_MANAGEMENT symbol you can tell Stun to
 * execute user registered unloader functions upon termination. */
#define EXTEND_RESOURCE_MANAGEMENT              0x01

/* Stun provides the options of using either of a framework-specific OpenGL
 * extension loading mechanism or GLEW.
 *
 * By defining the USE_GLEW_FOR_OPENGL_EXTENSIONS macro, you can tell Stun to
 * use GLEW instead of a framework-specific solution.
 *
 * But, since the MINIMAL framework targets the minimum possible size for the
 * resulting executable, using GLEW is not an option with this framework. */
#define USE_GLEW_FOR_OPENGL_EXTENSIONS          0x01

#if (FRAMEWORK == MINIMAL)
#       undef USE_GLEW_FOR_OPENGL_EXTENSIONS
#elif (FRAMEWORK == QT) && !defined (USE_GLEW_FOR_OPENGL_EXTENSIONS)
        /* Unfortunately Qt doesn't provide a specific mechanism for loading
         * OpenGL extensions.  In that case, we enforce the use of GLEW. */
#       define USE_GLEW_FOR_OPENGL_EXTENSIONS   0x01
#endif /* FRAMEWORK == MINIMAL */

/* Stun tries very hard to outsource all of its mathematically heavy procedural
 * generation and data processing tasks to the GPU.
 *
 * However, there are times when certain tasks are costlier to export to the
 * GPU than to be handle on the CPU.  To address such cases, Stun can be
 * configured to implement a certain set of functions, some using the FPU.
 *
 * Hence, defining the preprocessor macro, ENABLE_MATHEMATICS_SUPPORT, tells
 * Stun to incorporate additional mathematical structures and functions into
 * its facilities.
 *
 * Attention:  If you're using the MINIMAL framework, make sure the
 * floating-point precision or type is set to 'fast'.  This will make the
 * compiler try to use its intrinsics; which Stun can exploit for smaller code
 * generation. */
#define ENABLE_MATHEMATICS_SUPPORT              0x01

/* Due to function overloading not being available in C we will be using a
 * special value that indicates that the parameter is to be ignored. */
#define DISCARD                                 (~0)

/* Two convenience macros meant to be used as the third parameter to window.
 * They should be pretty much self explanatory. */
#define WINDOWED                                false
#define FULLSCREEN                              true

/* Define this symbol to the number of tasks Stun has to execute during the
 * loading phase. */
#define NUMBER_OF_LOADERS                       1

/* This is basically tan(FoV * .5) * near; where FoV stands for the
 * field-of-view angle in radians.  It is the most crucial value for the
 * frustum. */
#define FRUSTUM                                 (.414213f * NEAR_PLANE_DEPTH)

/* The distance to the near plane from the eye point. */
#define NEAR_PLANE_DEPTH                        1.

/* The distance to the far plane from the eye point. */
#define FAR_PLANE_DEPTH                         100.

/* The maximum number of elements that the stitcher array can store.  If the
 * number of added fragments exceed this, then you're out of luck. */
#define MAXIMUM_STITCH_SIZE                     64

/* The number of stitches available to pick from.  This is equal to the number
 * of unique shader fragments (called stitches) the application has. */
#define NUMBER_OF_STITCHES                      8

/* This is a special symbol that indicates the end of the indices stream. */
#define END_STITCH_STREAM                       0xFF

/* The width and height of the grid of primitives used by the mesh generator.
 * This grid acts as the reference plane according to which the procedural
 * generation is done.
 *
 * In turn, this means, there cannot be more data streamed out of the GPU than
 * this grid can contain.
 *
 * This implies that the sizes of the textures used to generate the mesh cannot
 * be larger than the size of this grid. */
#define MAXIMUM_MESH_GRID_WIDTH                 512
#define MAXIMUM_MESH_GRID_HEIGHT                512

/* Stun provides additional features for computing certain vertex attributes
 * of a mesh (such as the tangent-space vertex normals and the tangent
 * vectors).
 *
 * However, by default, any procedurally generated mesh won't have any of its
 * vertex attributes computed unless the operation is manually performed.
 *
 * If automatic computation of these vectors are desired for each and every
 * procedurally generated mesh, define the COMPUTE_VERTEX_ATTRIBUTES
 * preprocessor symbol. */
#define COMPUTE_VERTEX_ATTRIBUTES               0x01

/* The DEFAULT_VERTEX_SHADER macro resolves to the OpenGL ID of the precompiled
 * pass-through vertex shader that does nothing but send up to four
 * multi-texturing coordinates down the pipeline.
 *
 * The default vertex shader is provided as a convenience functionality and
 * comes very handy for things like post-processing. */
#define DEFAULT_VERTEX_SHADER                   MUANXLC

/* Stun provides a simple and transparent way of automatically creating an
 * accumulation layer.  An accumulation layer is a bundle of an FBO and its
 * texture attachment (altogether, a post-processing layer) onto which all of
 * the render products are output.
 *
 * Using an accumulation layer makes it extremely simple to apply a coat of
 * post-processing on top of the render products.
 *
 * By defining the GENERATE_ACCUMULATION_LAYER symbol one can ask Stun to
 * generate a fullscreen, floating-point accumulation layer (a layer_t).
 *
 * If generated, this layer will then become accessible through the
 * ACCUMULATION macro. */
#define GENERATE_ACCUMULATION_LAYER             0x01

#if defined (GENERATE_ACCUMULATION_LAYER)
#       define ACCUMULATION                     P4E6JEEC
#endif /* defined (GENERATE_ACCUMULATION_LAYER) */

/* Stun provides global access to its render window's properties, whenever
 * possible.  Obviously, in order to do so, in needs some global storage.
 *
 * With the MINIMAL back-end, it is very important for Stun to make sure it
 * doesn't waste resources by introducing the use of new memory locations.  For
 * this reason, it tries to reuse as many storage locations as possible,
 * without creating new ones.
 *
 * However, with the rest of the back-ends, for the sake of easy portability,
 * Stun uses a separate global location for storing the window's properties. */
#if (FRAMEWORK == MINIMAL)
#       if (SYSTEM == MAC_OS_X)
                /* todo */
#       elif (SYSTEM == LINUX)
                /* todo */
#       elif (SYSTEM == WINDOWS)
                /* On Windows, with the MINIMAL back-end, the width and the
                 * height of the window are stored within the DEVMODE
                 * construct.
                 *
                 * These exact locations also map to the locations that are
                 * queried by ChangeDisplaySettings for the width and the
                 * height of the window while switching to fullscreen.
                 *
                 * This way, ChangeDisplaySettings can also take advantage of
                 * these values; and using the same memory location for
                 * multiple purposes produces code that compresses better. */
#               define WINDOW_WIDTH             (*((int *) JNY8NEKN + 0x1B))
#               define WINDOW_HEIGHT            (*((int *) JNY8NEKN + 0x1C))
#       endif /* SYSTEM == MAC_OS_X */
#else
        /* On the rest of the FRAMEWORKs, we use a global storage buffer for
         * storing the window's properties. */
#       define WINDOW_WIDTH                     TVJAYSLH[0]
#       define WINDOW_HEIGHT                    TVJAYSLH[1]
#endif /* FRAMEWORK == MINIMAL */

/* The following three values are used for preprocessor string generation; and
 * hence, for the sake of optimisation, they must be manually maintained. */

/* Two times the distance to the near plane from the eye point. */
#define LIWQTX2O                                _(2.)

/* Sum of the distance to the near and far planes from the eye point. */
#define O7TNQ83D                                _(101.)

/* The distance between the far and near planes. */
#define GUM5GQZZ                                _(99.)

/* The number of OpenGL GPU programs automatically generated by Stun.  Almost
 * all of these programs are meant explicitly for internal use; as well as this
 * preprocessor symbol itself. */
#define ON3400B7                                4

/* Some compilers don't provide the stdbool.h header as required by the
 * IEEE STD 1003.1-2001 standard.  However, Stun makes use of this type and it
 * is convenient to have access to such a facility.
 *
 * As a result, in case an actual stdbool.h (or a variety of it) is not
 * included before this header, or this header is not being included within a
 * C++ source, a flavor of stdbool.h is provided by Stun.
 *
 * Differently from the standard stdbool.h header, Stun's version allows anyone
 * to decide for the underlying type used to represent a boolean. */
#if !defined (__bool_true_false_are_defined) && !defined (__cplusplus)
        /* By defining the following symbol as the type of your choice, you can
         * change the underlying data type represented by bool. */
#       define BOOL                             unsigned char

        /* The true keyword indicates a positive state and is represented by
         * the constant 1. */
#       define true                             0x01

        /* The false keyword indicates a negative state.  Basic logic would
         * suggest that false must be everything that isn't true.  However,
         * this is impossible to express in pure C.  Therefor, it's represented
         * by the constant 0. */
#       define false                            0x00

        /* The bool keyword expands to the actual boolean type introduced by
         * the C99 standard: _Bool.  However, as a bounty, Stun provides a way
         * to override this definition.  See: BOOL. */
#       if !defined (BOOL)
#               define bool                     _Bool

                /* If the active C standard followed by the compiler is older
                 * than C99 then _Bool is defined as an alias of int in order
                 * to avoid sacrificing the boolean type. */
#               if defined (__STDC__) && (__STDC_VERSION__ < 199901L)
                        typedef int             _Bool;
#               endif /* defined (__STDC__) && (__STDC_VERSION__ < 199901L) */
#       else
                typedef BOOL bool;
#       endif /* !defined (BOOL) */

        /* Finally, signal to the rest of the source tree that as of now bool,
         * true and false keywords are defined. */
#       define __bool_true_false_are_defined    0x01
#endif /* !defined (__bool_true_false_are_defined) && !defined (__cplusplus) */

/* This macro makes a string out of the given preprocessor token without
 * expanding it. */
#define N2IGT4HO(TOKEN)                         #TOKEN

/* _ is a convenience macro that stringizes its input token.  However, unlike
 * N2IGT4HO, the _ macro expands its input. */
#define _(TOKEN)                                N2IGT4HO(TOKEN)

/* On Windows, while using the MINIMAL framework, some static data is required
 * to be able to initialize a fullscreen window.  The T7E0PSFY preprocessor
 * macro embeds this data at the location of its invocation.
 *
 * So far, on the other platforms no static data is required.  On these
 * platforms, this macro resolves to simply nothing. */
#if (FRAMEWORK == MINIMAL)
#       if (SYSTEM == WINDOWS)
                /* The device mode descriptor is required to switch to the
                 * fullscreen display mode.  It contains information about the
                 * initialization and environment of a printer or a display
                 * device.
                 *
                 * However, we also embed our pixel-format descriptor here for
                 * the sake of tight packing. */
#               define T7E0PSFY()                                             \
                        static DEVMODE N8ZWIZB9 = {                           \
                                /* [32 bytes] */                              \
                                {                                             \
                                        0, 0,           /* [2 bytes] */       \
                                        0, 0,           /* [2 bytes] */       \
                                                                              \
                                        37, 0, 0, 0,    /* [4 bytes] */       \
                                                                              \
                                        PFD_TYPE_RGBA,  /* [1 byte] */        \
                                                                              \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        32,             /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                        0,              /* [1 byte] */        \
                                                                              \
                                        0, 0, 0, 0      /* [4 bytes] */       \
                                },                                            \
                                                                              \
                                0,                      /* [2 bytes] */       \
                                0,                      /* [2 bytes] */       \
                                                                              \
                                sizeof(DEVMODE),        /* [2 bytes] */       \
                                                                              \
                                0,                      /* [2 bytes] */       \
                                                                              \
                                DM_PELSWIDTH |          /* [ >> ] */          \
                                DM_PELSHEIGHT,          /* [4 bytes] */       \
                                                                              \
                                {                                             \
                                        {                                     \
                                                0,      /* [2 bytes] */       \
                                                0,      /* [2 bytes] */       \
                                                0,      /* [2 bytes] */       \
                                                0,      /* [2 bytes] */       \
                                                0,      /* [2 bytes] */       \
                                                0,      /* [2 bytes] */       \
                                                0,      /* [2 bytes] */       \
                                                0,      /* [2 bytes] */       \
                                        }                                     \
                                },                                            \
                                                                              \
                                0,                      /* [2 bytes] */       \
                                0,                      /* [2 bytes] */       \
                                0,                      /* [2 bytes] */       \
                                0,                      /* [2 bytes] */       \
                                0,                      /* [2 bytes] */       \
                                                                              \
                                {                                             \
                                        0               /* [32 bytes] */      \
                                },                                            \
                                                                              \
                                0,                      /* [2 bytes] */       \
                                0,                      /* [4 bytes] */       \
                                0,                      /* [4 bytes] */       \
                                0,                      /* [4 bytes] */       \
                                                                              \
                                {                                             \
                                        0               /* [4 bytes] */       \
                                },                                            \
                                                                              \
                                0,                      /* [4 bytes] */       \
                                0,                      /* [4 bytes] */       \
                                0,                      /* [4 bytes] */       \
                                0,                      /* [4 bytes] */       \
                                0,                      /* [4 bytes] */       \
                                0,                      /* [4 bytes] */       \
                                0,                      /* [4 bytes] */       \
                                0,                      /* [4 bytes] */       \
                                0,                      /* [4 bytes] */       \
                        };                                                    \
                                                                              \
                        /* The memory pointed to by this pointer is used as a
                         * storage for various data.  Think of this memory as a
                         * generic scratch buffer.
                         *
                         * Using the same pointer for storing and accessing
                         * temporary data produces very similar and frequent
                         * opcodes which in turn pack extremely well.
                         *
                         * In addition, this pointer also points to the
                         * embedded PIXELFORMATDESCRIPTOR blob. */            \
                        void *const JNY8NEKN = &N8ZWIZB9;

                        /* The pointer might be used across multiple units. */
                        extern void *const JNY8NEKN;
#       endif /* SYSTEM == WINDOWS */
#endif /* FRAMEWORK == MINIMAL */

#if !defined (T7E0PSFY)
#       define T7E0PSFY()
#endif

/* On Windows the main entry point of a graphical Windows-based application is
 * WinMain.  However, while using the MINIMAL framework, we are not linking
 * against any standard libraries for producing smaller code size.  This causes
 * crt0.obj, which contains the actual entry point, to be absent from the
 * linkage.
 *
 * For this reason, we have to address the issue by defining and declaring the
 * actual entry point of the application if we're using the MINIMAL framework
 * on Windows.
 *
 * For the other platforms supported by Stun, main is the only entry point of
 * the application and hence, no hacking is required.
 *
 * Additionally, on Windows; when using the MINIMAL framework; there are
 * certain other requirements which are also handled through this hack. */
#if (SYSTEM == WINDOWS)
#       if (FRAMEWORK == MINIMAL)
#               define main(...)                                              \
                        *_fltused = (void *) 0x00;                            \
                        T7E0PSFY();                                           \
                        int WinMainCRTStartup(void)
#       elif (FRAMEWORK != GHOST)
                /* The pointer hack below seems quite unnecessary, however we
                 * have to make absolutely sure that the type of WinMain is
                 * int.  However, in contrast the type of main is allowed to be
                 * void as well.
                 *
                 * By inserting this hack, we can work-around the user defined
                 * type of the entry point and make sure it's int. */
#               define main(...)                                              \
                        *JR031HK = (void *) 0x00;                             \
                        int WINAPI WinMain(HINSTANCE IR5LT3IF,                \
                                           HINSTANCE Z7408PH4, LPSTR LKWCKNK, \
                                           int N49XQUYK)
#       endif /* FRAMEWORK == MINIMAL */
#endif /* SYSTEM == WINDOWS */

/* In addition to hacking main itself, Stun provides some more function-like
 * macros for creating a window supporting the chosen renderer, updating this
 * window as well as handling any message processing and finally terminating
 * the application and freeing up the resources. */
#if (FRAMEWORK == MINIMAL)
#       if (SYSTEM == MAC_OS_X)
                /* todo */
#       elif (SYSTEM == LINUX)
                /* todo */
#       elif (SYSTEM == WINDOWS)
                /* The first one of these macros, window, is used to create a
                 * window of the given size which supports the chosen renderer.
                 *
                 * If IS_FULLSCREEN evaluates to true, then a fullscreen window
                 * will be created. */
#               define window(WIDTH, HEIGHT, IS_FULLSCREEN)                   \
                        /* The window handle. */                              \
                        HWND NEWWYUG8;                                        \
                                                                              \
                        /* The window device context.  Isn't really used for
                         * anything other than for initializing and updating
                         * the OpenGL window. */                              \
                        HDC C98WXZEW;                                         \
                                                                              \
                        WINDOW_WIDTH = (WIDTH);                               \
                        WINDOW_HEIGHT = (HEIGHT);                             \
                                                                              \
                        if ((IS_FULLSCREEN)) {                                \
                                /* When running the application fullscreen it
                                 * might become a little annoying to have the
                                 * cursor around.
                                 *
                                 * The return value from ChangeDisplaySettings
                                 * should be zero on success, hence we can use
                                 * that to hide the cursor. */                \
                                ShowCursor(ChangeDisplaySettings(             \
                                        JNY8NEKN,                             \
                                        CDS_FULLSCREEN));                     \
                        }                                                     \
                                                                              \
                        NEWWYUG8 = CreateWindow("static", NULL, WS_POPUP |    \
                                                                WS_VISIBLE,   \
                                                0,                            \
                                                0, (WIDTH), (HEIGHT), NULL,   \
                                                NULL, NULL, NULL);            \
                                                                              \
                        C98WXZEW = GetDC(NEWWYUG8);                           \
                        SetPixelFormat(C98WXZEW, ChoosePixelFormat(C98WXZEW,  \
                                                                   JNY8NEKN), \
                                       JNY8NEKN);                             \
                        wglMakeCurrent(C98WXZEW, wglCreateContext(C98WXZEW)); \
                        RI1NFZQ4()

                /* flush is used to flush the renderer's pipeline and update
                 * the contents of the visible framebuffer.
                 *
                 * By default, this macro doesn't include any message
                 * processing code.  This means, with the MINIMAL framework on
                 * Windows, no OS messages are handled.
                 *
                 * However, by defining the FORCE_MESSAGE_PROCESSING symbol
                 * this macro can be modified to contain an additional snippet
                 * of code for receiving and sending OS messages. */
#               if defined (FORCE_MESSAGE_PROCESSING)
#                       define flush()                                        \
                                if (PeekMessage(JNY8NEKN, NEWWYUG8, 0, 0,     \
                                                PM_REMOVE)) {                 \
                                        TranslateMessage(JNY8NEKN);           \
                                        DispatchMessage(JNY8NEKN);            \
                                }                                             \
                                                                              \
                                SwapBuffers(C98WXZEW)
#               else
#                       define flush()          SwapBuffers(C98WXZEW)
#               endif /* defined (FORCE_MESSAGE_PROCESSING) */

                /* key resolves to a function returning the state of the key on
                 * the keyboard, whose (usually) OS-specific key-code is
                 * given. */
#               define key(KEYCODE)             GetAsyncKeyState((KEYCODE))

                /* escape evaluates to the OS-specific function that returns
                 * the state of the escape key.
                 *
                 * This is a simple macro meant for convenience.  If desired,
                 * the macro key with the appropriate key-code can be used in
                 * place of escape as well. */
#               define escape()                 key(VK_ESCAPE)

                /* seconds retrieves the system time, in seconds.  The system
                 * time is the time elapsed since the system was started. */
#               define seconds()                ((float) timeGetTime() * .001f)

                /* The last macro, quit, fires up the termination protocol for
                 * safely exiting the execution of the process.
                 *
                 * If configured to do so, Stun also calls all of the resource
                 * unloaders through this macro. */
#               if defined (EXTEND_RESOURCE_MANAGEMENT)
#                       define quit()                                         \
                                do {                                          \
                                        AS48PNHM();                           \
                                        ExitProcess(0);                       \
                                } while (false)
#               else
#                       define quit()           ExitProcess(0);
#               endif /* defined (EXTEND_RESOURCE_MANAGEMENT) */
#       endif /* SYSTEM == MAC_OS_X */
#elif (FRAMEWORK == GHOST)
        /* The ghost implementation of window.  Just calls the loaders and sets
         * the global window width & height information; does nothing else. */
#       define window(WIDTH, HEIGHT, IS_FULLSCREEN)                           \
                WINDOW_WIDTH = (WIDTH);                                       \
                WINDOW_HEIGHT = (HEIGHT);                                     \
                RI1NFZQ4()

        /* The ghost implementation of flush. */
#       define flush()

        /* The ghost implementation of key. */
#       define key(KEYCODE)

        /* The ghost implementation of escape. */
#       define escape()

        /* The ghost implementation of seconds. */
#       define seconds()

        /* The ghost implementation of quit.  Just calls the unloaders, if
         * necessary; and does nothing else. */
#       if defined (EXTEND_RESOURCE_MANAGEMENT)
#               define quit()           AS48PNHM();
#       else
#               define quit()
#       endif /* defined (EXTEND_RESOURCE_MANAGEMENT) */
#elif (FRAMEWORK == QT)
        /* todo */
#elif (FRAMEWORK == GLFW)
        /* The first one of these macros, window, is used to create an
         * OpenGL-enabled window of the given size.  This window is empowered
         * by the GLFW framework.
         *
         * If IS_FULLSCREEN evaluates to true, then a fullscreen window will be
         * created. */
#       define window(WIDTH, HEIGHT, IS_FULLSCREEN)                           \
                WINDOW_WIDTH = (WIDTH);                                       \
                WINDOW_HEIGHT = (HEIGHT);                                     \
                                                                              \
                glfwInit();                                                   \
                glfwOpenWindow((WIDTH), (HEIGHT), 8, 8, 8, 8, 32, 0,          \
                               (IS_FULLSCREEN) ? GLFW_FULLSCREEN :            \
                                                 GLFW_WINDOW);                \
                RI1NFZQ4()

        /* flush is used to flush the OpenGL pipeline and update the contents
         * of the front-buffer by swapping it with the back-buffer.
         *
         * Additionally, GLFW's internal message processing also takes place
         * through the callees of this macro. */
#       define flush()                          glfwSwapBuffers()

        /* key resolves to a function returning the state of the key on the
         * keyboard, whose GLFW-specific key-code is given. */
#       define key(KEYCODE)                     glfwGetKey((KEYCODE))

        /* escape evaluates to the GLFW-specific function that returns the
         * state of the escape key.
         *
         * This is a simple macro meant for convenience.  If desired, the macro
         * key with the appropriate key-code can be used in place of escape as
         * well. */
#       define escape()                         key(GLFW_KEY_ESC)

        /* seconds retrieves the number of seconds elapsed since the
         * initialization of the GLFW library (or since a call to
         * glfwSetTime). */
#       define seconds()                        glfwGetTime()

        /* The last macro, quit, fires up the termination protocol for safely
         * exiting the execution of the process.
         *
         * If configured to do so, Stun also calls all of the resource
         * unloaders through this macro. */
#       if defined (EXTEND_RESOURCE_MANAGEMENT)
#               define quit()                                                 \
                        do {                                                  \
                                AS48PNHM();                                   \
                                glfwTerminate();                              \
                        } while (false)
#       else
#               define quit()                   glfwTerminate();
#       endif /* defined (EXTEND_RESOURCE_MANAGEMENT) */
#elif (FRAMEWORK == SDL)
        /* todo */
#endif /* FRAMEWORK == MINIMAL */

/* Aside from window management and OS interfacing functionality, Stun also
 * provides some basic memory and resource management features. */
#if defined (FORCE_STANDARD_LIBRARY)
        /* The standard library flavor of allocate. */
#       define allocate(SIZE)                   malloc((SIZE))

        /* The standard library flavor of dispose. */
#       define dispose(POINTER)                 free((POINTER))

        /* The standard library flavor of fill. */
#       define fill(POINTER, VALUE, SIZE)                                     \
                __builtin_memset((POINTER), (VALUE), (SIZE))

        /* The standard library flavor of duplicate. */
#       define duplicate(FROM, TO, SIZE)        memmove((TO), (FROM), (SIZE))
#elif (SYSTEM == MAC_OS_X)
        /* The Carbon flavor of allocate. */
#       define allocate(SIZE)                   (void *) NewPtr((SIZE))

        /* The Carbon flavor of dispose. */
#       define dispose(POINTER)                 (void *) DisposePtr((POINTER))

        /* Carbon doesn't provide any dedicated equivalents for the following
         * functions.
         *
         * This is what Apple says about them: The BlockMove, BlockMoveData,
         * and BlockZero family of functions are not available; use the BSD
         * functions memmove and bzero instead.
         *
         * Hence, these calls either get implemented by Stun (if there
         * is no CRT) or map to the standard library calls of
         * equivalent functionality. */
#       if (FRAMEWORK == MINIMAL)
                /* The custom implementation of fill. */
                void fill(void *pointer, int value, unsigned int size);

                /* The custom implementation of duplicate. */
                void duplicate(void const *from, void *to, unsigned int size);
#       else
                /* The standard library flavor of fill. */
#               define fill(POINTER, VALUE, SIZE)                             \
                        memset((POINTER), (VALUE), (SIZE))

                /* The standard library flavor of duplicate. */
#               define duplicate(FROM, TO, SIZE)                              \
                        memmove((TO), (FROM), (SIZE))
#       endif /* FRAMEWORK == MINIMAL */
#elif (SYSTEM == LINUX)
        /* The standard library flavor of allocate. */
#       define allocate(SIZE)                   malloc((SIZE))

        /* The standard library flavor of dispose. */
#       define dispose(POINTER)                 free((POINTER))

        /* The standard library flavor of fill. */
#       define fill(POINTER, VALUE, SIZE)                                     \
                memset((POINTER), (VALUE), (SIZE))

        /* The standard library flavor of duplicate. */
#       define duplicate(FROM, TO, SIZE)        memmove((TO), (FROM), (SIZE))
#elif (SYSTEM == WINDOWS)
        /* The WinAPI flavor of allocate. */
#       define allocate(SIZE)                                                 \
                HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (SIZE))

        /* The WinAPI flavor of dispose. */
#       define dispose(POINTER)                                               \
                HeapFree(GetProcessHeap(), 0, (POINTER))

        /* On Windows, the dedicated functions depend on memset et al., which
         * are implemented in the CRT (which is not available with the MINIMAL
         * framework).
         *
         * Hence, in that case, we'd need to provide our own versions of these
         * functions.  Otherwise, we can safely use the dedicated versions. */
#       if (FRAMEWORK == MINIMAL)
                /* The custom implementation of fill. */
                void fill(void *pointer, int value, unsigned int size);

                /* The custom implementation of duplicate. */
                void duplicate(void const *from, void *to, unsigned int size);
#       else
                /* The WinAPI flavor of fill. */
#               define fill(POINTER, VALUE, SIZE)                             \
                        FillMemory((POINTER), (SIZE), (VALUE))

                /* The WinAPI flavor of duplicate. */
#               define duplicate(FROM, TO, SIZE)                              \
                        MoveMemory((TO), (FROM), (SIZE))
#       endif /* FRAMEWORK == MINIMAL */
#endif /* defined (FORCE_STANDARD_LIBRARY) */

/* Lastly, zero is a simple convenience macro implemented using fill.  It
 * simply fills the described region of memory with zeros. */
#define zero(POINTER, LENGTH)                   fill((POINTER), 0x00, (LENGTH))

/* Stun provides the options of using either of a framework-specific OpenGL
 * extension loading mechanism or GLEW.  With the GHOST framework, unless using
 * GLEW, the extension are expected to be available. */
#if defined (USE_GLEW_FOR_OPENGL_EXTENSIONS)
        /* We'll be using GLEW; so, everything is smooth and the extension
         * symbols are available through the GLEW library. */
#       define J5F2LXAZ()                       glewInit()
#elif (FRAMEWORK != GHOST)
        /* The macro, extension, resolves to the function pointer at the
         * specified index of the extensions array and serves it after casting
         * it to the correct type.
         *
         * token is the name of the function being addressed in all capitals,
         * and without the GL prefix. */
#       define extension(TOKEN)                                               \
                ((PFNGL ## TOKEN ## PROC) R5UU5MEU[(TOKEN)])

        /* The following is a listing of preprocessor tokens which resolve to
         * the indices of the OpenGL extension function pointers within the
         * extensions array. */
        enum E794JVKW {
                CREATESHADER,
                SHADERSOURCE,
                COMPILESHADER,
                ATTACHSHADER,
                CREATEPROGRAM,
                LINKPROGRAM,
                USEPROGRAM,
                GETUNIFORMLOCATION,
                UNIFORM1I,
                UNIFORM1F,
                UNIFORM1FV,
                UNIFORM2F,
                UNIFORMMATRIX4FV,
                GENRENDERBUFFERS,
                BINDRENDERBUFFER,
                RENDERBUFFERSTORAGE,
                GENFRAMEBUFFERS,
                BINDFRAMEBUFFER,
                FRAMEBUFFERTEXTURE2D,
                FRAMEBUFFERTEXTURELAYER,
                FRAMEBUFFERRENDERBUFFER,
                DRAWBUFFERS,
                ACTIVETEXTURE,
                TEXIMAGE3D,
                BUFFERDATA,
                BINDBUFFER,
                GENBUFFERS,

#               if defined (EXTEND_RESOURCE_MANAGEMENT)
                        DELETESHADER,
                        DELETEPROGRAM,
                        DELETERENDERBUFFERS,
                        DELETEFRAMEBUFFERS,
#               endif /* defined (EXTEND_RESOURCE_MANAGEMENT) */

                NUMBER_OF_EXTENSIONS
        };

        /* This array of pointers holds the function pointers to the OpenGL
         * extension functions which are dynamically loaded. */
        extern void *R5UU5MEU[NUMBER_OF_EXTENSIONS];

        /* Array of strings holding the names of the aforementioned OpenGL
         * extensions that be loaded. */
        static char const *const W2OPFFT9[NUMBER_OF_EXTENSIONS] = {
                "glCreateShader",
                "glShaderSource",
                "glCompileShader",
                "glAttachShader",
                "glCreateProgram",
                "glLinkProgram",
                "glUseProgram",
                "glGetUniformLocation",
                "glUniform1i",
                "glUniform1f",
                "glUniform1fv",
                "glUniform2f",
                "glUniformMatrix4fv",
                "glGenRenderbuffers",
                "glBindRenderbuffer",
                "glRenderbufferStorage",
                "glGenFramebuffers",
                "glBindFramebuffer",
                "glFramebufferTexture2D",
                "glFramebufferTextureLayer",
                "glFramebufferRenderbuffer",
                "glDrawBuffers",
                "glActiveTexture",
                "glTexImage3D",
                "glBufferData",
                "glBindBuffer",
                "glGenBuffers",

#               if defined (EXTEND_RESOURCE_MANAGEMENT)
                        "glDeleteShader",
                        "glDeleteProgram",
                        "glDeleteRenderbuffers",
                        "glDeleteFramebuffers"
#               endif /* defined (EXTEND_RESOURCE_MANAGEMENT) */
        };

#       if (FRAMEWORK == MINIMAL)
#               if (SYSTEM == MAC_OS_X)
                        /* todo */
#               elif (SYSTEM == LINUX)
                        /* On Linux, the glXGetProcAddress function is used for
                         * mapping the extension addresses. */
#                       define MTO3ODSW(NAME)   glXGetProcAddress((NAME))
#               elif (SYSTEM == WINDOWS)
                        /* On Windows, we are using wglGetProcAddress to map
                         * the extension addresses. */
#                       define MTO3ODSW(NAME)   wglGetProcAddress((NAME))
#               endif /* SYSTEM == MAC_OS_X */
#       elif (FRAMEWORK == GLFW)
                /* The GLFW library provides its own, cross-platform, way of
                 * loading OpenGL extensions. */
#               define MTO3ODSW(NAME)           glfwGetProcAddress((NAME))
#       elif (FRAMEWORK == SDL)
                /* Just like GLFW, SDL, too, provides its own, cross-platform
                 * abstraction layer for loading OpenGL extensions. */
#               define MTO3ODSW(NAME)           SDL_GL_GetProcAddress((NAME))
#       endif /* FRAMEWORK == MINIMAL */

        /* The J5F2LXAZ macro resolves to a code snippet that snapshots the
         * respective addresses of all of the OpenGL extension functions Stun
         * is loading. */
#       define J5F2LXAZ()                                                     \
                do {                                                          \
                        int Q1MVZ4Z3 = 0;                                     \
                                                                              \
                        for (; Q1MVZ4Z3 < NUMBER_OF_EXTENSIONS; ++Q1MVZ4Z3) { \
                                R5UU5MEU[Q1MVZ4Z3] =                          \
                                        MTO3ODSW(W2OPFFT9[Q1MVZ4Z3]);         \
                        }                                                     \
                } while (false)

        /* The following are the OpenGL extension functions as mapped to their
         * respective addresses within the OpenGL dynamic library.
         *
         * To keep it simple, stupid; we are using macros instead of actual
         * pointers.  These macros act just like the OpenGL function prototypes
         * and are function-callable. */
#       define glCreateShader                   extension(CREATESHADER)
#       define glShaderSource                   extension(SHADERSOURCE)
#       define glCompileShader                  extension(COMPILESHADER)
#       define glAttachShader                   extension(ATTACHSHADER)
#       define glCreateProgram                  extension(CREATEPROGRAM)
#       define glLinkProgram                    extension(LINKPROGRAM)
#       define glUseProgram                     extension(USEPROGRAM)
#       define glGetUniformLocation             extension(GETUNIFORMLOCATION)
#       define glUniform1i                      extension(UNIFORM1I)
#       define glUniform1f                      extension(UNIFORM1F)
#       define glUniform1fv                     extension(UNIFORM1FV)
#       define glUniform2f                      extension(UNIFORM2F)
#       define glUniformMatrix4fv               extension(UNIFORMMATRIX4FV)
#       define glGenRenderbuffers               extension(GENRENDERBUFFERS)
#       define glBindRenderbuffer               extension(BINDRENDERBUFFER)
#       define glRenderbufferStorage            extension(RENDERBUFFERSTORAGE)
#       define glGenFramebuffers                extension(GENFRAMEBUFFERS)
#       define glBindFramebuffer                extension(BINDFRAMEBUFFER)
#       define glFramebufferTexture2D           extension(FRAMEBUFFERTEXTURE2D)
#       define glFramebufferTextureLayer                                      \
                extension(FRAMEBUFFERTEXTURELAYER)
#       define glFramebufferRenderbuffer                                      \
                extension(FRAMEBUFFERRENDERBUFFER)
#       define glDrawBuffers                    extension(DRAWBUFFERS)
#       define glActiveTexture                  extension(ACTIVETEXTURE)
#       define glTexImage3D                     extension(TEXIMAGE3D)
#       define glBufferData                     extension(BUFFERDATA)
#       define glBindBuffer                     extension(BINDBUFFER)
#       define glGenBuffers                     extension(GENBUFFERS)

#       if defined (EXTEND_RESOURCE_MANAGEMENT)
#               define glDeleteShader           extension(DELETESHADER)
#               define glDeleteProgram          extension(DELETEPROGRAM)
#               define glDeleteRenderbuffers    extension(DELETERENDERBUFFERS)
#               define glDeleteFramebuffers     extension(DELETEFRAMEBUFFERS)
#       endif /* defined (EXTEND_RESOURCE_MANAGEMENT) */
#endif /* defined (USE_GLEW_FOR_OPENGL_EXTENSIONS) */

/* The following type definitions are not meant to be completely individual
 * types with their own characteristics but rather they are meant to put more
 * emphasis on the purpose of the type itself.
 *
 * At certain cases, such as loader_t, they also make typing the actual type
 * considerably simpler. */
typedef unsigned int shader_t;
typedef unsigned int program_t;
typedef unsigned int fbo_t;
typedef unsigned int texture_t;
typedef void (*const loader_t)(void);
typedef loader_t unloader_t;
typedef struct Y3JCIKIM layer_t;
typedef struct I3VGFDNN mesh_t;

/* Represents a post-processing layer.  A layer is a compound type made out of
 * a framebuffer object and a texture attached to it. */
struct Y3JCIKIM {
        fbo_t framebuffer;      /* FBO */
        texture_t attachment;   /* Render-product */

        unsigned int width;     /* attachment's Width */
        unsigned int height;    /* attachment's Height */
};

/* This struct of PODs represents a mesh.  A mesh is a grouping of buffers
 * describing an arbitrary geometry. */
struct I3VGFDNN {
        /* Origin-aligned object vertices. */
        float *vertices;

        /* Tangent space per vertex normals. */
        float *normals;

        /* Tangent space per vertex tangent vectors. */
        float *tangents;

        /* Naturally, one would expect to see the bitangent vectors as well;
         * however, they can be easily derived within a vertex shader using the
         * following simple and straightforward formula:
         *
         *        bitangent = cross(normal, tangent);
         *
         * Stun employs this method to save at least a little from its memory
         * consumption.  It also saves a few bytes in the final executable for
         * not having to send yet another attribute to the shaders. */

        /* Texture space texture mapping coordinates. */
        float *uvw_coordinates;

        /* The indices of the object.  Each group of three indices describe a
         * face made out of the corresponding vertices from the vertices
         * array. */
        unsigned int *indices;

        /* The number of triangular faces the object has. */
        unsigned int number_of_faces;

        /* The number of vertex attributes the object has. */
        unsigned int number_of_vertices;
};

/* The array of function pointers which point to the loader functions to be
 * called by the loading system. */
extern loader_t P744X1EL[NUMBER_OF_LOADERS];

#if defined (EXTEND_RESOURCE_MANAGEMENT)
        /* The array of function pointers which point to the resource
         * deallocation and unloading functions to be called by the loading
         * system. */
        extern unloader_t FDC2CHR7[NUMBER_OF_LOADERS];
#endif /* defined (EXTEND_RESOURCE_MANAGEMENT) */

/* This is the FBO that is used while generating meshes.  It has a certain
 * number textures attached to it.
 *
 * After the mesh generation process, it might be used for other purposes as
 * well.  Such as the G-buffer by logically changing the purpose of the texture
 * attachments, or by adding new attachments if necessary. */
extern fbo_t HVATQD0L;

/* This is the OpenGL ID for the precompiled default vertex shader.  It simply
 * functions as a pass-through shader and sends up to four UVW-coordinates down
 * the pipeline. */
extern shader_t MUANXLC;

#if defined (GENERATE_ACCUMULATION_LAYER)
        /* This is the automatically created post-processing layer that is used
         * as the accumulation layer for the render products. */
        extern layer_t *P4E6JEEC;
#endif /* defined (GENERATE_ACCUMULATION_LAYER) */

#if (FRAMEWORK != MINIMAL)
        /* If a FRAMEWORK other than MINIMAL is used, then Stun will require
         * some extra storage space for storing the width and height
         * information of a possible window. */
        extern int TVJAYSLH[2];
#endif /* FRAMEWORK != MINIMAL */

/* This array is meant for storing the OpenGL IDs of the GPU programs that are
 * generic and well-defined.  Due to these qualities Stun automates the process
 * of compiling and linking these shaders.
 *
 * Even though the contents of this array can be accessed from elsewhere, it is
 * mean for internal use only. */
extern program_t FOBIMQQB[ON3400B7];

/* This is the resource loading system of Stun.  Its purpose is to trigger
 * resource generation and allocation functions.  It is also used to load some
 * of the features used by Stun (such as OpenGL extensions).
 *
 * This macro doesn't need to be called manually as it is handled by window. */
#define RI1NFZQ4()                                                            \
        do {                                                                  \
                int i = 0;                                                    \
                                                                              \
                float x = (float) (((WINDOW_WIDTH) - 512) >> 1);              \
                float y = (float) (((WINDOW_HEIGHT) - 4) >> 1);               \
                                                                              \
                float k = seconds();                                          \
                float l = 0.f;                                                \
                                                                              \
                while (true) {                                                \
                        l = since(k);                                         \
                                                                              \
                        if (l >= 2.f) {                                       \
                                break;                                        \
                        }                                                     \
                                                                              \
                        if (i < NUMBER_OF_LOADERS) {                          \
                                (P744X1EL[i])();                              \
                                ++i;                                          \
                        }                                                     \
                                                                              \
                        orthographic((WINDOW_WIDTH), (WINDOW_HEIGHT));        \
                        glClear(GL_COLOR_BUFFER_BIT);                         \
                                                                              \
                        glRectf(x - 2.f, y - 2.f, x + 514.f, y + 6.f);        \
                                                                              \
                        glColor3ub(0, 0, 0);                                  \
                        glRectf(x - 1.f, y - 1.f, x + 513.f, y + 5.f);        \
                                                                              \
                        glColor3ub(255, 255, 255);                            \
                        glRectf(x, y, x + l * 256.f, y + 4.f);                \
                                                                              \
                        flush();                                              \
                }                                                             \
        } while (false)

/* And this is the resource unloading and deallocation system of Stun.  Just
 * like its cousin above, its purpose is to trigger a bunch of functions.
 *
 * This macro doesn't need to be called manually as it is handled by quit. */
#define AS48PNHM()                                                            \
        do {                                                                  \
                int i = 0;                                                    \
                                                                              \
                float x = (float) (((WINDOW_WIDTH) - 512) >> 1);              \
                float y = (float) (((WINDOW_HEIGHT) - 4) >> 1);               \
                                                                              \
                float k = seconds();                                          \
                float l = 0.f;                                                \
                                                                              \
                glDisable(GL_DEPTH_TEST);                                     \
                                                                              \
                while (true) {                                                \
                        l = since(k);                                         \
                                                                              \
                        if (l >= 2.f) {                                       \
                                break;                                        \
                        }                                                     \
                                                                              \
                        if (i < NUMBER_OF_LOADERS) {                          \
                                (FDC2CHR7[i])();                              \
                                ++i;                                          \
                        }                                                     \
                                                                              \
                        orthographic((WINDOW_WIDTH), (WINDOW_HEIGHT));        \
                        glClear(GL_COLOR_BUFFER_BIT);                         \
                                                                              \
                        glRectf(x - 2.f, y - 2.f, x + 514.f, y + 6.f);        \
                                                                              \
                        glColor3ub(0, 0, 0);                                  \
                        glRectf(x - 1.f, y - 1.f, x + 513.f, y + 5.f);        \
                                                                              \
                        glColor3ub(255, 255, 255);                            \
                        glRectf(x, y, x + (512.f - l * 256.f), y + 4.f);      \
                                                                              \
                        flush();                                              \
                }                                                             \
        } while (false)

/* shader creates, sets the source code in and compiles a shader.  The shader
 * identifier is returned from the function. */
shader_t shader(char const *source, int type);

/* The following is the infamous stitcher, by Goksel, made into a proper
 * utility.  It was born with the final version of Youth Uprising's Muon Baryon
 * as a result of fifteen minutes of hacking, then it was re-abused by Jake in
 * Magnus and apparently also in Ethos.
 *
 * What it does is to maintain a list of pointers to the individual AND unique
 * shader fragments (stitches).  One can then use these stitches to stitch a
 * rope of shader strings.
 *
 * And this function is the entire brain of the stitching utility.  It accepts
 * an array of integers (possibly 8-bit too) which are indices of the shader
 * stitches in the internally maintained array of the stitcher.  A special
 * marker (END_STITCH_STREAM) indicates the end of the indices stream.
 *
 * It then constructs an array of pointers to the strings so that the fragments
 * are laid out in a way when interpreted linearly they construct the desired
 * shader source.  Finally it passes this array to glShaderSource and let's the
 * action begin.
 *
 * Any shader stitch can be used for any shader as long as the said shader
 * supports all the functions utilized (as in no texture2D in vertex or
 * geometry shaders).
 *
 * The second parameter of this function is the type of the shader.  It can be
 * GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER or GL_VERTEX_SHADER (or well,
 * anything else that OpenGL supports).
 *
 * In the end, this function returns the OpenGL shader-id of the shader after
 * compiling it. */
shader_t stitch(unsigned char const *indices, int type);

/* program creates a program to which it attaches the given vertex, fragment
 * and geometry shaders.  Afterwards it returns the program id after linking
 * it.
 *
 * If you don't want to have any of the three shaders, simply set the
 * identifier to DISCARD.  However, at least a fragment shader is necessary. */
program_t program(shader_t vertex, shader_t geometry, shader_t fragment);

/* pixel_shader sets up a GPU program from the given shader source code to be
 * used in post-processing. */
#define pixel_shader(SOURCE)                                                  \
        program(DISCARD, DISCARD, shader((SOURCE), GL_FRAGMENT_SHADER))

/* The texture function allocates an OpenGL name for a texture object as well
 * as the video memory requested by the texture. */
texture_t texture(unsigned int width, unsigned int height, int level,
                  int internalFormat, int format, int type, void *data);

/* The layer function creates a post-processing layer; essentially an FBO with
 * a predefined texture of the specified size attached to it.  In contrast,
 * layers don't sport a fancy depth buffer. */
layer_t *layer(unsigned int width, unsigned int height);

/* The depth function provides a special kind of a layer; an FBO with a
 * predefined texture of the specified size attached to it.  But, differently
 * from the layer function the texture is a depth attachment. */
layer_t *depth(unsigned int width, unsigned int height);

/* The LINEAR_DEPTH macro resolves to a string containing a simple formula for
 * linearizing a given logarithmic depth value (such as gl_FragCoord.z).
 *
 * The macro is for string generation use only and doesn't resolve to any code
 * snippet that actually calculates a formula. */
#define LINEAR_DEPTH(DEPTH)                                                   \
        LIWQTX2O "/(" O7TNQ83D "-" DEPTH "*" GUM5GQZZ ")"

/* The mesh function is the implementation of a crazy idea that does hopefully
 * work without causing any harm.
 *
 * The idea is to stream-out the GPU calculated 3D meshes onto the RAM.  This
 * is achieved by laying out quads on an orthographically projected canvas in
 * such a way that every vertex of each quad gets to be drawn but the quads
 * that are not on the edges consist entirely of shared vertices.
 *
 * Which means, logically, our mesh has a continuous initial surface; and the
 * modelling GPU program can take advantage of this.
 *
 * This layout also renders it possible to actually modify the geometry within
 * the fragment shader at a per-vertex level, too.
 *
 * Then a GPU program is ran over this grid.  The key point of the GPU program
 * is that it doesn't modify the actual geometry but rather stores the
 * transformation of each vertex of each quad at its corresponding pixel on the
 * canvas (= framebuffer).
 *
 * On top of that the GPU program also calculates and stores the texturing
 * coordinates of the geometry and stores them into a separate render-target.
 *
 * Finally these values are read back to the RAM using 'glReadPixels' and are
 * re-ordered.  Additionally, if Stun is configured to do so, some other
 * features are also automatically calculated.
 *
 * This is all, after this your GPU modelled mesh is ready to be rendered in
 * realtime with no further calculations required! */
mesh_t *mesh(program_t gpu_program, unsigned int width, unsigned int height);

/* Given a mesh, the compute_vertex_attributes function generates certain
 * vertex attributes (such as the vertex normals and the vertex tangents) using
 * the topology of the provided mesh.
 *
 * These newly generated values are then stored in the corresponding buffers of
 * the specified mesh. */
void compute_vertex_attributes(mesh_t *mesh);

/* Given a mesh and a tolerance value, this function consolidates the given
 * mesh by removing any logical duplicates of a vertex.
 *
 * A vertex is marked as a logical duplicate if it is within a certain radius
 * of another vertex; the radius is specified by the tolerance value.
 *
 * The corresponding buffers of the given mesh are then replaced by these newly
 * modified buffers. */
void consolidate(mesh_t *mesh, float tolerance);

/* Given a mesh, this function simply asks OpenGL to render it.  Note that it
 * doesn't perform any translations, rotations or scaling. */
void render(mesh_t const *mesh);

/* perspective sets up a perspective projection with the given width, height
 * and aspect ratio.  It also resizes the viewport accordingly. */
void perspective(unsigned int width, unsigned int height, float aspect_ratio);

/* orthographic sets up a two-dimensional orthographic projection with the
 * given width and height.  Again, it also resizes the viewport accordingly. */
void orthographic(unsigned int width, unsigned int height);

/* The since macro retrieves the time elapsed since the specified time.  The
 * resultant value is in fractional seconds. */
#define since(TIME)                             (seconds() - (TIME))

/* The is_interval_hit function returns true every n seconds where n is the
 * specified amount of seconds.
 *
 * It also snapshots the current time and stores it in timer; guaranteeing
 * continuous action. */
int is_interval_hit(float *timer, float interval);

/* The quad function draws a unit-textured quad of the specified width and
 * height. */
void quad(float width, float height);

/* far_plane renders a 2D orthographic quad whose texturing coordinates
 * correspond to the view-space x and y values of the frustum's far-plane.
 *
 * The interpolated texturing coordinates would be amazingly useful for
 * constructing a camera ray from the eye point towards the far-plane passing
 * through each and every pixel on the screen. */
void far_plane(unsigned short int width, unsigned short int height);

/* The generate function generates the elements of the given destination layer
 * using the output from the supplied GPU program. */
void generate(layer_t *destination, program_t generator);

/* The transform function, as its name suggests, transforms the elements of the
 * given source layer via the supplied GPU program and then writes the newly
 * computed render products into the destination layer. */
void transform(layer_t *source, layer_t *destination, program_t transformer);

/* The blit function simply blits the elements of the source layer onto the
 * destination layer using the fixed-function pipeline.
 *
 * If the sizes of the layers involved are different then the source layer is
 * linearly scaled to fit the size of the destination layer. */
void blit(layer_t *source, layer_t *destination);

/* The blit_to_back_buffer function is a specialization of the functionality
 * provided by the blit function.
 *
 * Given a source layer, this function blits the contents of the layer to the
 * back-buffer, scaling the texture to cover the entire window. */
void blit_to_back_buffer(layer_t *source);

/* The gaussian_blur macro runs a separable, two-pass Gaussian Blur filter on
 * the elements of the given layer.
 *
 * The result is stored in the original, first layer; the second one is used
 * for ping-ponging. */
#define gaussian_blur(LAYER, TEMPORARY)                                       \
        do {                                                                  \
                glUseProgram(FOBIMQQB[0]);                                    \
                glUniform1f(glGetUniformLocation(FOBIMQQB[0], "m"),           \
                            (LAYER)->height);                                 \
                glUseProgram(FOBIMQQB[1]);                                    \
                glUniform1f(glGetUniformLocation(FOBIMQQB[1], "m"),           \
                            (TEMPORARY)->width);                              \
                                                                              \
                transform((LAYER), (TEMPORARY), FOBIMQQB[0]);                 \
                transform((TEMPORARY), (LAYER), FOBIMQQB[1]);                 \
        } while (false)

/* The white_noise macro resolves to a very simple snippet that fills the
 * contents of the supplied layer with a white noise. */
#define white_noise(LAYER)                      generate((LAYER), FOBIMQQB[2])

/* The white_noise macro resolves to a very simple snippet that fills the
 * contents of the supplied layer with a simplex noise.  Certain noise
 * properties such as its frequency, number of octaves and amplitude are also
 * controllable through this macro. */
#define simplex_noise(LAYER, FREQUENCY, OCTAVES, AMPLITUDE)                   \
        do {                                                                  \
                glUseProgram(FOBIMQQB[3]);                                    \
                glUniform1f(glGetUniformLocation(FOBIMQQB[3], "f"),           \
                            (FREQUENCY));                                     \
                glUniform1f(glGetUniformLocation(FOBIMQQB[3], "o"),           \
                            (OCTAVES));                                       \
                glUniform1f(glGetUniformLocation(FOBIMQQB[3], "a"),           \
                            (AMPLITUDE));                                     \
                                                                              \
                generate((LAYER), FOBIMQQB[3]);                               \
        } while (false)

#if defined (ACCUMULATION)
        /* The bind_accumulation_layer macro resolves to a basic snippet that
         * binds ACCUMULATION's framebuffer as the target framebuffer that will
         * receive the render products. */
#       define bind_accumulation_layer()                                      \
                glBindFramebuffer(GL_FRAMEBUFFER, ACCUMULATION->framebuffer)
#endif /* defined (ACCUMULATION) */

#if defined (ENABLE_MATHEMATICS_SUPPORT)
        /* Calculates the second power (square) of the given value. */
#       define square(VALUE)                    ((VALUE) * (VALUE))

        /* Given a coordinate and a standard deviation, this macro calculates
         * the one-dimensional Gaussian distribution function. */
#       define gaussian(X, STANDARD_DEVIATION)                                \
                (expf(-square((X)) / (2.f * square((STANDARD_DEVIATION)))) /  \
                ((STANDARD_DEVIATION) * 2.506628f))

        /* This function, as its name suggests, generates an array of Gaussian
         * weights.  This array can be extremely useful for doing things like
         * Gaussian-blur. */
        float *generate_gaussian_weights(float standard_deviation);
#endif /* defined (ENABLE_MATHEMATICS_SUPPORT) */

/* On certain platforms, independent of the framework in use, some headers are
 * needed for providing the required function prototypes. */
#if (SYSTEM == MAC_OS_X)
#       include <Carbon/Carbon.h>
#elif (SYSTEM == LINUX)
        /* todo */
#elif (SYSTEM == WINDOWS)
        /* It is necessary to undefine the BOOL symbol as it is occupied, and
         * hence defined, by WinAPI's winbase.h header. */
#       undef BOOL
#       include <windows.h>
#endif /* SYSTEM == MAC_OS_X */

#if defined (FORCE_STANDARD_LIBRARY)
#       include <stdlib.h>
#endif /* defined (FORCE_STANDARD_LIBRARY) */

#if defined (ENABLE_MATHEMATICS_SUPPORT)
#       if defined (__cplusplus)
#               include <cmath>
#       else
#               include <math.h>
#       endif /* defined (__cplusplus) */
#endif /* defined (ENABLE_MATHEMATICS_SUPPORT) */

#if defined (USE_GLEW_FOR_OPENGL_EXTENSIONS)
#       include <GL/glew.h>
#endif /* defined (USE_GLEW_FOR_OPENGL_EXTENSIONS) */

/* Obviously, all of these different frameworks and different platforms have
 * different header files that define their APIs.  And obviously, we'll need
 * them in order to interface their features.
 *
 * To save the library user from all of that hassle, Stun.h includes these
 * header files as required by their frameworks. */
#if (FRAMEWORK == MINIMAL)
#       if (SYSTEM == MAC_OS_X)
                /* todo */
#       elif (SYSTEM == LINUX)
#               if defined (USE_BLEEDING_EDGE_OPENGL)
#                       include <GL/gl3.h>
#               else
#                       include <GL/gl.h>
#               endif /* defined (USE_BLEEDING_EDGE_OPENGL) */
#               include <GL/glext.h>
#               include <GL/glxext.h>
#       elif (SYSTEM == WINDOWS)
#               if defined (USE_BLEEDING_EDGE_OPENGL)
#                       include <GL/gl3.h>
#               else
#                       include <GL/gl.h>
#               endif /* defined (USE_BLEEDING_EDGE_OPENGL) */
#               include <GL/glext.h>
#               include <GL/wglext.h>
#       endif /* SYSTEM == MAC_OS_X */
#elif (FRAMEWORK == QT)
#       if defined (__cplusplus)
#               include <QtOpenGL>
#               include <QtGui>
#       endif /* defined (__cplusplus) */
#elif (FRAMEWORK == GLFW)
#       include <GL/glfw.h>
#elif (FRAMEWORK == SDL)
#       include <SDL/SDL.h>
#       include <SDL/SDL_opengl.h>
#endif /* FRAMEWORK == MINIMAL */
#endif /* STUN_H_S4YXGLYO */
