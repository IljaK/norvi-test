#pragma once
#include "common/Timer.h"
#include "common/Util.h"
#include "array/StringStackArray.h"
#include "quectel/QuectelGSMManager.h"

class GSMController:
    public IGPRSHandler,
    public IGSMCallManager,
    public IGSMNetworkManager,
    public QuectelGSMManager
{

public:

    /**
     * Constructor
     */
    GSMController();
    virtual ~GSMController();

    /**
     * Main loop for scheduler
     */
    void Start(const char *simPin);

    void OnModemStartReboot() override;
    void OnModemBooted() override;
    void OnModemFailedBoot() override;

    void OnGSMConnected() override;
    void OnGSMSimUnlocked() override;
    void OnGSMFailed(GSM_FAIL_STATE failState) override;

    void OnGPRSActivated() override;
    void OnGPRSDeactivated() override;

    bool OnGSMResponse(BaseModemCMD *request, char * response, size_t respLen, MODEM_RESPONSE_TYPE type) override;
    bool OnGSMEvent(char * data, size_t dataLen) override;

    bool OnSMSSendStream(Print *smsStream, char *phoneNumber, uint8_t customData) override;
    void OnSMSReceive(IncomingSMSInfo *smsInfo, char *message, size_t messageLen) override;

    void OnGSMNetworkType(GSM_NETWORK_TYPE type) override {};

    void DeactivateGPRS();

    time_t GetUTCTime();
    time_t GetLocalTime();
    GSMNetworkStats GetGSMStats();
    bool IsGSMReady();
    bool IsGPRSReady();

protected:
    const char *simPin;
    bool simLocked = false;

    bool AddCommand(BaseModemCMD *cmd) override;
    bool ForceCommand(BaseModemCMD *cmd) override;

    void ConnectGPRS();

    virtual void ResetSerial(uint32_t baud, uint32_t config, int8_t rxPin=-1, int8_t txPin=-1, bool invert=false) override;

    void HandleCallState(VOICE_CALLSTATE state) override {};
    void OnDTMF(char sign) override {};

    void OnHostNameResolve(const char *hostName, IPAddr ip, bool isSuccess) override {};

    void OnGSMStatus(GSM_REG_STATE state) override {};
    void OnGSMQuality(uint8_t strength, uint8_t quality) override {};
    void OnGSMThreshold(GSM_THRESHOLD_STATE type) override {};
};
