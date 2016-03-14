/*
* Copyright (C) 2013-2015
*
* @author jxfengzi@gmail.com
* @date   2013-11-19
*
* @file   UpnpRuntime.h
*
* @remark
*
*/

#ifndef __UPNP_RUNTIME_H__
#define __UPNP_RUNTIME_H__

#include "tiny_base.h"
#include "upnp_api.h"
#include "UpnpError.h"
#include "UpnpAction.h"
#include "UpnpDevice.h"
#include "UpnpDeviceSummary.h"
#include "UpnpSubscription.h"

TINY_BEGIN_DECLS


struct _UpnpRuntime;
typedef struct _UpnpRuntime UpnpRuntime;

UPNP_API UpnpRuntime * UpnpRuntime_New(void);
UPNP_API void UpnpRuntime_Delete(UpnpRuntime *thiz);

UPNP_API TinyRet UpnpRuntime_Start(UpnpRuntime *thiz);
UPNP_API TinyRet UpnpRuntime_Stop(UpnpRuntime *thiz);

/**
 * for UpnpControlPoint
 */
typedef void(*UpnpDeviceListener)(UpnpDeviceSummary *deviceSummary, bool alive, void *ctx);
typedef bool(*UpnpDeviceFilter)(const char *schemas, const char *deviceType, void *ctx);
UPNP_API TinyRet UpnpRuntime_StartScan(UpnpRuntime *thiz, UpnpDeviceListener listener, UpnpDeviceFilter filter, void *ctx);
UPNP_API TinyRet UpnpRuntime_StopScan(UpnpRuntime *thiz);
UPNP_API TinyRet UpnpRuntime_Invoke(UpnpRuntime *thiz, UpnpAction *action, UpnpError *error);
UPNP_API TinyRet UpnpRuntime_Subscribe(UpnpRuntime *thiz, UpnpSubscription *subscription, UpnpError *error);
UPNP_API TinyRet UpnpRuntime_Unsubscribe(UpnpRuntime *thiz, UpnpSubscription *subscription, UpnpError *error);

/**
* for UpnpDeviceHost
*/
typedef uint32_t (*UpnpActionHandler)(UpnpAction *action, void *ctx);
UPNP_API TinyRet UpnpRuntime_register(UpnpRuntime *thiz, UpnpDevice *device, UpnpActionHandler *handler, void *ctx);
UPNP_API TinyRet UpnpRuntime_unregister(UpnpRuntime *thiz, UpnpDevice *device);
UPNP_API TinyRet UpnpRuntime_sendEvents(UpnpRuntime *thiz, UpnpService *service);


TINY_END_DECLS

#endif /* __UPNP_RUNTIME_H__ */