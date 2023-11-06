/*
 *  Copyright (c) 2016, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

// Disable OpenThread's own new implementation to avoid duplicate definition
#define OT_CORE_COMMON_NEW_HPP_
#include "test_platform.h"

#include <map>
#include <vector>

#include <stdio.h>
#include <sys/time.h>

enum
{
    FLASH_SWAP_SIZE = 2048,
    FLASH_SWAP_NUM  = 2,
};

std::map<uint32_t, std::vector<std::vector<uint8_t>>> settings;

ot::Instance *testInitInstance(void)
{
    otInstance *instance = nullptr;

#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
    size_t   instanceBufferLength = 0;
    uint8_t *instanceBuffer       = nullptr;

    // Call to query the buffer size
    (void)otInstanceInit(nullptr, &instanceBufferLength);

    // Call to allocate the buffer
    instanceBuffer = (uint8_t *)malloc(instanceBufferLength);
    VerifyOrQuit(instanceBuffer != nullptr, "Failed to allocate otInstance");
    memset(instanceBuffer, 0, instanceBufferLength);

    // Initialize OpenThread with the buffer
    instance = otInstanceInit(instanceBuffer, &instanceBufferLength);
#else
    instance = otInstanceInitSingle();
#endif

    return static_cast<ot::Instance *>(instance);
}

void testFreeInstance(otInstance *aInstance)
{
    otInstanceFinalize(aInstance);

#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
    free(aInstance);
#endif
}

bool sDiagMode = false;

extern "C" {

#if OPENTHREAD_CONFIG_HEAP_EXTERNAL_ENABLE
OT_TOOL_WEAK void *otPlatCAlloc(size_t aNum, size_t aSize) { return calloc(aNum, aSize); }

OT_TOOL_WEAK void otPlatFree(void *aPtr) { free(aPtr); }
#endif

OT_TOOL_WEAK void otTaskletsSignalPending(otInstance *) {}

OT_TOOL_WEAK void otPlatAlarmMilliStop(otInstance *) {}

OT_TOOL_WEAK void otPlatAlarmMilliStartAt(otInstance *, uint32_t, uint32_t) {}

OT_TOOL_WEAK uint32_t otPlatAlarmMilliGetNow(void)
{
    struct timeval tv;

    gettimeofday(&tv, nullptr);

    return (uint32_t)((tv.tv_sec * 1000) + (tv.tv_usec / 1000) + 123456);
}

OT_TOOL_WEAK void otPlatAlarmMicroStop(otInstance *) {}

OT_TOOL_WEAK void otPlatAlarmMicroStartAt(otInstance *, uint32_t, uint32_t) {}

OT_TOOL_WEAK uint32_t otPlatAlarmMicroGetNow(void)
{
    struct timeval tv;

    gettimeofday(&tv, nullptr);

    return (uint32_t)((tv.tv_sec * 1000000) + tv.tv_usec + 123456);
}

OT_TOOL_WEAK void otPlatRadioGetIeeeEui64(otInstance *, uint8_t *) {}

OT_TOOL_WEAK void otPlatRadioSetPanId(otInstance *, uint16_t) {}

OT_TOOL_WEAK void otPlatRadioSetExtendedAddress(otInstance *, const otExtAddress *) {}

OT_TOOL_WEAK void otPlatRadioSetShortAddress(otInstance *, uint16_t) {}

OT_TOOL_WEAK void otPlatRadioSetPromiscuous(otInstance *, bool) {}

OT_TOOL_WEAK bool otPlatRadioIsEnabled(otInstance *) { return true; }

OT_TOOL_WEAK otError otPlatRadioEnable(otInstance *) { return OT_ERROR_NONE; }

OT_TOOL_WEAK otError otPlatRadioDisable(otInstance *) { return OT_ERROR_NONE; }

OT_TOOL_WEAK otError otPlatRadioSleep(otInstance *) { return OT_ERROR_NONE; }

OT_TOOL_WEAK otError otPlatRadioReceive(otInstance *, uint8_t) { return OT_ERROR_NONE; }

OT_TOOL_WEAK otError otPlatRadioTransmit(otInstance *, otRadioFrame *) { return OT_ERROR_NONE; }

OT_TOOL_WEAK otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *) { return nullptr; }

OT_TOOL_WEAK int8_t otPlatRadioGetRssi(otInstance *) { return 0; }

OT_TOOL_WEAK otRadioCaps otPlatRadioGetCaps(otInstance *) { return OT_RADIO_CAPS_NONE; }

OT_TOOL_WEAK bool otPlatRadioGetPromiscuous(otInstance *) { return false; }

OT_TOOL_WEAK void otPlatRadioEnableSrcMatch(otInstance *, bool) {}

OT_TOOL_WEAK otError otPlatRadioAddSrcMatchShortEntry(otInstance *, uint16_t) { return OT_ERROR_NONE; }

OT_TOOL_WEAK otError otPlatRadioAddSrcMatchExtEntry(otInstance *, const otExtAddress *) { return OT_ERROR_NONE; }

OT_TOOL_WEAK otError otPlatRadioClearSrcMatchShortEntry(otInstance *, uint16_t) { return OT_ERROR_NONE; }

OT_TOOL_WEAK otError otPlatRadioClearSrcMatchExtEntry(otInstance *, const otExtAddress *) { return OT_ERROR_NONE; }

OT_TOOL_WEAK void otPlatRadioClearSrcMatchShortEntries(otInstance *) {}

OT_TOOL_WEAK void otPlatRadioClearSrcMatchExtEntries(otInstance *) {}

OT_TOOL_WEAK otError otPlatRadioEnergyScan(otInstance *, uint8_t, uint16_t) { return OT_ERROR_NOT_IMPLEMENTED; }

OT_TOOL_WEAK otError otPlatRadioSetTransmitPower(otInstance *, int8_t) { return OT_ERROR_NOT_IMPLEMENTED; }

OT_TOOL_WEAK int8_t otPlatRadioGetReceiveSensitivity(otInstance *) { return -100; }

OT_TOOL_WEAK otError otPlatEntropyGet(uint8_t *aOutput, uint16_t aOutputLength)
{
    otError error = OT_ERROR_NONE;

    VerifyOrExit(aOutput, error = OT_ERROR_INVALID_ARGS);

#if __SANITIZE_ADDRESS__ == 0
    {
        FILE  *file = nullptr;
        size_t readLength;

        file = fopen("/dev/urandom", "rb");
        VerifyOrExit(file != nullptr, error = OT_ERROR_FAILED);

        readLength = fread(aOutput, 1, aOutputLength, file);

        if (readLength != aOutputLength)
        {
            error = OT_ERROR_FAILED;
        }

        fclose(file);
    }
#else
    for (uint16_t length = 0; length < aOutputLength; length++)
    {
        aOutput[length] = (uint8_t)rand();
    }
#endif

exit:
    return error;
}

OT_TOOL_WEAK void otPlatDiagProcess(otInstance *, uint8_t, char *aArgs[], char *aOutput, size_t aOutputMaxLen)
{
    snprintf(aOutput, aOutputMaxLen, "diag feature '%s' is not supported\r\n", aArgs[0]);
}

OT_TOOL_WEAK void otPlatDiagModeSet(bool aMode) { sDiagMode = aMode; }

OT_TOOL_WEAK bool otPlatDiagModeGet() { return sDiagMode; }

OT_TOOL_WEAK void otPlatDiagChannelSet(uint8_t) {}

OT_TOOL_WEAK void otPlatDiagTxPowerSet(int8_t) {}

OT_TOOL_WEAK void otPlatDiagRadioReceived(otInstance *, otRadioFrame *, otError) {}

OT_TOOL_WEAK void otPlatDiagAlarmCallback(otInstance *) {}

OT_TOOL_WEAK void otPlatUartSendDone(void) {}

OT_TOOL_WEAK void otPlatUartReceived(const uint8_t *, uint16_t) {}

OT_TOOL_WEAK void otPlatReset(otInstance *) {}

OT_TOOL_WEAK otError otPlatResetToBootloader(otInstance *) { return OT_ERROR_NOT_CAPABLE; }

OT_TOOL_WEAK otPlatResetReason otPlatGetResetReason(otInstance *) { return OT_PLAT_RESET_REASON_POWER_ON; }

OT_TOOL_WEAK void otPlatLog(otLogLevel, otLogRegion, const char *, ...) {}

OT_TOOL_WEAK void otPlatSettingsInit(otInstance *, const uint16_t *, uint16_t) {}

OT_TOOL_WEAK void otPlatSettingsDeinit(otInstance *) {}

OT_TOOL_WEAK otError otPlatSettingsGet(otInstance *, uint16_t aKey, int aIndex, uint8_t *aValue, uint16_t *aValueLength)
{
    auto setting = settings.find(aKey);

    if (setting == settings.end())
    {
        return OT_ERROR_NOT_FOUND;
    }

    if (aIndex > setting->second.size())
    {
        return OT_ERROR_NOT_FOUND;
    }

    if (aValueLength == nullptr)
    {
        return OT_ERROR_NONE;
    }

    const auto &data = setting->second[aIndex];

    if (aValue == nullptr)
    {
        *aValueLength = data.size();
        return OT_ERROR_NONE;
    }

    if (*aValueLength >= data.size())
    {
        *aValueLength = data.size();
    }

    memcpy(aValue, &data[0], *aValueLength);

    return OT_ERROR_NONE;
}

OT_TOOL_WEAK otError otPlatSettingsSet(otInstance *, uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength)
{
    auto setting = std::vector<uint8_t>(aValue, aValue + aValueLength);

    settings[aKey].clear();
    settings[aKey].push_back(setting);

    return OT_ERROR_NONE;
}

OT_TOOL_WEAK otError otPlatSettingsAdd(otInstance *, uint16_t aKey, const uint8_t *aValue, uint16_t aValueLength)
{
    auto setting = std::vector<uint8_t>(aValue, aValue + aValueLength);
    settings[aKey].push_back(setting);

    return OT_ERROR_NONE;
}

OT_TOOL_WEAK otError otPlatSettingsDelete(otInstance *, uint16_t aKey, int aIndex)
{
    auto setting = settings.find(aKey);
    if (setting == settings.end())
    {
        return OT_ERROR_NOT_FOUND;
    }

    if (aIndex >= setting->second.size())
    {
        return OT_ERROR_NOT_FOUND;
    }
    setting->second.erase(setting->second.begin() + aIndex);
    return OT_ERROR_NONE;
}

OT_TOOL_WEAK void otPlatSettingsWipe(otInstance *) { settings.clear(); }

uint8_t *GetFlash(void)
{
    static uint8_t sFlash[FLASH_SWAP_SIZE * FLASH_SWAP_NUM];
    static bool    sInitialized;

    if (!sInitialized)
    {
        memset(sFlash, 0xff, sizeof(sFlash));
        sInitialized = true;
    }

    return sFlash;
}

OT_TOOL_WEAK void otPlatFlashInit(otInstance *) {}

OT_TOOL_WEAK uint32_t otPlatFlashGetSwapSize(otInstance *) { return FLASH_SWAP_SIZE; }

OT_TOOL_WEAK void otPlatFlashErase(otInstance *, uint8_t aSwapIndex)
{
    uint32_t address;

    VerifyOrQuit(aSwapIndex < FLASH_SWAP_NUM, "aSwapIndex invalid");

    address = aSwapIndex ? FLASH_SWAP_SIZE : 0;

    memset(GetFlash() + address, 0xff, FLASH_SWAP_SIZE);
}

OT_TOOL_WEAK void otPlatFlashRead(otInstance *, uint8_t aSwapIndex, uint32_t aOffset, void *aData, uint32_t aSize)
{
    uint32_t address;

    VerifyOrQuit(aSwapIndex < FLASH_SWAP_NUM, "aSwapIndex invalid");
    VerifyOrQuit(aSize <= FLASH_SWAP_SIZE, "aSize invalid");
    VerifyOrQuit(aOffset <= (FLASH_SWAP_SIZE - aSize), "aOffset + aSize invalid");

    address = aSwapIndex ? FLASH_SWAP_SIZE : 0;

    memcpy(aData, GetFlash() + address + aOffset, aSize);
}

OT_TOOL_WEAK void otPlatFlashWrite(otInstance *,
                                   uint8_t     aSwapIndex,
                                   uint32_t    aOffset,
                                   const void *aData,
                                   uint32_t    aSize)
{
    uint32_t address;

    VerifyOrQuit(aSwapIndex < FLASH_SWAP_NUM, "aSwapIndex invalid");
    VerifyOrQuit(aSize <= FLASH_SWAP_SIZE, "aSize invalid");
    VerifyOrQuit(aOffset <= (FLASH_SWAP_SIZE - aSize), "aOffset + aSize invalid");

    address = aSwapIndex ? FLASH_SWAP_SIZE : 0;

    for (uint32_t index = 0; index < aSize; index++)
    {
        GetFlash()[address + aOffset + index] &= ((uint8_t *)aData)[index];
    }
}

#if OPENTHREAD_CONFIG_TIME_SYNC_ENABLE || OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE
OT_TOOL_WEAK uint16_t otPlatTimeGetXtalAccuracy(void) { return 0; }
#endif

#if OPENTHREAD_CONFIG_MAC_CSL_RECEIVER_ENABLE
OT_TOOL_WEAK otError otPlatRadioEnableCsl(otInstance *, uint32_t, otShortAddress, const otExtAddress *)
{
    return OT_ERROR_NONE;
}

OT_TOOL_WEAK void otPlatRadioUpdateCslSampleTime(otInstance *, uint32_t) {}

OT_TOOL_WEAK uint8_t otPlatRadioGetCslAccuracy(otInstance *)
{
    return static_cast<uint8_t>(otPlatTimeGetXtalAccuracy() / 2);
}
#endif

#if OPENTHREAD_CONFIG_OTNS_ENABLE
OT_TOOL_WEAK void otPlatOtnsStatus(const char *) {}
#endif

#if OPENTHREAD_CONFIG_RADIO_LINK_TREL_ENABLE
OT_TOOL_WEAK void otPlatTrelEnable(otInstance *, uint16_t *) {}

OT_TOOL_WEAK void otPlatTrelDisable(otInstance *) {}

OT_TOOL_WEAK void otPlatTrelSend(otInstance *, const uint8_t *, uint16_t, const otSockAddr *) {}

OT_TOOL_WEAK void otPlatTrelRegisterService(otInstance *, uint16_t, const uint8_t *, uint8_t) {}
#endif

#if OPENTHREAD_CONFIG_MLE_LINK_METRICS_SUBJECT_ENABLE
OT_TOOL_WEAK otError otPlatRadioConfigureEnhAckProbing(otInstance *,
                                                       otLinkMetrics,
                                                       const otShortAddress,
                                                       const otExtAddress *)
{
    return OT_ERROR_NONE;
}

OT_TOOL_WEAK otLinkMetrics otPlatRadioGetEnhAckProbingMetrics(otInstance *, const otShortAddress)
{
    otLinkMetrics metrics;

    memset(&metrics, 0, sizeof(metrics));

    return metrics;
}
#endif

#if OPENTHREAD_CONFIG_BORDER_ROUTING_ENABLE
OT_TOOL_WEAK bool otPlatInfraIfHasAddress(uint32_t, const otIp6Address *) { return false; }

OT_TOOL_WEAK otError otPlatInfraIfSendIcmp6Nd(uint32_t, const otIp6Address *, const uint8_t *, uint16_t)
{
    return OT_ERROR_FAILED;
}

OT_TOOL_WEAK otError otPlatInfraIfDiscoverNat64Prefix(uint32_t) { return OT_ERROR_FAILED; }
#endif

#if OPENTHREAD_CONFIG_PLATFORM_KEY_REFERENCES_ENABLE

otError otPlatCryptoImportKey(otCryptoKeyRef      *aKeyRef,
                              otCryptoKeyType      aKeyType,
                              otCryptoKeyAlgorithm aKeyAlgorithm,
                              int                  aKeyUsage,
                              otCryptoKeyStorage   aKeyPersistence,
                              const uint8_t       *aKey,
                              size_t               aKeyLen)
{
    OT_UNUSED_VARIABLE(aKeyRef);
    OT_UNUSED_VARIABLE(aKeyType);
    OT_UNUSED_VARIABLE(aKeyAlgorithm);
    OT_UNUSED_VARIABLE(aKeyUsage);
    OT_UNUSED_VARIABLE(aKeyPersistence);
    OT_UNUSED_VARIABLE(aKey);
    OT_UNUSED_VARIABLE(aKeyLen);

    return OT_ERROR_NONE;
}

otError otPlatCryptoExportKey(otCryptoKeyRef aKeyRef, uint8_t *aBuffer, size_t aBufferLen, size_t *aKeyLen)
{
    OT_UNUSED_VARIABLE(aKeyRef);
    OT_UNUSED_VARIABLE(aBuffer);
    OT_UNUSED_VARIABLE(aBufferLen);

    *aKeyLen = 0;

    return OT_ERROR_NONE;
}

otError otPlatCryptoDestroyKey(otCryptoKeyRef aKeyRef)
{
    OT_UNUSED_VARIABLE(aKeyRef);

    return OT_ERROR_NONE;
}

bool otPlatCryptoHasKey(otCryptoKeyRef aKeyRef)
{
    OT_UNUSED_VARIABLE(aKeyRef);

    return false;
}

#endif // OPENTHREAD_CONFIG_PLATFORM_KEY_REFERENCES_ENABLE

otError otPlatCryptoEcdsaGenerateAndImportKey(otCryptoKeyRef aKeyRef)
{
    OT_UNUSED_VARIABLE(aKeyRef);

    return OT_ERROR_NONE;
}

otError otPlatCryptoEcdsaExportPublicKey(otCryptoKeyRef aKeyRef, otPlatCryptoEcdsaPublicKey *aPublicKey)
{
    OT_UNUSED_VARIABLE(aKeyRef);
    OT_UNUSED_VARIABLE(aPublicKey);

    return OT_ERROR_NONE;
}

otError otPlatCryptoEcdsaSignUsingKeyRef(otCryptoKeyRef                aKeyRef,
                                         const otPlatCryptoSha256Hash *aHash,
                                         otPlatCryptoEcdsaSignature   *aSignature)
{
    OT_UNUSED_VARIABLE(aKeyRef);
    OT_UNUSED_VARIABLE(aHash);
    OT_UNUSED_VARIABLE(aSignature);

    return OT_ERROR_NONE;
}

otError otPlatCryptoEcdsaVerifyUsingKeyRef(otCryptoKeyRef                    aKeyRef,
                                           const otPlatCryptoSha256Hash     *aHash,
                                           const otPlatCryptoEcdsaSignature *aSignature)
{
    OT_UNUSED_VARIABLE(aKeyRef);
    OT_UNUSED_VARIABLE(aHash);
    OT_UNUSED_VARIABLE(aSignature);

    return OT_ERROR_NONE;
}

otError otPlatRadioSetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t aThreshold)
{
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aThreshold);

    return OT_ERROR_NONE;
}

#if OPENTHREAD_CONFIG_DNS_DSO_ENABLE

OT_TOOL_WEAK void otPlatDsoEnableListening(otInstance *aInstance, bool aEnable)
{
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aEnable);
}

OT_TOOL_WEAK void otPlatDsoConnect(otPlatDsoConnection *aConnection, const otSockAddr *aPeerSockAddr)
{
    OT_UNUSED_VARIABLE(aConnection);
    OT_UNUSED_VARIABLE(aPeerSockAddr);
}

OT_TOOL_WEAK void otPlatDsoSend(otPlatDsoConnection *aConnection, otMessage *aMessage)
{
    OT_UNUSED_VARIABLE(aConnection);
    OT_UNUSED_VARIABLE(aMessage);
}

OT_TOOL_WEAK void otPlatDsoDisconnect(otPlatDsoConnection *aConnection, otPlatDsoDisconnectMode aMode)
{
    OT_UNUSED_VARIABLE(aConnection);
    OT_UNUSED_VARIABLE(aMode);
}

#endif // #if OPENTHREAD_CONFIG_DNS_DSO_ENABLE

#if OPENTHREAD_CONFIG_PLATFORM_UDP_ENABLE
otError otPlatUdpSocket(otUdpSocket *aUdpSocket)
{
    OT_UNUSED_VARIABLE(aUdpSocket);
    return OT_ERROR_NONE;
}

otError otPlatUdpClose(otUdpSocket *aUdpSocket)
{
    OT_UNUSED_VARIABLE(aUdpSocket);
    return OT_ERROR_NONE;
}

otError otPlatUdpBind(otUdpSocket *aUdpSocket)
{
    OT_UNUSED_VARIABLE(aUdpSocket);
    return OT_ERROR_NONE;
}

otError otPlatUdpBindToNetif(otUdpSocket *aUdpSocket, otNetifIdentifier aNetifIdentifier)
{
    OT_UNUSED_VARIABLE(aUdpSocket);
    OT_UNUSED_VARIABLE(aNetifIdentifier);
    return OT_ERROR_NONE;
}

otError otPlatUdpConnect(otUdpSocket *aUdpSocket)
{
    OT_UNUSED_VARIABLE(aUdpSocket);
    return OT_ERROR_NONE;
}

otError otPlatUdpSend(otUdpSocket *aUdpSocket, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    OT_UNUSED_VARIABLE(aUdpSocket);
    OT_UNUSED_VARIABLE(aMessageInfo);
    return OT_ERROR_NONE;
}

otError otPlatUdpJoinMulticastGroup(otUdpSocket        *aUdpSocket,
                                    otNetifIdentifier   aNetifIdentifier,
                                    const otIp6Address *aAddress)
{
    OT_UNUSED_VARIABLE(aUdpSocket);
    OT_UNUSED_VARIABLE(aNetifIdentifier);
    OT_UNUSED_VARIABLE(aAddress);
    return OT_ERROR_NONE;
}

otError otPlatUdpLeaveMulticastGroup(otUdpSocket        *aUdpSocket,
                                     otNetifIdentifier   aNetifIdentifier,
                                     const otIp6Address *aAddress)
{
    OT_UNUSED_VARIABLE(aUdpSocket);
    OT_UNUSED_VARIABLE(aNetifIdentifier);
    OT_UNUSED_VARIABLE(aAddress);
    return OT_ERROR_NONE;
}
#endif // OPENTHREAD_CONFIG_PLATFORM_UDP_ENABLE

#if OPENTHREAD_CONFIG_DNS_UPSTREAM_QUERY_ENABLE
void otPlatDnsStartUpstreamQuery(otInstance *aInstance, otPlatDnsUpstreamQuery *aTxn, const otMessage *aQuery)
{
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aTxn);
    OT_UNUSED_VARIABLE(aQuery);
}

void otPlatDnsCancelUpstreamQuery(otInstance *aInstance, otPlatDnsUpstreamQuery *aTxn)
{
    otPlatDnsUpstreamQueryDone(aInstance, aTxn, nullptr);
}
#endif

} // extern "C"
