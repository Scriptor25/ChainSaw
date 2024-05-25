# WGL #

def BOOL = int8;
def CHAR = int8;
def DWORD = int32;
def FLOAT = flt32;
def HANDLE;
def HDC;
def HENHMETAFILE;
def HGLRC;
def INT = int32;
def INT64 = int64;
def LPCSTR = int8*;
def LPGLYPHMETRICSFLOAT = void*;
def LPVOID = void*;
def PROC = void*;
def RECT;
def UINT = int32;
def VOID = void;

WGL_FONT_LINES: int32      = 0;
WGL_FONT_POLYGONS: int32   = 1;
WGL_SWAP_MAIN_PLANE: int32 = 0x00000001;
WGL_SWAP_OVERLAY1: int32   = 0x00000002;
WGL_SWAP_OVERLAY2: int32   = 0x00000004;
WGL_SWAP_OVERLAY3: int32   = 0x00000008;
WGL_SWAP_OVERLAY4: int32   = 0x00000010;
WGL_SWAP_OVERLAY5: int32   = 0x00000020;
WGL_SWAP_OVERLAY6: int32   = 0x00000040;
WGL_SWAP_OVERLAY7: int32   = 0x00000080;
WGL_SWAP_OVERLAY8: int32   = 0x00000100;
WGL_SWAP_OVERLAY9: int32   = 0x00000200;
WGL_SWAP_OVERLAY10: int32  = 0x00000400;
WGL_SWAP_OVERLAY11: int32  = 0x00000800;
WGL_SWAP_OVERLAY12: int32  = 0x00001000;
WGL_SWAP_OVERLAY13: int32  = 0x00002000;
WGL_SWAP_OVERLAY14: int32  = 0x00004000;
WGL_SWAP_OVERLAY15: int32  = 0x00008000;
WGL_SWAP_UNDERLAY1: int32  = 0x00010000;
WGL_SWAP_UNDERLAY2: int32  = 0x00020000;
WGL_SWAP_UNDERLAY3: int32  = 0x00040000;
WGL_SWAP_UNDERLAY4: int32  = 0x00080000;
WGL_SWAP_UNDERLAY5: int32  = 0x00100000;
WGL_SWAP_UNDERLAY6: int32  = 0x00200000;
WGL_SWAP_UNDERLAY7: int32  = 0x00400000;
WGL_SWAP_UNDERLAY8: int32  = 0x00800000;
WGL_SWAP_UNDERLAY9: int32  = 0x01000000;
WGL_SWAP_UNDERLAY10: int32 = 0x02000000;
WGL_SWAP_UNDERLAY11: int32 = 0x04000000;
WGL_SWAP_UNDERLAY12: int32 = 0x08000000;
WGL_SWAP_UNDERLAY13: int32 = 0x10000000;
WGL_SWAP_UNDERLAY14: int32 = 0x20000000;
WGL_SWAP_UNDERLAY15: int32 = 0x40000000;

def PFNCHOOSEPIXELFORMATPROC = (hDc: HDC, pPfd: PIXELFORMATDESCRIPTOR*)(int32)*;
def PFNDESCRIBEPIXELFORMATPROC = (hdc: HDC, ipfd: int32, cjpfd: UINT, ppfd: PIXELFORMATDESCRIPTOR*)(int32)*;
def PFNGETENHMETAFILEPIXELFORMATPROC = (hemf: HENHMETAFILE, cbBuffer: UINT, ppfd: PIXELFORMATDESCRIPTOR*)(UINT)*;
def PFNGETPIXELFORMATPROC = (hdc: HDC)(int32)*;
def PFNSETPIXELFORMATPROC = (hdc: HDC, ipfd: int32, ppfd: PIXELFORMATDESCRIPTOR*)(BOOL)*;
def PFNSWAPBUFFERSPROC = (hdc: HDC)(BOOL)*;
def PFNWGLCOPYCONTEXTPROC = (hglrcSrc: HGLRC, hglrcDst: HGLRC, mask: UINT)(BOOL)*;
def PFNWGLCREATECONTEXTPROC = (hDc: HDC)(HGLRC)*;
def PFNWGLCREATELAYERCONTEXTPROC = (hDc: HDC, level: int32)(HGLRC)*;
def PFNWGLDELETECONTEXTPROC = (oldContext: HGLRC)(BOOL)*;
def PFNWGLDESCRIBELAYERPLANEPROC = (hDc: HDC, pixelFormat: int32, layerPlane: int32, nBytes: UINT, plpd: LAYERPLANEDESCRIPTOR*)(BOOL)*;
def PFNWGLGETCURRENTCONTEXTPROC = ()(HGLRC)*;
def PFNWGLGETCURRENTDCPROC = ()(HDC)*;
def PFNWGLGETLAYERPALETTEENTRIESPROC = (hdc: HDC, iLayerPlane: int32, iStart: int32, cEntries: int32, pcr: COLORREF*)(int32)*;
def PFNWGLGETPROCADDRESSPROC = (lpszProc: LPCSTR)(PROC)*;
def PFNWGLMAKECURRENTPROC = (hDc: HDC, newContext: HGLRC)(BOOL)*;
def PFNWGLREALIZELAYERPALETTEPROC = (hdc: HDC, iLayerPlane: int32, bRealize: BOOL)(BOOL)*;
def PFNWGLSETLAYERPALETTEENTRIESPROC = (hdc: HDC, iLayerPlane: int32, iStart: int32, cEntries: int32, pcr: COLORREF*)(int32)*;
def PFNWGLSHARELISTSPROC = (hrcSrvShare: HGLRC, hrcSrvSource: HGLRC)(BOOL)*;
def PFNWGLSWAPLAYERBUFFERSPROC = (hdc: HDC, fuFlags: UINT)(BOOL)*;
def PFNWGLUSEFONTBITMAPSPROC = (hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD)(BOOL)*;
def PFNWGLUSEFONTBITMAPSAPROC = (hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD)(BOOL)*;
def PFNWGLUSEFONTBITMAPSWPROC = (hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD)(BOOL)*;
def PFNWGLUSEFONTOUTLINESPROC = (hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD, deviation: FLOAT, extrusion: FLOAT, format: int32, lpgmf: LPGLYPHMETRICSFLOAT)(BOOL)*;
def PFNWGLUSEFONTOUTLINESAPROC = (hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD, deviation: FLOAT, extrusion: FLOAT, format: int32, lpgmf: LPGLYPHMETRICSFLOAT)(BOOL)*;
def PFNWGLUSEFONTOUTLINESWPROC = (hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD, deviation: FLOAT, extrusion: FLOAT, format: int32, lpgmf: LPGLYPHMETRICSFLOAT)(BOOL)*;

@[c]ChoosePixelFormat(hDc: HDC, pPfd: PIXELFORMATDESCRIPTOR*): int32;
@[c]DescribePixelFormat(hdc: HDC, ipfd: int32, cjpfd: UINT, ppfd: PIXELFORMATDESCRIPTOR*): int32;
@[c]GetEnhMetaFilePixelFormat(hemf: HENHMETAFILE, cbBuffer: UINT, ppfd: PIXELFORMATDESCRIPTOR*): UINT;
@[c]GetPixelFormat(hdc: HDC): int32;
@[c]SetPixelFormat(hdc: HDC, ipfd: int32, ppfd: PIXELFORMATDESCRIPTOR*): BOOL;
@[c]SwapBuffers(hdc: HDC): BOOL;
@[c]wglCopyContext(hglrcSrc: HGLRC, hglrcDst: HGLRC, mask: UINT): BOOL;
@[c]wglCreateContext(hDc: HDC): HGLRC;
@[c]wglCreateLayerContext(hDc: HDC, level: int32): HGLRC;
@[c]wglDeleteContext(oldContext: HGLRC): BOOL;
@[c]wglDescribeLayerPlane(hDc: HDC, pixelFormat: int32, layerPlane: int32, nBytes: UINT, plpd: LAYERPLANEDESCRIPTOR*): BOOL;
@[c]wglGetCurrentContext::HGLRC;
@[c]wglGetCurrentDC::HDC;
@[c]wglGetLayerPaletteEntries(hdc: HDC, iLayerPlane: int32, iStart: int32, cEntries: int32, pcr: COLORREF*): int32;
@[c]wglGetProcAddress(lpszProc: LPCSTR): PROC;
@[c]wglMakeCurrent(hDc: HDC, newContext: HGLRC): BOOL;
@[c]wglRealizeLayerPalette(hdc: HDC, iLayerPlane: int32, bRealize: BOOL): BOOL;
@[c]wglSetLayerPaletteEntries(hdc: HDC, iLayerPlane: int32, iStart: int32, cEntries: int32, pcr: COLORREF*): int32;
@[c]wglShareLists(hrcSrvShare: HGLRC, hrcSrvSource: HGLRC): BOOL;
@[c]wglSwapLayerBuffers(hdc: HDC, fuFlags: UINT): BOOL;
@[c]wglUseFontBitmaps(hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD): BOOL;
@[c]wglUseFontBitmapsA(hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD): BOOL;
@[c]wglUseFontBitmapsW(hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD): BOOL;
@[c]wglUseFontOutlines(hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD, deviation: FLOAT, extrusion: FLOAT, format: int32, lpgmf: LPGLYPHMETRICSFLOAT): BOOL;
@[c]wglUseFontOutlinesA(hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD, deviation: FLOAT, extrusion: FLOAT, format: int32, lpgmf: LPGLYPHMETRICSFLOAT): BOOL;
@[c]wglUseFontOutlinesW(hDC: HDC, first: DWORD, count: DWORD, listBase: DWORD, deviation: FLOAT, extrusion: FLOAT, format: int32, lpgmf: LPGLYPHMETRICSFLOAT): BOOL;

