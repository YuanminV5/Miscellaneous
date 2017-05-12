#
# This PowerShell script sets up the Endeavour Backend on this machine.
#
###################################################################################################

# The location of the branch
$branch = "$($PSScriptRoot)"

# Get the asset version number
$XmlDocument = Get-Content -Path "$($PSScriptRoot)\NGSMS.OPERATORCLIENT\versions.xml"
$NGSMSAssetsVersion = ([string]([xml]$XmlDocument).Project.PropertyGroup.NGSMSAssetsVersion).Trim()
$NGSMSPlatformVersion = ([string]([xml]$XmlDocument).Project.PropertyGroup.NGSMSPlatformVersion).Trim()
$NGSMSCommonModuleVersion = ([string]([xml]$XmlDocument).Project.PropertyGroup.NGSMSCommonVersion).Trim()
$NGSMSVideoModuleVersion = ([string]([xml]$XmlDocument).Project.PropertyGroup.NGSMSVideoModuleVersion).Trim()
$NGSMSTRACKINGMODULEVERSION = ([string]([xml]$XmlDocument).Project.PropertyGroup.NGSMSTrackingVersion).Trim()

# The location of the assets folder
$assets = "$branch\ASSETS"


# Use CFX to setup the machine
###################################################################################################

# Load CFX powertool
Invoke-Expression -Command "$assets\$NGSMSAssetsVersion\PowerTools\cfx.ps1"

# Configure everything as we want it
Enable-Development -Branch $branch
Set-Configuration -Platform x86 -Configuration debug
	
# Clear Redis Cache
Write-Host "`nClearing Redis" 
Clear-Redis

# Disable/reset runtimes which were enabled before.
Write-Host "`nDisabling Runtimes:" 
$EnabledRuntimes = Get-EnabledRuntimes

Write-Host "`nDisabling Runtimes:" 
foreach ($Runtime in $EnabledRuntimes) 
{
    Disable-Runtime $Runtime.Name	
}

Write-Host ""
Write-Host "Enabling required runtimes..."
Enable-Runtime OPERATIONALMANAGEMENT -Version $NGSMSPlatformVersion -Settings @{"Warmstart" = "false"}
Enable-Runtime LICENSING -Version $NGSMSPlatformVersion -Settings @{"Warmstart" = "false"}
Enable-Runtime CONFIGURATION -Version $NGSMSPlatformVersion -Settings @{"Warmstart" = "false"}
Enable-Runtime FIER -Version $NGSMSPlatformVersion -Settings @{"Warmstart" = "false"}
Enable-Runtime COMMON -Version $NGSMSCommonModuleVersion -Settings @{"Warmstart" = "false"}
Enable-Runtime VIDEO -Version $NGSMSVideoModuleVersion -Settings @{"Warmstart" = "false"}
Enable-Runtime TRACKING -Version $NGSMSTRACKINGMODULEVERSION -Settings @{"Warmstart" = "false"}
Enable-Runtime DIB-HOST	-Version $NGSMSPlatformVersion -Settings @{"Name"="DIB-HOST"; "DeviceBusHostName" = "DeviceBusHost"; "Warmstart"= "false"}	
Enable-Runtime DIB-SERVICE -Version $NGSMSPlatformVersion -Settings @{"DeviceBusHosts" = "DeviceBusHost"; "Warmstart"= "false"}

Write-Host ""
Write-Host "Enabled runtimes:"
Get-EnabledRuntimes | ft

Write-Host ""
Write-Host "Deploying runtimes:"
Deploy-Runtime OPERATIONALMANAGEMENT -Verbose
Deploy-Runtime LICENSING -Verbose
Deploy-Runtime CONFIGURATION -Verbose
Deploy-Runtime FIER -Verbose
Deploy-Runtime COMMON -Verbose
Deploy-Runtime VIDEO -Verbose
Deploy-Runtime TRACKING -Verbose
Deploy-Runtime DIB-HOST -Verbose
Deploy-Runtime DIB-SERVICE -Verbose

#Write-Host ""
#Write-Host "Deploying databases:"
Deploy-Database CONFIGURATION -Verbose
Deploy-Database FIER -Verbose
Deploy-Database COMMON -Verbose
Deploy-Database VIDEO -Verbose
Deploy-Database TRACKING -Verbose


# Workaround for broken CFX tool
Sign-Manifests -Input $branch\DevInt\Manifests -Certificate "01792FCD08679DD162DE8D13E025A5E8DBEA894B"

iisreset
