

#include "config.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
//wrong definition?
#define ExtDeviceMode WINAPI ExtDeviceMode
#include "winspool.h"
#undef ExtDeviceMode
#include "winuser.h"
#include "wownt32.h"
#include "wine/unicode.h"
#include "wine/winbase16.h"

#include "wine/debug.h"
/* Maximum length of a device name */
#define CCHDEVICENAME16 32

/* Maximum length of a paper name */
#define CCHPAPERNAME16  64
#pragma pack(push, 1)
WINE_DEFAULT_DEBUG_CHANNEL(winspool);
typedef struct {
    char  dmDeviceName[CCHDEVICENAME16];
    UINT16  dmSpecVersion;
    UINT16  dmDriverVersion;
    UINT16  dmSize;
    UINT16  dmDriverExtra;
    DWORD dmFields;
    INT16   dmOrientation;
    INT16   dmPaperSize;
    INT16   dmPaperLength;
    INT16   dmPaperWidth;
    INT16   dmScale;
    INT16   dmCopies;
    INT16   dmDefaultSource;
    INT16   dmPrintQuality;
    INT16   dmColor;
    INT16   dmDuplex;
    INT16   dmYResolution;
    INT16   dmTTOption;
} DEVMODE16, *LPDEVMODE16;
#pragma pack(pop)
//sizeof(DEVMODE16) == sizeof(DEVMODEA)
void DEVMODE16To32(CONST DEVMODE16 *src, LPDEVMODEA dst, LONG extra)
{
    memcpy(dst->dmDeviceName, src->dmDeviceName, min(CCHDEVICENAME16, CCHDEVICENAME));
    dst->dmDeviceName[CCHDEVICENAME - 1] = 0;
    dst->dmSpecVersion = src->dmSpecVersion;
    dst->dmDriverVersion = src->dmDriverVersion;
    dst->dmSize = src->dmSize;
    dst->dmDriverExtra = src->dmDriverExtra;
    dst->dmFields = src->dmFields;
    dst->dmOrientation = src->dmOrientation;
    dst->dmPaperSize = src->dmPaperSize;
    dst->dmPaperLength = src->dmPaperLength;
    dst->dmPaperWidth = src->dmPaperWidth;
    dst->dmScale = src->dmScale;
    dst->dmCopies = src->dmCopies;
    dst->dmDefaultSource = src->dmDefaultSource;
    dst->dmPrintQuality = src->dmPrintQuality;
    dst->dmColor = src->dmColor;
    dst->dmDuplex = src->dmDuplex;
    dst->dmYResolution = src->dmYResolution;
    dst->dmTTOption = src->dmTTOption;
    memcpy((char*)dst + sizeof(DEVMODEA), (char*)src + sizeof(DEVMODE16), extra - sizeof(DEVMODEA));
}
void DEVMODE32To16(LPDEVMODE16 dst, const LPDEVMODEA src, LONG extra)
{
    memcpy(dst->dmDeviceName, src->dmDeviceName, min(CCHDEVICENAME16, CCHDEVICENAME));
    dst->dmDeviceName[CCHDEVICENAME16 - 1] = 0;
    dst->dmSpecVersion = src->dmSpecVersion;
    dst->dmDriverVersion = src->dmDriverVersion;
    dst->dmSize = src->dmSize;
    dst->dmDriverExtra = src->dmDriverExtra;
    dst->dmFields = src->dmFields;
    dst->dmOrientation = src->dmOrientation;
    dst->dmPaperSize = src->dmPaperSize;
    dst->dmPaperLength = src->dmPaperLength;
    dst->dmPaperWidth = src->dmPaperWidth;
    dst->dmScale = src->dmScale;
    dst->dmCopies = src->dmCopies;
    dst->dmDefaultSource = src->dmDefaultSource;
    dst->dmPrintQuality = src->dmPrintQuality;
    dst->dmColor = src->dmColor;
    dst->dmDuplex = src->dmDuplex;
    dst->dmYResolution = src->dmYResolution;
    dst->dmTTOption = src->dmTTOption;
    memcpy((char*)dst + sizeof(DEVMODE16), (char*)src + sizeof(DEVMODEA), extra - sizeof(DEVMODEA));
}
int WINAPI ExtDeviceMode16(HWND16 hwnd16, HANDLE16 hDriver16, LPDEVMODE16 pDevModeOutput, LPSTR pDeviceName, LPSTR pPort, LPDEVMODE16 pDevModeInput, LPSTR pProfile, WORD fMode)
{
    LONG size = ExtDeviceMode(HWND_32(hwnd16), (HANDLE)hDriver16, NULL, pDeviceName, pPort, NULL, pProfile, 0);
    char *devmode32 = alloca(size);
    char *input32 = alloca(size);
    if (pDevModeInput)
        DEVMODE16To32(pDevModeInput, (LPDEVMODEA)&input32[0], size);
    LONG result = ExtDeviceMode(HWND_32(hwnd16), (HANDLE)hDriver16, pDevModeOutput ? (LPDEVMODEA)&devmode32[0] : NULL, pDeviceName, pPort, pDevModeInput ? (LPDEVMODEA)&input32[0] : NULL, pProfile, fMode);
    if (fMode == 0)
    {
        size = result;
    }
    if (pDevModeOutput)
        DEVMODE32To16(pDevModeOutput, (LPDEVMODEA)&devmode32[0], size);
    return result;
}
void WINAPI DeviceMode16(HWND16 hwnd, HMODULE16 hModule16, LPSTR lpszDevice, LPSTR lpszOutput)
{
    return;
}
DWORD WINAPI DeviceCapabilities16(LPCSTR pDevice, LPCSTR pPort, WORD fwCapability, LPSTR pOutput, CONST DEVMODE16 *pDevMode)
{
    char devmode32[65536] = { 0 };
    if (pDevMode)
        DEVMODE16To32(pDevMode, (LPDEVMODEA)&devmode32[0], 0);
    DWORD result = DeviceCapabilitiesA(pDevice, pPort, fwCapability, pOutput, pDevMode ? (LPDEVMODEA)&devmode32[0] : NULL);
    return result;
}