WGL_FRONT_COLOR_BUFFER_BIT_ARB: int32 = 0x00000001;
WGL_BACK_COLOR_BUFFER_BIT_ARB: int32  = 0x00000002;
WGL_DEPTH_BUFFER_BIT_ARB: int32       = 0x00000004;
WGL_STENCIL_BUFFER_BIT_ARB: int32     = 0x00000008;

def PFNWGLCREATEBUFFERREGIONARBPROC = (hDC: HDC, iLayerPlane: int32, uType: UINT)(HANDLE)*;
def PFNWGLDELETEBUFFERREGIONARBPROC = (hRegion: HANDLE)(VOID)*;
def PFNWGLSAVEBUFFERREGIONARBPROC = (hRegion: HANDLE, x: int32, y: int32, width: int32, height: int32)(BOOL)*;
def PFNWGLRESTOREBUFFERREGIONARBPROC = (hRegion: HANDLE, x: int32, y: int32, width: int32, height: int32, xSrc: int32, ySrc: int32)(BOOL)*;

@[c]wglCreateBufferRegionARB(hDC: HDC, iLayerPlane: int32, uType: UINT): HANDLE;
@[c]wglDeleteBufferRegionARB(hRegion: HANDLE): VOID;
@[c]wglSaveBufferRegionARB(hRegion: HANDLE, x: int32, y: int32, width: int32, height: int32): BOOL;
@[c]wglRestoreBufferRegionARB(hRegion: HANDLE, x: int32, y: int32, width: int32, height: int32, xSrc: int32, ySrc: int32): BOOL;

WGL_CONTEXT_RELEASE_BEHAVIOR_ARB: int32       = 0x2097;
WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB: int32  = 0;
WGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB: int32 = 0x2098;

WGL_CONTEXT_DEBUG_BIT_ARB: int32              = 0x00000001;
WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB: int32 = 0x00000002;
WGL_CONTEXT_MAJOR_VERSION_ARB: int32          = 0x2091;
WGL_CONTEXT_MINOR_VERSION_ARB: int32          = 0x2092;
WGL_CONTEXT_LAYER_PLANE_ARB: int32            = 0x2093;
WGL_CONTEXT_FLAGS_ARB: int32                  = 0x2094;
ERROR_INVALID_VERSION_ARB: int32              = 0x2095;

def PFNWGLCREATECONTEXTATTRIBSARBPROC = (hDC: HDC, hShareContext: HGLRC, attribList: int32*)(HGLRC)*;

@[c]wglCreateContextAttribsARB(hDC: HDC, hShareContext: HGLRC, attribList: int32*): HGLRC;

WGL_CONTEXT_OPENGL_NO_ERROR_ARB: int32 = 0x31B3;

WGL_CONTEXT_PROFILE_MASK_ARB: int32              = 0x9126;
WGL_CONTEXT_CORE_PROFILE_BIT_ARB: int32          = 0x00000001;
WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB: int32 = 0x00000002;
ERROR_INVALID_PROFILE_ARB: int32                 = 0x2096;

WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB: int32           = 0x00000004;
WGL_LOSE_CONTEXT_ON_RESET_ARB: int32               = 0x8252;
WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB: int32 = 0x8256;
WGL_NO_RESET_NOTIFICATION_ARB: int32               = 0x8261;

def PFNWGLGETEXTENSIONSSTRINGARBPROC = (hdc: HDC)(char*)*;

@[c]wglGetExtensionsStringARB(hdc: HDC): char*;

WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB: int32 = 0x20A9;

ERROR_INVALID_PIXEL_TYPE_ARB: int32           = 0x2043;
ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB: int32 = 0x2054;

def PFNWGLMAKECONTEXTCURRENTARBPROC = (hDrawDC: HDC, hReadDC: HDC, hglrc: HGLRC)(BOOL)*;
def PFNWGLGETCURRENTREADDCARBPROC = ()(HDC)*;

@[c]wglMakeContextCurrentARB(hDrawDC: HDC, hReadDC: HDC, hglrc: HGLRC): BOOL;
@[c]wglGetCurrentReadDCARB::HDC;

WGL_SAMPLE_BUFFERS_ARB: int32 = 0x2041;
WGL_SAMPLES_ARB: int32        = 0x2042;

def HPBUFFERARB__ { unused: int32 }
def HPBUFFERARB = HPBUFFERARB__*;

WGL_DRAW_TO_PBUFFER_ARB: int32    = 0x202D;
WGL_MAX_PBUFFER_PIXELS_ARB: int32 = 0x202E;
WGL_MAX_PBUFFER_WIDTH_ARB: int32  = 0x202F;
WGL_MAX_PBUFFER_HEIGHT_ARB: int32 = 0x2030;
WGL_PBUFFER_LARGEST_ARB: int32    = 0x2033;
WGL_PBUFFER_WIDTH_ARB: int32      = 0x2034;
WGL_PBUFFER_HEIGHT_ARB: int32     = 0x2035;
WGL_PBUFFER_LOST_ARB: int32       = 0x2036;

def PFNWGLCREATEPBUFFERARBPROC = (hDC: HDC, iPixelFormat: int32, iWidth: int32, iHeight: int32, piAttribList: int32*)(HPBUFFERARB)*;
def PFNWGLGETPBUFFERDCARBPROC = (hPbuffer: HPBUFFERARB)(HDC)*;
def PFNWGLRELEASEPBUFFERDCARBPROC = (hPbuffer: HPBUFFERARB, hDC: HDC)(int32)*;
def PFNWGLDESTROYPBUFFERARBPROC = (hPbuffer: HPBUFFERARB)(BOOL)*;
def PFNWGLQUERYPBUFFERARBPROC = (hPbuffer: HPBUFFERARB, iAttribute: int32, piValue: int32*)(BOOL)*;

@[c]wglCreatePbufferARB(hDC: HDC, iPixelFormat: int32, iWidth: int32, iHeight: int32, piAttribList: int32*): HPBUFFERARB;
@[c]wglGetPbufferDCARB(hPbuffer: HPBUFFERARB): HDC;
@[c]wglReleasePbufferDCARB(hPbuffer: HPBUFFERARB, hDC: HDC): int32;
@[c]wglDestroyPbufferARB(hPbuffer: HPBUFFERARB): BOOL;
@[c]wglQueryPbufferARB(hPbuffer: HPBUFFERARB, iAttribute: int32, piValue: int32*): BOOL;

