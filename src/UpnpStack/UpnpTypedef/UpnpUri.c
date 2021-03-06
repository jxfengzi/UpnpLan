/*
* Copyright (C) 2013-2015
*
* @author jxfengzi@gmail.com
* @date   2013-7-9
*
* @file   UpnpUri.c
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#include "UpnpUri.h"
#include "tiny_memory.h"
#include "tiny_log.h"
#include "tiny_str_equal.h"
#include "tiny_str_split.h"

#define TAG             "UpnpUri"

#define ROOT_DEVICE     "upnp:rootdevice"
#define UUID            "uuid"
#define URN             "urn"
#define DEVICE          "device"
#define SERVICE         "service"

UpnpUri * UpnpUri_New(void)
{
    UpnpUri *thiz = NULL;

    do
    {
        TinyRet ret = TINY_RET_OK;

        thiz = (UpnpUri *)tiny_malloc(sizeof(UpnpUri));
        if (thiz == NULL)
        {
            break;
        }

        ret = UpnpUri_Construct(thiz);
        if (RET_FAILED(ret))
        {
            UpnpUri_Delete(thiz);
            thiz = NULL;
            break;
        }
    }
    while (0);

    return thiz;
}

TinyRet UpnpUri_Construct(UpnpUri *thiz)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    do
    {
        memset(thiz, 0, sizeof(UpnpUri));
        thiz->type = UPNP_URI_UNDEFINED;

        ret = TinyUuid_Construct(&thiz->uuid);
        if (RET_FAILED(ret))
        {
            break;
        }
    }
    while (0);

    return ret;
}

TinyRet UpnpUri_Dispose(UpnpUri *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    TinyUuid_Dispose(&thiz->uuid);

    return TINY_RET_OK;
}

void UpnpUri_Delete(UpnpUri *thiz)
{
    RETURN_IF_FAIL(thiz);

    UpnpUri_Dispose(thiz);
    tiny_free(thiz);
}

void UpnpUri_Copy(UpnpUri *dst, UpnpUri *src)
{
    RETURN_IF_FAIL(dst);
    RETURN_IF_FAIL(src);

    dst->type = src->type;
    strncpy(dst->string, src->string, UPNP_URI_LEN);
    TinyUuid_Copy(&dst->uuid, &src->uuid);
    strncpy(dst->uuid_string, src->uuid_string, UPNP_UUID_LEN);
    strncpy(dst->domain_name, src->domain_name, UPNP_DOMAIN_NAME_LEN);
    strncpy(dst->device_type, src->device_type, UPNP_TYPE_LEN);
    strncpy(dst->service_type, src->service_type, UPNP_TYPE_LEN);
    strncpy(dst->version, src->version, UPNP_VERSION_LEN);
}

TinyRet UpnpUri_Initialize_RootDevice(UpnpUri *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    thiz->type = UPNP_URI_ROOT_DEVICE;
    tiny_snprintf(thiz->string, UPNP_URI_LEN, ROOT_DEVICE);

    return TINY_RET_OK;
}

TinyRet UpnpUri_Initialize_Uuid(UpnpUri *thiz, const char *uuid)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);

    thiz->type = UPNP_URI_UUID;
    tiny_snprintf(thiz->string, UPNP_URI_LEN, "%s", uuid);

    return TINY_RET_OK;
}

TinyRet UpnpUri_Initialize_UpnpDevice(UpnpUri *thiz, const char *type, const char *version)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(type, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(version, TINY_RET_E_ARG_NULL);

    thiz->type = UPNP_URI_UPNP_DEVICE;
    tiny_snprintf(thiz->string, UPNP_URI_LEN, "%s:%s:%s:%s:%s", URN, SCHEMAS_UPNP_ORG, DEVICE, type, version);

    return TINY_RET_OK;
}

TinyRet UpnpUri_Initialize_UpnpService(UpnpUri *thiz, const char *type, const char *version)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(type, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(version, TINY_RET_E_ARG_NULL);

    thiz->type = UPNP_URI_UPNP_SERVICE;
    tiny_snprintf(thiz->string, UPNP_URI_LEN, "%s:%s:%s:%s:%s", URN, SCHEMAS_UPNP_ORG, SERVICE, type, version);

    return TINY_RET_OK;
}

TinyRet UpnpUri_Initialize_NonUpnpDevice(UpnpUri *thiz, const char *domain, const char *type, const char *version)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(domain, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(type, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(version, TINY_RET_E_ARG_NULL);

    thiz->type = UPNP_URI_NON_UPNP_DEVICE;
    tiny_snprintf(thiz->string, UPNP_URI_LEN, "%s:%s:%s:%s:%s", URN, domain, DEVICE, type, version);

    return TINY_RET_OK;
}

TinyRet UpnpUri_Initialize_NonUpnpService(UpnpUri *thiz, const char *domain, const char *type, const char *version)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(domain, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(type, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(version, TINY_RET_E_ARG_NULL);

    thiz->type = UPNP_URI_NON_UPNP_SERVICE;
    tiny_snprintf(thiz->string, UPNP_URI_LEN, "%s:%s:%s:%s:%s", URN, domain, SERVICE, type, version);

    return TINY_RET_OK;
}

TinyRet UpnpUri_Parse(UpnpUri *thiz, const char *string, bool strict_uuid)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_ARG_NULL);
    RETURN_VAL_IF_FAIL(string, TINY_RET_E_ARG_NULL);

    do
    {
        char group[10][128];
        uint32_t count = 0;

        if (STR_EQUAL(string, ROOT_DEVICE))
        {
            thiz->type = UPNP_URI_ROOT_DEVICE;
            break;
        }

        memset(group, 0, 10 * 128);
        count = str_split(string, ":", group, 10);
        if (count == 0)
        {
            ret = TINY_RET_E_ARG_INVALID;
            break;
        }

        if (STR_EQUAL(group[0], UUID))
        {
            strncpy(thiz->uuid_string, group[1], UPNP_UUID_LEN);

            if (strict_uuid)
            {
                ret = TinyUuid_ParseFromString(&thiz->uuid, group[1]);
            }
            break;
        }

        if (STR_EQUAL(group[0], URN))
        {
            if (count != 5)
            {
                ret = TINY_RET_E_ARG_INVALID;
                break;
            }

            if (STR_EQUAL(group[1], SCHEMAS_UPNP_ORG))
            {
                strncpy(thiz->domain_name, group[1], UPNP_DOMAIN_NAME_LEN);

                if (STR_EQUAL(group[2], DEVICE))
                {
                    thiz->type = UPNP_URI_UPNP_DEVICE;
                    strncpy(thiz->device_type, group[3], UPNP_TYPE_LEN);
                    strncpy(thiz->version, group[4], UPNP_VERSION_LEN);
                    break;
                }
                else if (STR_EQUAL(group[2], SERVICE))
                {
                    thiz->type = UPNP_URI_UPNP_SERVICE;
                    strncpy(thiz->service_type, group[3], UPNP_TYPE_LEN);
                    strncpy(thiz->version, group[4], UPNP_VERSION_LEN);
                    break;
                }
                else
                {
                    ret = TINY_RET_E_ARG_INVALID;
                    break;
                }
            }
            else
            {
                strncpy(thiz->domain_name, group[1], UPNP_DOMAIN_NAME_LEN);

                if (STR_EQUAL(group[2], DEVICE))
                {
                    thiz->type = UPNP_URI_NON_UPNP_DEVICE;
                    strncpy(thiz->device_type, group[3], UPNP_TYPE_LEN);
                    strncpy(thiz->version, group[4], UPNP_VERSION_LEN);
                    break;
                }
                else if (STR_EQUAL(group[2], SERVICE))
                {
                    thiz->type = UPNP_URI_NON_UPNP_SERVICE;
                    strncpy(thiz->service_type, group[3], UPNP_TYPE_LEN);
                    strncpy(thiz->version, group[4], UPNP_VERSION_LEN);
                    break;
                }
                else
                {
                    ret = TINY_RET_E_ARG_INVALID;
                    break;
                }
            }

            break;
        }

        ret = TINY_RET_E_ARG_INVALID;
    }
    while (0);

    if (RET_SUCCEEDED(ret))
    {
        strncpy(thiz->string, string, UPNP_URI_LEN);
    }

    return ret;
}

bool UpnpUri_IsEqual(UpnpUri *thiz, const char *string)
{
    return STR_EQUAL(thiz->string, string);
}