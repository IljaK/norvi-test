#include "GSMController.h"

GSMController::GSMController():
    QuectelGSMManager(&Serial2, GSM_RESET_PIN)
{
    this->simPin = simPin;
    callManager.SetCallStateHandler(this);
    gsmManager.SetGSMListener(this);
    gprsManager.SetAPNHandler(this);
}

GSMController::~GSMController()
{
}

void GSMController::ResetSerial(uint32_t baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
{
    GSMSerialModem::ResetSerial(baud, config, GSM_RX_PIN, GSM_TX_PIN, invert);

    // Reassign pins
    HardwareSerial * serial = GetSerial();
    if (serial == NULL) return;

    if (debugPrint != NULL) {
        debugPrint->print("setPins RX:");
        debugPrint->print((int)GSM_RX_PIN);
        debugPrint->print(", TX:");
        debugPrint->print((int)GSM_TX_PIN);

        debugPrint->print(", resetPin:");
        debugPrint->println((int)resetPin);
    }
}

void GSMController::Start(const char *simPin)
{
    this->simPin = simPin;

    if (simPin == NULL) {
        Serial.println("Missing sim code");
        return;
    }

    if (Serial) {
        SetDebugPrint(&Serial);
    }
    StartModem();
}

void GSMController::OnModemStartReboot()
{

}

void GSMController::OnModemBooted()
{
    gsmManager.Connect(simPin);
}

void GSMController::OnModemFailedBoot()
{
    StartModem();
}

void GSMController::OnGSMFailed(GSM_FAIL_STATE failState)
{
    switch (failState)
    {
    case GSM_FAIL_OTHER_PIN:
        simLocked = true;
        // This is not modem issue, no need to reboot indefinitely
        break;
    default:
        StartModem();
        break;
    }
}

void GSMController::OnGSMConnected()
{
    Serial.println();
    Serial.println("-------------------------------");
    Serial.println("Ready to accept incoming calls!");
    Serial.println("-------------------------------");
    Serial.println();
}

void GSMController::OnGSMSimUnlocked()
{
}

void GSMController::ConnectGPRS()
{
    if (debugPrint != NULL) {
        debugPrint->println(PSTR("GSMController::ConnectGPRS"));
    }

    // TODO:

    char* apnAddr = "";
    char* apnLogin = "";
    char* apnPass = "";

    gprsManager.Connect(apnAddr, apnLogin, apnPass);
}


void GSMController::DeactivateGPRS()
{
    if (gprsManager.IsActive()) {
        gprsManager.Deactivate();
        return;
    }
}

void GSMController::OnGPRSActivated()
{
    if (debugPrint != NULL) {
        IPAddr ip = gprsManager.GetDeviceAddr();
        debugPrint->print("IP: ");
        debugPrint->print(ip.octet[0]);
        debugPrint->print('.');
        debugPrint->print(ip.octet[1]);
        debugPrint->print('.');
        debugPrint->print(ip.octet[2]);
        debugPrint->print('.');
        debugPrint->print(ip.octet[3]);
        debugPrint->println();
    }
}

void GSMController::OnGPRSDeactivated()
{

}

bool GSMController::OnGSMResponse(BaseModemCMD *request, char * response, size_t respLen, MODEM_RESPONSE_TYPE type)
{
    if (QuectelGSMManager::OnGSMResponse(request, response, respLen, type)) {
        return true;
    }
    return false;
}
bool GSMController::OnGSMEvent(char * data, size_t dataLen)
{
    if (QuectelGSMManager::OnGSMEvent(data, dataLen)) {
        return true;
    }
    return false;
}

bool GSMController::OnSMSSendStream(Print *smsStream, char *phoneNumber, uint8_t customData)
{
    return false;
}

void GSMController::OnSMSReceive(IncomingSMSInfo *smsInfo, char *message, size_t messageLen)
{
}


bool GSMController::AddCommand(BaseModemCMD *cmd)
{
    bool result = QuectelGSMManager::AddCommand(cmd);
    if (!result) {
        StartModem();
    }
    return result;
}
bool GSMController::ForceCommand(BaseModemCMD *cmd)
{
    bool result = QuectelGSMManager::ForceCommand(cmd);
    if (!result) {
        StartModem();
    }
    return result;
}

time_t GSMController::GetUTCTime()
{
    return gsmManager.GetUTCTime();
}

time_t GSMController::GetLocalTime()
{
    return gsmManager.GetLocalTime();
}

GSMNetworkStats GSMController::GetGSMStats() {
    return gsmManager.GetGSMStats();
}

bool GSMController::IsGSMReady()
{
    return gsmManager.IsInitialized();
}
bool GSMController::IsGPRSReady()
{
    return gprsManager.IsActive();
}