WGL_NUMBER_PIXEL_FORMATS_ARB: int32    = 0x2000;
WGL_DRAW_TO_WINDOW_ARB: int32          = 0x2001;
WGL_DRAW_TO_BITMAP_ARB: int32          = 0x2002;
WGL_ACCELERATION_ARB: int32            = 0x2003;
WGL_NEED_PALETTE_ARB: int32            = 0x2004;
WGL_NEED_SYSTEM_PALETTE_ARB: int32     = 0x2005;
WGL_SWAP_LAYER_BUFFERS_ARB: int32      = 0x2006;
WGL_SWAP_METHOD_ARB: int32             = 0x2007;
WGL_NUMBER_OVERLAYS_ARB: int32         = 0x2008;
WGL_NUMBER_UNDERLAYS_ARB: int32        = 0x2009;
WGL_TRANSPARENT_ARB: int32             = 0x200A;
WGL_TRANSPARENT_RED_VALUE_ARB: int32   = 0x2037;
WGL_TRANSPARENT_GREEN_VALUE_ARB: int32 = 0x2038;
WGL_TRANSPARENT_BLUE_VALUE_ARB: int32  = 0x2039;
WGL_TRANSPARENT_ALPHA_VALUE_ARB: int32 = 0x203A;
WGL_TRANSPARENT_INDEX_VALUE_ARB: int32 = 0x203B;
WGL_SHARE_DEPTH_ARB: int32             = 0x200C;
WGL_SHARE_STENCIL_ARB: int32           = 0x200D;
WGL_SHARE_ACCUM_ARB: int32             = 0x200E;
WGL_SUPPORT_GDI_ARB: int32             = 0x200F;
WGL_SUPPORT_OPENGL_ARB: int32          = 0x2010;
WGL_DOUBLE_BUFFER_ARB: int32           = 0x2011;
WGL_STEREO_ARB: int32                  = 0x2012;
WGL_PIXEL_TYPE_ARB: int32              = 0x2013;
WGL_COLOR_BITS_ARB: int32              = 0x2014;
WGL_RED_BITS_ARB: int32                = 0x2015;
WGL_RED_SHIFT_ARB: int32               = 0x2016;
WGL_GREEN_BITS_ARB: int32              = 0x2017;
WGL_GREEN_SHIFT_ARB: int32             = 0x2018;
WGL_BLUE_BITS_ARB: int32               = 0x2019;
WGL_BLUE_SHIFT_ARB: int32              = 0x201A;
WGL_ALPHA_BITS_ARB: int32              = 0x201B;
WGL_ALPHA_SHIFT_ARB: int32             = 0x201C;
WGL_ACCUM_BITS_ARB: int32              = 0x201D;
WGL_ACCUM_RED_BITS_ARB: int32          = 0x201E;
WGL_ACCUM_GREEN_BITS_ARB: int32        = 0x201F;
WGL_ACCUM_BLUE_BITS_ARB: int32         = 0x2020;
WGL_ACCUM_ALPHA_BITS_ARB: int32        = 0x2021;
WGL_DEPTH_BITS_ARB: int32              = 0x2022;
WGL_STENCIL_BITS_ARB: int32            = 0x2023;
WGL_AUX_BUFFERS_ARB: int32             = 0x2024;
WGL_NO_ACCELERATION_ARB: int32         = 0x2025;
WGL_GENERIC_ACCELERATION_ARB: int32    = 0x2026;
WGL_FULL_ACCELERATION_ARB: int32       = 0x2027;
WGL_SWAP_EXCHANGE_ARB: int32           = 0x2028;
WGL_SWAP_COPY_ARB: int32               = 0x2029;
WGL_SWAP_UNDEFINED_ARB: int32          = 0x202A;
WGL_TYPE_RGBA_ARB: int32               = 0x202B;
WGL_TYPE_COLORINDEX_ARB: int32         = 0x202C;

def PFNWGLGETPIXELFORMATATTRIBIVARBPROC = (hdc: HDC, iPixelFormat: int32, iLayerPlane: int32, nAttributes: UINT, piAttributes: int32*, piValues: int32*)(BOOL)*;
def PFNWGLGETPIXELFORMATATTRIBFVARBPROC = (hdc: HDC, iPixelFormat: int32, iLayerPlane: int32, nAttributes: UINT, piAttributes: int32*, pfValues: FLOAT*)(BOOL)*;
def PFNWGLCHOOSEPIXELFORMATARBPROC = (hdc: HDC, piAttribIList: int32*, pfAttribFList: FLOAT*, nMaxFormats: UINT, piFormats: int32*, nNumFormats: UINT*)(BOOL)*;

@[c]wglGetPixelFormatAttribivARB(hdc: HDC, iPixelFormat: int32, iLayerPlane: int32, nAttributes: UINT, piAttributes: int32*, piValues: int32*): BOOL;
@[c]wglGetPixelFormatAttribfvARB(hdc: HDC, iPixelFormat: int32, iLayerPlane: int32, nAttributes: UINT, piAttributes: int32*, pfValues: FLOAT*): BOOL;
@[c]wglChoosePixelFormatARB(hdc: HDC, piAttribIList: int32*, pfAttribFList: FLOAT*, nMaxFormats: UINT, piFormats: int32*, nNumFormats: UINT*): BOOL;

WGL_TYPE_RGBA_FLOAT_ARB: int32 = 0x21A0;

WGL_BIND_TO_TEXTURE_RGB_ARB: int32         = 0x2070;
WGL_BIND_TO_TEXTURE_RGBA_ARB: int32        = 0x2071;
WGL_TEXTURE_FORMAT_ARB: int32              = 0x2072;
WGL_TEXTURE_TARGET_ARB: int32              = 0x2073;
WGL_MIPMAP_TEXTURE_ARB: int32              = 0x2074;
WGL_TEXTURE_RGB_ARB: int32                 = 0x2075;
WGL_TEXTURE_RGBA_ARB: int32                = 0x2076;
WGL_NO_TEXTURE_ARB: int32                  = 0x2077;
WGL_TEXTURE_CUBE_MAP_ARB: int32            = 0x2078;
WGL_TEXTURE_1D_ARB: int32                  = 0x2079;
WGL_TEXTURE_2D_ARB: int32                  = 0x207A;
WGL_MIPMAP_LEVEL_ARB: int32                = 0x207B;
WGL_CUBE_MAP_FACE_ARB: int32               = 0x207C;
WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB: int32 = 0x207D;
WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB: int32 = 0x207E;
WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB: int32 = 0x207F;
WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB: int32 = 0x2080;
WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB: int32 = 0x2081;
WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB: int32 = 0x2082;
WGL_FRONT_LEFT_ARB: int32                  = 0x2083;
WGL_FRONT_RIGHT_ARB: int32                 = 0x2084;
WGL_BACK_LEFT_ARB: int32                   = 0x2085;
WGL_BACK_RIGHT_ARB: int32                  = 0x2086;
WGL_AUX0_ARB: int32                        = 0x2087;
WGL_AUX1_ARB: int32                        = 0x2088;
WGL_AUX2_ARB: int32                        = 0x2089;
WGL_AUX3_ARB: int32                        = 0x208A;
WGL_AUX4_ARB: int32                        = 0x208B;
WGL_AUX5_ARB: int32                        = 0x208C;
WGL_AUX6_ARB: int32                        = 0x208D;
WGL_AUX7_ARB: int32                        = 0x208E;
WGL_AUX8_ARB: int32                        = 0x208F;
WGL_AUX9_ARB: int32                        = 0x2090;

def PFNWGLBINDTEXIMAGEARBPROC = (hPbuffer: HPBUFFERARB, iBuffer: int32)(BOOL)*;
def PFNWGLRELEASETEXIMAGEARBPROC = (hPbuffer: HPBUFFERARB, iBuffer: int32)(BOOL)*;
def PFNWGLSETPBUFFERATTRIBARBPROC = (hPbuffer: HPBUFFERARB, piAttribList: int32*)(BOOL)*;

@[c]wglBindTexImageARB(hPbuffer: HPBUFFERARB, iBuffer: int32): BOOL;
@[c]wglReleaseTexImageARB(hPbuffer: HPBUFFERARB, iBuffer: int32): BOOL;
@[c]wglSetPbufferAttribARB(hPbuffer: HPBUFFERARB, piAttribList: int32*): BOOL;

WGL_CONTEXT_RESET_ISOLATION_BIT_ARB: int32 = 0x00000008;

WGL_SAMPLE_BUFFERS_3DFX: int32 = 0x2060;
WGL_SAMPLES_3DFX: int32        = 0x2061;

WGL_STEREO_EMITTER_ENABLE_3DL: int32  = 0x2055;
WGL_STEREO_EMITTER_DISABLE_3DL: int32 = 0x2056;
WGL_STEREO_POLARITY_NORMAL_3DL: int32 = 0x2057;
WGL_STEREO_POLARITY_INVERT_3DL: int32 = 0x2058;

