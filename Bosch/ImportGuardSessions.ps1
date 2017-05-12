#Requires -Version 3
#
# This PowerShell script provides Endeavour-specific functionality. This script requests security token 
# for authentication to the Endeavour-Backend and creates new Guard Tracking Sessions for the 
# guard specified in the Guard Session Configuration file.
#

[CmdletBinding()]
Param
(
	[Parameter(Mandatory = $false)]
	[string]
	$EndeavourConfiguration
)

import-module "$PSScriptRoot\InvokeEndeavourAuthenticate.psm1" -Force
Import-Module "$PSScriptRoot\GuardSessionMgmt.psm1" -Force

# the guard session configuration file
$configurationFile = "F:\E2E\SGP_Systest\GuardSessions\TrackingSessions.json";

#$parentDir = Split-Path $PSScriptRoot -Parent
#$configurationFile = Join-Path $parentDir "Assets"
#$configurationFile = Join-Path $configurationFile "SchipholMap"
#$configurationFile = Join-Path $configurationFile "GuardSessions";
#$configurationFile = Join-Path $configurationFile "GuardTrackingTestSettings.json";

# user used for authentication
$user ="bob";

$identityProviderUri = "https://localhost/DeveloperIdentityProvider/auth1/token";

# request new token
$token = Invoke-EndeavourAuthenticate -User $user -Password $user -IdentityProviderUri $identityProviderUri

if( Test-Path $configurationFile )
{
    Write-Output "Importing Guard sessions from $configurationFile."
}
else
{
    Write-Error "$configurationFile does not exist!"
}

# create guard sessions
Initialize-GuardSessions -Configuration $configurationFile -Token $token -Hostname "localhost"

