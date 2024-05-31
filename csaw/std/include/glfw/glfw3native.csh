#
 GLFW 3.5 - www.glfw.org
 A library for OpenGL, window and input
------------------------------------------------------------------------
 Copyright (c) 2002-2006 Marcus Geelnard
 Copyright (c) 2006-2018 Camilla Löwy <elmindreda@glfw.org>

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would
    be appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not
    be misrepresented as being the original software.

 3. This notice may not be removed or altered from any source
    distribution.
#

def GLFWmonitor;
def GLFWwindow;
def HWND;
def HGLRC;
def CGDirectDisplayID;
def id;
def Display;
def RRCrtc;
def RROutput;
def GLXContext;
def GLXWindow;
def wl_display;
def wl_output;
def wl_surface;
def EGLDisplay;
def EGLContext;
def EGLSurface;
def OSMesaContext;

@[c]glfwGetWin32Adapter(monitor: GLFWmonitor*): int8*;
@[c]glfwGetWin32Monitor(monitor: GLFWmonitor*): int8*;
@[c]glfwGetWin32Window(window: GLFWwindow*): HWND;
@[c]glfwGetWGLContext(window: GLFWwindow*): HGLRC;
@[c]glfwGetCocoaMonitor(monitor: GLFWmonitor*): CGDirectDisplayID;
@[c]glfwGetCocoaWindow(window: GLFWwindow*): id;
@[c]glfwGetCocoaView(window: GLFWwindow*): id;
@[c]glfwGetNSGLContext(window: GLFWwindow*): id;
@[c]glfwGetX11Display::Display*;
@[c]glfwGetX11Adapter(monitor: GLFWmonitor*): RRCrtc;
@[c]glfwGetX11Monitor(monitor: GLFWmonitor*): RROutput;
@[c]glfwGetX11Window(window: GLFWwindow*): Window;
@[c]glfwSetX11SelectionString(string: int8*): void;
@[c]glfwGetX11SelectionString::int8*;
@[c]glfwGetGLXContext(window: GLFWwindow*): GLXContext;
@[c]glfwGetGLXWindow(window: GLFWwindow*): GLXWindow;
@[c]glfwGetWaylandDisplay::wl_display*;
@[c]glfwGetWaylandMonitor(monitor: GLFWmonitor*): wl_output*;
@[c]glfwGetWaylandWindow(window: GLFWwindow*): wl_surface*;
@[c]glfwGetEGLDisplay::EGLDisplay;
@[c]glfwGetEGLContext(window: GLFWwindow*): EGLContext;
@[c]glfwGetEGLSurface(window: GLFWwindow*): EGLSurface;
@[c]glfwGetOSMesaColorBuffer(window: GLFWwindow*, width: int32*, height: int32*, format: int32*, buffer: void**): int32;
@[c]glfwGetOSMesaDepthBuffer(window: GLFWwindow*, width: int32*, height: int32*, bytesPerValue: int32*, buffer: void**): int32;
@[c]glfwGetOSMesaContext(window: GLFWwindow*): OSMesaContext;