def PFNWGLSETSTEREOEMITTERSTATE3DLPROC = (hDC: HDC, uState: UINT)(BOOL)*;

@[c]wglSetStereoEmitterState3DL(hDC: HDC, uState: UINT): BOOL;

WGL_GPU_VENDOR_AMD: int32                = 0x1F00;
WGL_GPU_RENDERER_STRING_AMD: int32       = 0x1F01;
WGL_GPU_OPENGL_VERSION_STRING_AMD: int32 = 0x1F02;
WGL_GPU_FASTEST_TARGET_GPUS_AMD: int32   = 0x21A2;
WGL_GPU_RAM_AMD: int32                   = 0x21A3;
WGL_GPU_CLOCK_AMD: int32                 = 0x21A4;
WGL_GPU_NUM_PIPES_AMD: int32             = 0x21A5;
WGL_GPU_NUM_SIMD_AMD: int32              = 0x21A6;
WGL_GPU_NUM_RB_AMD: int32                = 0x21A7;
WGL_GPU_NUM_SPI_AMD: int32               = 0x21A8;

def PFNWGLGETGPUIDSAMDPROC = (maxCount: UINT, ids: UINT*)(UINT)*;
def PFNWGLGETGPUINFOAMDPROC = (id: UINT, property: INT, dataType: GLenum, size: UINT, data: void*)(int32)*;
def PFNWGLGETCONTEXTGPUIDAMDPROC = (hglrc: HGLRC)(UINT)*;
def PFNWGLCREATEASSOCIATEDCONTEXTAMDPROC = (id: UINT)(HGLRC)*;
def PFNWGLCREATEASSOCIATEDCONTEXTATTRIBSAMDPROC = (id: UINT, hShareContext: HGLRC, attribList: int32*)(HGLRC)*;
def PFNWGLDELETEASSOCIATEDCONTEXTAMDPROC = (hglrc: HGLRC)(BOOL)*;
def PFNWGLMAKEASSOCIATEDCONTEXTCURRENTAMDPROC = (hglrc: HGLRC)(BOOL)*;
def PFNWGLGETCURRENTASSOCIATEDCONTEXTAMDPROC = ()(HGLRC)*;
def PFNWGLBLITCONTEXTFRAMEBUFFERAMDPROC = (dstCtx: HGLRC, srcX0: GLint, srcY0: GLint, srcX1: GLint, srcY1: GLint, dstX0: GLint, dstY0: GLint, dstX1: GLint, dstY1: GLint, mask: GLbitfield, filter: GLenum)(VOID)*;

@[c]wglGetGPUIDsAMD(maxCount: UINT, ids: UINT*): UINT;
@[c]wglGetGPUInfoAMD(id: UINT, property: INT, dataType: GLenum, size: UINT, data: void*): INT;
@[c]wglGetContextGPUIDAMD(hglrc: HGLRC): UINT;
@[c]wglCreateAssociatedContextAMD(id: UINT): HGLRC;
@[c]wglCreateAssociatedContextAttribsAMD(id: UINT, hShareContext: HGLRC, attribList: int32*): HGLRC;
@[c]wglDeleteAssociatedContextAMD(hglrc: HGLRC): BOOL;
@[c]wglMakeAssociatedContextCurrentAMD(hglrc: HGLRC): BOOL;
@[c]wglGetCurrentAssociatedContextAMD::HGLRC;
@[c]wglBlitContextFramebufferAMD(dstCtx: HGLRC, srcX0: GLint, srcY0: GLint, srcX1: GLint, srcY1: GLint, dstX0: GLint, dstY0: GLint, dstX1: GLint, dstY1: GLint, mask: GLbitfield, filter: GLenum): VOID;

WGL_TYPE_RGBA_FLOAT_ATI: int32 = 0x21A0;

WGL_TEXTURE_RECTANGLE_ATI: int32 = 0x21A5;

WGL_COLORSPACE_EXT: int32        = 0x309D;
WGL_COLORSPACE_SRGB_EXT: int32   = 0x3089;
WGL_COLORSPACE_LINEAR_EXT: int32 = 0x308A;

WGL_CONTEXT_ES2_PROFILE_BIT_EXT: int32 = 0x00000004;

WGL_CONTEXT_ES_PROFILE_BIT_EXT: int32 = 0x00000004;

WGL_DEPTH_FLOAT_EXT: int32 = 0x2040;

def PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC = (id: GLushort)(GLboolean)*;
def PFNWGLLOADDISPLAYCOLORTABLEEXTPROC = (table: GLushort*, length: GLuint)(GLboolean)*;
def PFNWGLBINDDISPLAYCOLORTABLEEXTPROC = (id: GLushort)(GLboolean)*;
def PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC = (id: GLushort)(VOID)*;

@[c]wglCreateDisplayColorTableEXT(id: GLushort): GLboolean;
@[c]wglLoadDisplayColorTableEXT(table: GLushort*, length: GLuint): GLboolean;
@[c]wglBindDisplayColorTableEXT(id: GLushort): GLboolean;
@[c]wglDestroyDisplayColorTableEXT(id: GLushort): VOID;

def PFNWGLGETEXTENSIONSSTRINGEXTPROC = ()(char*)*;

@[c]wglGetExtensionsStringEXT::char*;

WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT: int32 = 0x20A9;

ERROR_INVALID_PIXEL_TYPE_EXT: int32 = 0x2043;

def PFNWGLMAKECONTEXTCURRENTEXTPROC = (hDrawDC: HDC, hReadDC: HDC, hglrc: HGLRC)(BOOL)*;
def PFNWGLGETCURRENTREADDCEXTPROC = ()(HDC)*;

@[c]wglMakeContextCurrentEXT(hDrawDC: HDC, hReadDC: HDC, hglrc: HGLRC): BOOL;
@[c]wglGetCurrentReadDCEXT::HDC;

WGL_SAMPLE_BUFFERS_EXT: int32 = 0x2041;
WGL_SAMPLES_EXT: int32        = 0x2042;

def HPBUFFEREXT__ { unused: int32 }
def HPBUFFEREXT = HPBUFFEREXT__*;

WGL_DRAW_TO_PBUFFER_EXT: int32        = 0x202D;
WGL_MAX_PBUFFER_PIXELS_EXT: int32     = 0x202E;
WGL_MAX_PBUFFER_WIDTH_EXT: int32      = 0x202F;
WGL_MAX_PBUFFER_HEIGHT_EXT: int32     = 0x2030;
WGL_OPTIMAL_PBUFFER_WIDTH_EXT: int32  = 0x2031;
WGL_OPTIMAL_PBUFFER_HEIGHT_EXT: int32 = 0x2032;
WGL_PBUFFER_LARGEST_EXT: int32        = 0x2033;
WGL_PBUFFER_WIDTH_EXT: int32          = 0x2034;
WGL_PBUFFER_HEIGHT_EXT: int32         = 0x2035;

def PFNWGLCREATEPBUFFEREXTPROC = (hDC: HDC, iPixelFormat: int32, iWidth: int32, iHeight: int32, piAttribList: int32*)(HPBUFFEREXT)*;
def PFNWGLGETPBUFFERDCEXTPROC = (hPbuffer: HPBUFFEREXT)(HDC)*;
def PFNWGLRELEASEPBUFFERDCEXTPROC = (hPbuffer: HPBUFFEREXT, hDC: HDC)(int32)*;
def PFNWGLDESTROYPBUFFEREXTPROC = (hPbuffer: HPBUFFEREXT)(BOOL)*;
def PFNWGLQUERYPBUFFEREXTPROC = (hPbuffer: HPBUFFEREXT, iAttribute: int32, piValue: int32*)(BOOL)*;

@[c]wglCreatePbufferEXT(hDC: HDC, iPixelFormat: int32, iWidth: int32, iHeight: int32, piAttribList: int32*): HPBUFFEREXT;
@[c]wglGetPbufferDCEXT(hPbuffer: HPBUFFEREXT): HDC;
@[c]wglReleasePbufferDCEXT(hPbuffer: HPBUFFEREXT, hDC: HDC): int32;
@[c]wglDestroyPbufferEXT(hPbuffer: HPBUFFEREXT): BOOL;
@[c]wglQueryPbufferEXT(hPbuffer: HPBUFFEREXT, iAttribute: int32, piValue: int32*): BOOL;

