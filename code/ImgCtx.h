#pragma once

#include <IImgCtx.h>

// for using ATL smart pointer class, CComPtr/CComQIPtr...
struct __declspec(uuid("3050f3d7-98b5-11cf-bb82-00aa00bdce0b")) IImgCtx;

// for using IImgCtxPtr
#include <comdef.h>
_COM_SMARTPTR_TYPEDEF(IImgCtx, IID_IImgCtx);
