Import-Configuration -Type Map -Path F:\E2E\SGP_Systest\MapConfiguration.json
Activate-Configuration
Import-Configuration -Type Camera -Path F:\E2E\SGP_Systest\VideoCameraConfigurationSysTest_SGP_4_MixCams.json
Activate-Configuration
Import-Configuration -Type Tracking -Path F:\E2E\SGP_Systest\TrackingConfiguration.json
Activate-Configuration
#F:\E2E\SGP_Systest\Scripts\ImportGuardSessions.ps1