WGL_NUMBER_PIXEL_FORMATS_EXT: int32 = 0x2000;
WGL_DRAW_TO_WINDOW_EXT: int32       = 0x2001;
WGL_DRAW_TO_BITMAP_EXT: int32       = 0x2002;
WGL_ACCELERATION_EXT: int32         = 0x2003;
WGL_NEED_PALETTE_EXT: int32         = 0x2004;
WGL_NEED_SYSTEM_PALETTE_EXT: int32  = 0x2005;
WGL_SWAP_LAYER_BUFFERS_EXT: int32   = 0x2006;
WGL_SWAP_METHOD_EXT: int32          = 0x2007;
WGL_NUMBER_OVERLAYS_EXT: int32      = 0x2008;
WGL_NUMBER_UNDERLAYS_EXT: int32     = 0x2009;
WGL_TRANSPARENT_EXT: int32          = 0x200A;
WGL_TRANSPARENT_VALUE_EXT: int32    = 0x200B;
WGL_SHARE_DEPTH_EXT: int32          = 0x200C;
WGL_SHARE_STENCIL_EXT: int32        = 0x200D;
WGL_SHARE_ACCUM_EXT: int32          = 0x200E;
WGL_SUPPORT_GDI_EXT: int32          = 0x200F;
WGL_SUPPORT_OPENGL_EXT: int32       = 0x2010;
WGL_DOUBLE_BUFFER_EXT: int32        = 0x2011;
WGL_STEREO_EXT: int32               = 0x2012;
WGL_PIXEL_TYPE_EXT: int32           = 0x2013;
WGL_COLOR_BITS_EXT: int32           = 0x2014;
WGL_RED_BITS_EXT: int32             = 0x2015;
WGL_RED_SHIFT_EXT: int32            = 0x2016;
WGL_GREEN_BITS_EXT: int32           = 0x2017;
WGL_GREEN_SHIFT_EXT: int32          = 0x2018;
WGL_BLUE_BITS_EXT: int32            = 0x2019;
WGL_BLUE_SHIFT_EXT: int32           = 0x201A;
WGL_ALPHA_BITS_EXT: int32           = 0x201B;
WGL_ALPHA_SHIFT_EXT: int32          = 0x201C;
WGL_ACCUM_BITS_EXT: int32           = 0x201D;
WGL_ACCUM_RED_BITS_EXT: int32       = 0x201E;
WGL_ACCUM_GREEN_BITS_EXT: int32     = 0x201F;
WGL_ACCUM_BLUE_BITS_EXT: int32      = 0x2020;
WGL_ACCUM_ALPHA_BITS_EXT: int32     = 0x2021;
WGL_DEPTH_BITS_EXT: int32           = 0x2022;
WGL_STENCIL_BITS_EXT: int32         = 0x2023;
WGL_AUX_BUFFERS_EXT: int32          = 0x2024;
WGL_NO_ACCELERATION_EXT: int32      = 0x2025;
WGL_GENERIC_ACCELERATION_EXT: int32 = 0x2026;
WGL_FULL_ACCELERATION_EXT: int32    = 0x2027;
WGL_SWAP_EXCHANGE_EXT: int32        = 0x2028;
WGL_SWAP_COPY_EXT: int32            = 0x2029;
WGL_SWAP_UNDEFINED_EXT: int32       = 0x202A;
WGL_TYPE_RGBA_EXT: int32            = 0x202B;
WGL_TYPE_COLORINDEX_EXT: int32      = 0x202C;

def PFNWGLGETPIXELFORMATATTRIBIVEXTPROC = (hdc: HDC, iPixelFormat: int32, iLayerPlane: int32, nAttributes: UINT, piAttributes: int32*, piValues: int32*)(BOOL)*;
def PFNWGLGETPIXELFORMATATTRIBFVEXTPROC = (hdc: HDC, iPixelFormat: int32, iLayerPlane: int32, nAttributes: UINT, piAttributes: int32*, pfValues: FLOAT*)(BOOL)*;
def PFNWGLCHOOSEPIXELFORMATEXTPROC = (hdc: HDC, piAttribIList: int32*, pfAttribFList: FLOAT*, nMaxFormats: UINT, piFormats: int32*, nNumFormats: UINT*)(BOOL)*;

@[c]wglGetPixelFormatAttribivEXT(hdc: HDC, iPixelFormat: int32, iLayerPlane: int32, nAttributes: UINT, piAttributes: int32*, piValues: int32*): BOOL;
@[c]wglGetPixelFormatAttribfvEXT(hdc: HDC, iPixelFormat: int32, iLayerPlane: int32, nAttributes: UINT, piAttributes: int32*, pfValues: FLOAT*): BOOL;
@[c]wglChoosePixelFormatEXT(hdc: HDC, piAttribIList: int32*, pfAttribFList: FLOAT*, nMaxFormats: UINT, piFormats: int32*, nNumFormats: UINT*): BOOL;

WGL_TYPE_RGBA_UNSIGNED_FLOAT_EXT: int32 = 0x20A8;

def PFNWGLSWAPINTERVALEXTPROC = (interval: int32)(BOOL)*;
def PFNWGLGETSWAPINTERVALEXTPROC = ()(int32)*;

@[c]wglSwapIntervalEXT(interval: int32): BOOL;
@[c]wglGetSwapIntervalEXT::int32;

WGL_DIGITAL_VIDEO_CURSOR_ALPHA_FRAMEBUFFER_I3D: int32 = 0x2050;
WGL_DIGITAL_VIDEO_CURSOR_ALPHA_VALUE_I3D: int32       = 0x2051;
WGL_DIGITAL_VIDEO_CURSOR_INCLUDED_I3D: int32          = 0x2052;
WGL_DIGITAL_VIDEO_GAMMA_CORRECTED_I3D: int32          = 0x2053;

def PFNWGLGETDIGITALVIDEOPARAMETERSI3DPROC = (hDC: HDC, iAttribute: int32, piValue: int32*)(BOOL)*;
def PFNWGLSETDIGITALVIDEOPARAMETERSI3DPROC = (hDC: HDC, iAttribute: int32, piValue: int32*)(BOOL)*;

@[c]wglGetDigitalVideoParametersI3D(hDC: HDC, iAttribute: int32, piValue: int32*): BOOL;
@[c]wglSetDigitalVideoParametersI3D(hDC: HDC, iAttribute: int32, piValue: int32*): BOOL;

WGL_GAMMA_TABLE_SIZE_I3D: int32      = 0x204E;
WGL_GAMMA_EXCLUDE_DESKTOP_I3D: int32 = 0x204F;

def PFNWGLGETGAMMATABLEPARAMETERSI3DPROC = (hDC: HDC, iAttribute: int32, piValue: int32*)(BOOL)*;
def PFNWGLSETGAMMATABLEPARAMETERSI3DPROC = (hDC: HDC, iAttribute: int32, piValue: int32*)(BOOL)*;
def PFNWGLGETGAMMATABLEI3DPROC = (hDC: HDC, iEntries: int32, puRed: USHORT*, puGreen: USHORT*, puBlue: USHORT*)(BOOL)*;
def PFNWGLSETGAMMATABLEI3DPROC = (hDC: HDC, iEntries: int32, puRed: USHORT*, puGreen: USHORT*, puBlue: USHORT*)(BOOL)*;

@[c]wglGetGammaTableParametersI3D(hDC: HDC, iAttribute: int32, piValue: int32*): BOOL;
@[c]wglSetGammaTableParametersI3D(hDC: HDC, iAttribute: int32, piValue: int32*): BOOL;
@[c]wglGetGammaTableI3D(hDC: HDC, iEntries: int32, puRed: USHORT*, puGreen: USHORT*, puBlue: USHORT*): BOOL;
@[c]wglSetGammaTableI3D(hDC: HDC, iEntries: int32, puRed: USHORT*, puGreen: USHORT*, puBlue: USHORT*): BOOL;

WGL_GENLOCK_SOURCE_MULTIVIEW_I3D: int32      = 0x2044;
WGL_GENLOCK_SOURCE_EXTERNAL_SYNC_I3D: int32  = 0x2045;
WGL_GENLOCK_SOURCE_EXTERNAL_FIELD_I3D: int32 = 0x2046;
WGL_GENLOCK_SOURCE_EXTERNAL_TTL_I3D: int32   = 0x2047;
WGL_GENLOCK_SOURCE_DIGITAL_SYNC_I3D: int32   = 0x2048;
WGL_GENLOCK_SOURCE_DIGITAL_FIELD_I3D: int32  = 0x2049;
WGL_GENLOCK_SOURCE_EDGE_FALLING_I3D: int32   = 0x204A;
WGL_GENLOCK_SOURCE_EDGE_RISING_I3D: int32    = 0x204B;
WGL_GENLOCK_SOURCE_EDGE_BOTH_I3D: int32      = 0x204C;

def PFNWGLENABLEGENLOCKI3DPROC = (hDC: HDC)(BOOL)*;
def PFNWGLDISABLEGENLOCKI3DPROC = (hDC: HDC)(BOOL)*;
def PFNWGLISENABLEDGENLOCKI3DPROC = (hDC: HDC, pFlag: BOOL*)(BOOL)*;
def PFNWGLGENLOCKSOURCEI3DPROC = (hDC: HDC, uSource: UINT)(BOOL)*;
def PFNWGLGETGENLOCKSOURCEI3DPROC = (hDC: HDC, uSource: UINT*)(BOOL)*;
def PFNWGLGENLOCKSOURCEEDGEI3DPROC = (hDC: HDC, uEdge: UINT)(BOOL)*;
def PFNWGLGETGENLOCKSOURCEEDGEI3DPROC = (hDC: HDC, uEdge: UINT*)(BOOL)*;
def PFNWGLGENLOCKSAMPLERATEI3DPROC = (hDC: HDC, uRate: UINT)(BOOL)*;
def PFNWGLGETGENLOCKSAMPLERATEI3DPROC = (hDC: HDC, uRate: UINT*)(BOOL)*;
def PFNWGLGENLOCKSOURCEDELAYI3DPROC = (hDC: HDC, uDelay: UINT)(BOOL)*;
def PFNWGLGETGENLOCKSOURCEDELAYI3DPROC = (hDC: HDC, uDelay: UINT*)(BOOL)*;
def PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC = (hDC: HDC, uMaxLineDelay: UINT*, uMaxPixelDelay: UINT*)(BOOL)*;

@[c]wglEnableGenlockI3D(hDC: HDC): BOOL;
@[c]wglDisableGenlockI3D(hDC: HDC): BOOL;
@[c]wglIsEnabledGenlockI3D(hDC: HDC, pFlag: BOOL*): BOOL;
@[c]wglGenlockSourceI3D(hDC: HDC, uSource: UINT): BOOL;
@[c]wglGetGenlockSourceI3D(hDC: HDC, uSource: UINT*): BOOL;
@[c]wglGenlockSourceEdgeI3D(hDC: HDC, uEdge: UINT): BOOL;
@[c]wglGetGenlockSourceEdgeI3D(hDC: HDC, uEdge: UINT*): BOOL;
@[c]wglGenlockSampleRateI3D(hDC: HDC, uRate: UINT): BOOL;
@[c]wglGetGenlockSampleRateI3D(hDC: HDC, uRate: UINT*): BOOL;
@[c]wglGenlockSourceDelayI3D(hDC: HDC, uDelay: UINT): BOOL;
@[c]wglGetGenlockSourceDelayI3D(hDC: HDC, uDelay: UINT*): BOOL;
@[c]wglQueryGenlockMaxSourceDelayI3D(hDC: HDC, uMaxLineDelay: UINT*, uMaxPixelDelay: UINT*): BOOL;

WGL_IMAGE_BUFFER_MIN_ACCESS_I3D: int32 = 0x00000001;
WGL_IMAGE_BUFFER_LOCK_I3D: int32       = 0x00000002;

def PFNWGLCREATEIMAGEBUFFERI3DPROC = (hDC: HDC, dwSize: DWORD, uFlags: UINT)(LPVOID)*;
def PFNWGLDESTROYIMAGEBUFFERI3DPROC = (hDC: HDC, pAddress: LPVOID)(BOOL)*;
def PFNWGLASSOCIATEIMAGEBUFFEREVENTSI3DPROC = (hDC: HDC, pEvent: HANDLE*, pAddress: LPVOID*, pSize: DWORD*, count: UINT)(BOOL)*;
def PFNWGLRELEASEIMAGEBUFFEREVENTSI3DPROC = (hDC: HDC, pAddress: LPVOID*, count: UINT)(BOOL)*;

@[c]wglCreateImageBufferI3D(hDC: HDC, dwSize: DWORD, uFlags: UINT): LPVOID;
@[c]wglDestroyImageBufferI3D(hDC: HDC, pAddress: LPVOID): BOOL;
@[c]wglAssociateImageBufferEventsI3D(hDC: HDC, pEvent: HANDLE*, pAddress: LPVOID*, pSize: DWORD*, count: UINT): BOOL;
@[c]wglReleaseImageBufferEventsI3D(hDC: HDC, pAddress: LPVOID*, count: UINT): BOOL;

def PFNWGLENABLEFRAMELOCKI3DPROC = ()(BOOL)*;
def PFNWGLDISABLEFRAMELOCKI3DPROC = ()(BOOL)*;
def PFNWGLISENABLEDFRAMELOCKI3DPROC = (pFlag: BOOL*)(BOOL)*;
def PFNWGLQUERYFRAMELOCKMASTERI3DPROC = (pFlag: BOOL*)(BOOL)*;

@[c]wglEnableFrameLockI3D::BOOL;
@[c]wglDisableFrameLockI3D::BOOL;
@[c]wglIsEnabledFrameLockI3D(pFlag: BOOL*): BOOL;
@[c]wglQueryFrameLockMasterI3D(pFlag: BOOL*): BOOL;

def PFNWGLGETFRAMEUSAGEI3DPROC = (pUsage: float*)(BOOL)*;
def PFNWGLBEGINFRAMETRACKINGI3DPROC = ()(BOOL)*;
def PFNWGLENDFRAMETRACKINGI3DPROC = ()(BOOL)*;
def PFNWGLQUERYFRAMETRACKINGI3DPROC = (pFrameCount: DWORD*, pMissedFrames: DWORD*, pLastMissedUsage: float*)(BOOL)*;

@[c]wglGetFrameUsageI3D(pUsage: float*): BOOL;
@[c]wglBeginFrameTrackingI3D::BOOL;
@[c]wglEndFrameTrackingI3D::BOOL;
@[c]wglQueryFrameTrackingI3D(pFrameCount: DWORD*, pMissedFrames: DWORD*, pLastMissedUsage: float*): BOOL;

WGL_ACCESS_READ_ONLY_NV: int32     = 0x00000000;
WGL_ACCESS_READ_WRITE_NV: int32    = 0x00000001;
WGL_ACCESS_WRITE_DISCARD_NV: int32 = 0x00000002;

def PFNWGLDXSETRESOURCESHAREHANDLENVPROC = (dxObject: void*, shareHandle: HANDLE)(BOOL)*;
def PFNWGLDXOPENDEVICENVPROC = (dxDevice: void*)(HANDLE)*;
def PFNWGLDXCLOSEDEVICENVPROC = (hDevice: HANDLE)(BOOL)*;
def PFNWGLDXREGISTEROBJECTNVPROC = (hDevice: HANDLE, dxObject: void*, name: GLuint, type: GLenum, access: GLenum)(HANDLE)*;
def PFNWGLDXUNREGISTEROBJECTNVPROC = (hDevice: HANDLE, hObject: HANDLE)(BOOL)*;
def PFNWGLDXOBJECTACCESSNVPROC = (hObject: HANDLE, access: GLenum)(BOOL)*;
def PFNWGLDXLOCKOBJECTSNVPROC = (hDevice: HANDLE, count: GLint, hObjects: HANDLE*)(BOOL)*;
def PFNWGLDXUNLOCKOBJECTSNVPROC = (hDevice: HANDLE, count: GLint, hObjects: HANDLE*)(BOOL)*;

@[c]wglDXSetResourceShareHandleNV(dxObject: void*, shareHandle: HANDLE): BOOL;
@[c]wglDXOpenDeviceNV(dxDevice: void*): HANDLE;
@[c]wglDXCloseDeviceNV(hDevice: HANDLE): BOOL;
@[c]wglDXRegisterObjectNV(hDevice: HANDLE, dxObject: void*, name: GLuint, type: GLenum, access: GLenum): HANDLE;
@[c]wglDXUnregisterObjectNV(hDevice: HANDLE, hObject: HANDLE): BOOL;
@[c]wglDXObjectAccessNV(hObject: HANDLE, access: GLenum): BOOL;
@[c]wglDXLockObjectsNV(hDevice: HANDLE, count: GLint, hObjects: HANDLE*): BOOL;
@[c]wglDXUnlockObjectsNV(hDevice: HANDLE, count: GLint, hObjects: HANDLE*): BOOL;

def PFNWGLCOPYIMAGESUBDATANVPROC = (hSrcRC: HGLRC, srcName: GLuint, srcTarget: GLenum, srcLevel: GLint, srcX: GLint, srcY: GLint, srcZ: GLint, hDstRC: HGLRC, dstName: GLuint, dstTarget: GLenum, dstLevel: GLint, dstX: GLint, dstY: GLint, dstZ: GLint, width: GLsizei, height: GLsizei, depth: GLsizei)(BOOL)*;

@[c]wglCopyImageSubDataNV(hSrcRC: HGLRC, srcName: GLuint, srcTarget: GLenum, srcLevel: GLint, srcX: GLint, srcY: GLint, srcZ: GLint, hDstRC: HGLRC, dstName: GLuint, dstTarget: GLenum, dstLevel: GLint, dstX: GLint, dstY: GLint, dstZ: GLint, width: GLsizei, height: GLsizei, depth: GLsizei): BOOL;

def PFNWGLDELAYBEFORESWAPNVPROC = (hDC: HDC, seconds: GLfloat)(BOOL)*;

@[c]wglDelayBeforeSwapNV(hDC: HDC, seconds: GLfloat): BOOL;

WGL_FLOAT_COMPONENTS_NV: int32                     = 0x20B0;
WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_R_NV: int32    = 0x20B1;
WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RG_NV: int32   = 0x20B2;
WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGB_NV: int32  = 0x20B3;
WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGBA_NV: int32 = 0x20B4;
WGL_TEXTURE_FLOAT_R_NV: int32                      = 0x20B5;
WGL_TEXTURE_FLOAT_RG_NV: int32                     = 0x20B6;
WGL_TEXTURE_FLOAT_RGB_NV: int32                    = 0x20B7;
WGL_TEXTURE_FLOAT_RGBA_NV: int32                   = 0x20B8;

def HGPUNV__ { unused: int32 }
def HGPUNV = HGPUNV__*;

def _GPU_DEVICE {
    cb: DWORD,
    DeviceName: CHAR[32],
    DeviceString: CHAR[128],
    Flags: DWORD,
    rcVirtualScreen: RECT,
}
def PGPU_DEVICE = _GPU_DEVICE*;

ERROR_INCOMPATIBLE_AFFINITY_MASKS_NV: int32 = 0x20D0;
ERROR_MISSING_AFFINITY_MASK_NV: int32       = 0x20D1;

def PFNWGLENUMGPUSNVPROC = (iGpuIndex: UINT, phGpu: HGPUNV*)(BOOL)*;
def PFNWGLENUMGPUDEVICESNVPROC = (hGpu: HGPUNV, iDeviceIndex: UINT, lpGpuDevice: PGPU_DEVICE)(BOOL)*;
def PFNWGLCREATEAFFINITYDCNVPROC = (phGpuList: HGPUNV*)(HDC)*;
def PFNWGLENUMGPUSFROMAFFINITYDCNVPROC = (hAffinityDC: HDC, iGpuIndex: UINT, hGpu: HGPUNV*)(BOOL)*;
def PFNWGLDELETEDCNVPROC = (hdc: HDC)(BOOL)*;

@[c]wglEnumGpusNV(iGpuIndex: UINT, phGpu: HGPUNV*): BOOL;
@[c]wglEnumGpuDevicesNV(hGpu: HGPUNV, iDeviceIndex: UINT, lpGpuDevice: PGPU_DEVICE): BOOL;
@[c]wglCreateAffinityDCNV(phGpuList: HGPUNV*): HDC;
@[c]wglEnumGpusFromAffinityDCNV(hAffinityDC: HDC, iGpuIndex: UINT, hGpu: HGPUNV*): BOOL;
@[c]wglDeleteDCNV(hdc: HDC): BOOL;

WGL_CONTEXT_MULTIGPU_ATTRIB_NV: int32                         = 0x20AA;
WGL_CONTEXT_MULTIGPU_ATTRIB_SINGLE_NV: int32                  = 0x20AB;
WGL_CONTEXT_MULTIGPU_ATTRIB_AFR_NV: int32                     = 0x20AC;
WGL_CONTEXT_MULTIGPU_ATTRIB_MULTICAST_NV: int32               = 0x20AD;
WGL_CONTEXT_MULTIGPU_ATTRIB_MULTI_DISPLAY_MULTICAST_NV: int32 = 0x20AE;

WGL_COVERAGE_SAMPLES_NV: int32 = 0x2042;
WGL_COLOR_SAMPLES_NV: int32    = 0x20B9;

def HVIDEOOUTPUTDEVICENV__ { unused: int32 }
def HVIDEOOUTPUTDEVICENV = HVIDEOOUTPUTDEVICENV__*;

WGL_NUM_VIDEO_SLOTS_NV: int32 = 0x20F0;

def PFNWGLENUMERATEVIDEODEVICESNVPROC = (hDc: HDC, phDeviceList: HVIDEOOUTPUTDEVICENV*)(int32)*;
def PFNWGLBINDVIDEODEVICENVPROC = (hDc: HDC, uVideoSlot: int32, hVideoDevice: HVIDEOOUTPUTDEVICENV, piAttribList: int32*)(BOOL)*;
def PFNWGLQUERYCURRENTCONTEXTNVPROC = (iAttribute: int32, piValue: int32*)(BOOL)*;

@[c]wglEnumerateVideoDevicesNV(hDc: HDC, phDeviceList: HVIDEOOUTPUTDEVICENV*): int32;
@[c]wglBindVideoDeviceNV(hDc: HDC, uVideoSlot: int32, hVideoDevice: HVIDEOOUTPUTDEVICENV, piAttribList: int32*): BOOL;
@[c]wglQueryCurrentContextNV(iAttribute: int32, piValue: int32*): BOOL;

WGL_BIND_TO_TEXTURE_DEPTH_NV: int32           = 0x20A3;
WGL_BIND_TO_TEXTURE_RECTANGLE_DEPTH_NV: int32 = 0x20A4;
WGL_DEPTH_TEXTURE_FORMAT_NV: int32            = 0x20A5;
WGL_TEXTURE_DEPTH_COMPONENT_NV: int32         = 0x20A6;
WGL_DEPTH_COMPONENT_NV: int32                 = 0x20A7;

WGL_BIND_TO_TEXTURE_RECTANGLE_RGB_NV: int32  = 0x20A0;
WGL_BIND_TO_TEXTURE_RECTANGLE_RGBA_NV: int32 = 0x20A1;
WGL_TEXTURE_RECTANGLE_NV: int32              = 0x20A2;

def PFNWGLJOINSWAPGROUPNVPROC = (hDC: HDC, group: GLuint)(BOOL)*;
def PFNWGLBINDSWAPBARRIERNVPROC = (group: GLuint, barrier: GLuint)(BOOL)*;
def PFNWGLQUERYSWAPGROUPNVPROC = (hDC: HDC, group: GLuint*, barrier: GLuint*)(BOOL)*;
def PFNWGLQUERYMAXSWAPGROUPSNVPROC = (hDC: HDC, maxGroups: GLuint*, maxBarriers: GLuint*)(BOOL)*;
def PFNWGLQUERYFRAMECOUNTNVPROC = (hDC: HDC, count: GLuint*)(BOOL)*;
def PFNWGLRESETFRAMECOUNTNVPROC = (hDC: HDC)(BOOL)*;

@[c]wglJoinSwapGroupNV(hDC: HDC, group: GLuint): BOOL;
@[c]wglBindSwapBarrierNV(group: GLuint, barrier: GLuint): BOOL;
@[c]wglQuerySwapGroupNV(hDC: HDC, group: GLuint*, barrier: GLuint*): BOOL;
@[c]wglQueryMaxSwapGroupsNV(hDC: HDC, maxGroups: GLuint*, maxBarriers: GLuint*): BOOL;
@[c]wglQueryFrameCountNV(hDC: HDC, count: GLuint*): BOOL;
@[c]wglResetFrameCountNV(hDC: HDC): BOOL;

def PFNWGLALLOCATEMEMORYNVPROC = (size: GLsizei, readfreq: GLfloat, writefreq: GLfloat, priority: GLfloat)(void*)*;
def PFNWGLFREEMEMORYNVPROC = (pointer: void*)(VOID)*;

@[c]wglAllocateMemoryNV(size: GLsizei, readfreq: GLfloat, writefreq: GLfloat, priority: GLfloat): void*;
@[c]wglFreeMemoryNV(pointer: void*): void;

def HVIDEOINPUTDEVICENV__ { unused: int32 }
def HVIDEOINPUTDEVICENV = HVIDEOINPUTDEVICENV__*;

WGL_UNIQUE_ID_NV: int32               = 0x20CE;
WGL_NUM_VIDEO_CAPTURE_SLOTS_NV: int32 = 0x20CF;

def PFNWGLBINDVIDEOCAPTUREDEVICENVPROC = (uVideoSlot: UINT, hDevice: HVIDEOINPUTDEVICENV)(BOOL)*;
def PFNWGLENUMERATEVIDEOCAPTUREDEVICESNVPROC = (hDc: HDC, phDeviceList: HVIDEOINPUTDEVICENV*)(UINT)*;
def PFNWGLLOCKVIDEOCAPTUREDEVICENVPROC = (hDc: HDC, hDevice: HVIDEOINPUTDEVICENV)(BOOL)*;
def PFNWGLQUERYVIDEOCAPTUREDEVICENVPROC = (hDc: HDC, hDevice: HVIDEOINPUTDEVICENV, iAttribute: int32, piValue: int32*)(BOOL)*;
def PFNWGLRELEASEVIDEOCAPTUREDEVICENVPROC = (hDc: HDC, hDevice: HVIDEOINPUTDEVICENV)(BOOL)*;

@[c]wglBindVideoCaptureDeviceNV(uVideoSlot: UINT, hDevice: HVIDEOINPUTDEVICENV): BOOL;
@[c]wglEnumerateVideoCaptureDevicesNV(hDc: HDC, phDeviceList: HVIDEOINPUTDEVICENV*): UINT;
@[c]wglLockVideoCaptureDeviceNV(hDc: HDC, hDevice: HVIDEOINPUTDEVICENV): BOOL;
@[c]wglQueryVideoCaptureDeviceNV(hDc: HDC, hDevice: HVIDEOINPUTDEVICENV, iAttribute: int32, piValue: int32*): BOOL;
@[c]wglReleaseVideoCaptureDeviceNV(hDc: HDC, hDevice: HVIDEOINPUTDEVICENV): BOOL;

def HPVIDEODEV__ { unused: int32 }
def HPVIDEODEV = HPVIDEODEV__*;

WGL_BIND_TO_VIDEO_RGB_NV: int32           = 0x20C0;
WGL_BIND_TO_VIDEO_RGBA_NV: int32          = 0x20C1;
WGL_BIND_TO_VIDEO_RGB_AND_DEPTH_NV: int32 = 0x20C2;
WGL_VIDEO_OUT_COLOR_NV: int32             = 0x20C3;
WGL_VIDEO_OUT_ALPHA_NV: int32             = 0x20C4;
WGL_VIDEO_OUT_DEPTH_NV: int32             = 0x20C5;
WGL_VIDEO_OUT_COLOR_AND_ALPHA_NV: int32   = 0x20C6;
WGL_VIDEO_OUT_COLOR_AND_DEPTH_NV: int32   = 0x20C7;
WGL_VIDEO_OUT_FRAME: int32                = 0x20C8;
WGL_VIDEO_OUT_FIELD_1: int32              = 0x20C9;
WGL_VIDEO_OUT_FIELD_2: int32              = 0x20CA;
WGL_VIDEO_OUT_STACKED_FIELDS_1_2: int32   = 0x20CB;
WGL_VIDEO_OUT_STACKED_FIELDS_2_1: int32   = 0x20CC;

def PFNWGLGETVIDEODEVICENVPROC = (hDC: HDC, numDevices: int32, hVideoDevice: HPVIDEODEV*)(BOOL)*;
def PFNWGLRELEASEVIDEODEVICENVPROC = (hVideoDevice: HPVIDEODEV)(BOOL)*;
def PFNWGLBINDVIDEOIMAGENVPROC = (hVideoDevice: HPVIDEODEV, hPbuffer: HPBUFFERARB, iVideoBuffer: int32)(BOOL)*;
def PFNWGLRELEASEVIDEOIMAGENVPROC = (hPbuffer: HPBUFFERARB, iVideoBuffer: int32)(BOOL)*;
def PFNWGLSENDPBUFFERTOVIDEONVPROC = (hPbuffer: HPBUFFERARB, iBufferType: int32, pulCounterPbuffer: long*, bBlock: BOOL)(BOOL)*;
def PFNWGLGETVIDEOINFONVPROC = (hpVideoDevice: HPVIDEODEV, pulCounterOutputPbuffer: long*, pulCounterOutputVideo: long*)(BOOL)*;

@[c]wglGetVideoDeviceNV(hDC: HDC, numDevices: int32, hVideoDevice: HPVIDEODEV*): BOOL;
@[c]wglReleaseVideoDeviceNV(hVideoDevice: HPVIDEODEV): BOOL;
@[c]wglBindVideoImageNV(hVideoDevice: HPVIDEODEV, hPbuffer: HPBUFFERARB, iVideoBuffer: int32): BOOL;
@[c]wglReleaseVideoImageNV(hPbuffer: HPBUFFERARB, iVideoBuffer: int32): BOOL;
@[c]wglSendPbufferToVideoNV(hPbuffer: HPBUFFERARB, iBufferType: int32, pulCounterPbuffer: long*, bBlock: BOOL): BOOL;
@[c]wglGetVideoInfoNV(hpVideoDevice: HPVIDEODEV, pulCounterOutputPbuffer: long*, pulCounterOutputVideo: long*): BOOL;

def PFNWGLGETSYNCVALUESOMLPROC = (hdc: HDC, ust: INT64*, msc: INT64*, sbc: INT64*)(BOOL)*;
def PFNWGLGETMSCRATEOMLPROC = (hdc: HDC, numerator: INT32*, denominator: INT32*)(BOOL)*;
def PFNWGLSWAPBUFFERSMSCOMLPROC = (hdc: HDC, target_msc: INT64, divisor: INT64, remainder: INT64)(INT64)*;
def PFNWGLSWAPLAYERBUFFERSMSCOMLPROC = (hdc: HDC, fuPlanes: INT, target_msc: INT64, divisor: INT64, remainder: INT64)(INT64)*;
def PFNWGLWAITFORMSCOMLPROC = (hdc: HDC, target_msc: INT64, divisor: INT64, remainder: INT64, ust: INT64*, msc: INT64*, sbc: INT64*)(BOOL)*;
def PFNWGLWAITFORSBCOMLPROC = (hdc: HDC, target_sbc: INT64, ust: INT64*, msc: INT64*, sbc: INT64*)(BOOL)*;

@[c]wglGetSyncValuesOML(hdc: HDC, ust: INT64*, msc: INT64*, sbc: INT64*): BOOL;
@[c]wglGetMscRateOML(hdc: HDC, numerator: INT32*, denominator: INT32*): BOOL;
@[c]wglSwapBuffersMscOML(hdc: HDC, target_msc: INT64, divisor: INT64, remainder: INT64): INT64;
@[c]wglSwapLayerBuffersMscOML(hdc: HDC, fuPlanes: INT, target_msc: INT64, divisor: INT64, remainder: INT64): INT64;
@[c]wglWaitForMscOML(hdc: HDC, target_msc: INT64, divisor: INT64, remainder: INT64, ust: INT64*, msc: INT64*, sbc: INT64*): BOOL;
@[c]wglWaitForSbcOML(hdc: HDC, target_sbc: INT64, ust: INT64*, msc: INT64*, sbc: INT64*): BOOL;
