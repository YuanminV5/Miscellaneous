$SettingFile = "C:\.CopyBinaryPoolSettings"

if (!(Test-Path $SettingFile -PathType Leaf))
{
    # #Requires -RunAsAdministrator
    if (-NOT ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator"))
    {    
        Write-Error "This script needs to be run as administrator at the first time."
        break
    }

	Write-Host "Setting file is not found, writing setting file..."

	New-Item $SettingFile -type file -force | Out-Null

    # Open folder browser dialog to select local store location.
    Add-Type -AssemblyName System.Windows.Forms
 
    $FolderBrowser = New-Object System.Windows.Forms.FolderBrowserDialog -Property @{
        RootFolder = 'MyDocuments'
        ShowNewFolderButton = $false
    }
 
    if($FolderBrowser.ShowDialog() -eq "OK")
    {
        $LocalStore = $FolderBrowser.SelectedPath
    }


    # Write settings to setting file.
    Add-Content $SettingFile -Value "LocalStore,$LocalStore"
    Add-Content $SettingFile -Value "LocalLatest,"
    Add-Content $SettingFile -Value "ASSETS,\\bosi-eng.global\shares\GRBEndvAS"
    Add-Content $SettingFile -Value "FIER,\\bosi-eng.global\shares\GRBEndvBP\FIER"
    Add-Content $SettingFile -Value "COMMON,\\bosi-eng.global\shares\GRBEndvBP\COMMON"

}

$Settings = @{}

Get-Content $SettingFile | ForEach-Object `
{
    $Var = $_.Split(',')
    try
    {
        $Settings.Add($Var[0], $Var[1])
    }
    catch
    {
        $Settings.Set_Item($Var[0], $Var[1])
    }
}
  
if($Settings.Count -eq 0)
{
    Write-Error "Empty setting file! Delete it manually and try again!"
    break
}

# Binary pool source 


if ($Settings.ContainsKey("ASSETS"))
{
    $ASSETSSource = $Settings.ASSETS
}

if ($Settings.ContainsKey("FIER"))
{
    $FIERSource = $Settings.FIER
}

if ($Settings.ContainsKey("COMMON"))
{
    $COMMONSource = $Settings.COMMON
}

# Local store
if ($Settings.ContainsKey("LocalStore"))
{
    $LocalStore = $Settings.LocalStore
}


$ASSETSLocal = "$LocalStore\ASSETS"
$FIERLocal = "$LocalStore\BP\FIER"
$COMMONLocal = "$LocalStore\BP\COMMON"

# Create local store folder structure as
# 
# LocalStoreFolder
#   -ASSETS
#   -BP
#     -FIER
#     -COMMON

if (!(Test-Path $ASSETSLocal -PathType Container))
{
	New-Item $ASSETSLocal -type directory 
}


if (!(Test-Path $FIERLocal -PathType Container))
{
	New-Item $FIERLocal -type directory 
}

if (!(Test-Path $COMMONLocal -PathType Container))
{
	New-Item $COMMONLocal -type directory 
}


# Copy from source to local

if ($Settings.ContainsKey("LocalLatest"))
{
    $LocalLastestVersion = $Settings.LocalLatest
}

$Versions = Get-Item "$FIERSource\*.zip" | Select-Object Name

Write-Host $Versions

[string] $SourceLatest = $Versions[-1].Name.Replace(".zip", "")
Write-Host $SourceLatest

if(!$LocalLastestVersion -or ($LocalLastestVersion.CompareTo($SourceLatest) -ne 0))
{
    $LocalLastestVersion = $SourceLatest
    $DoCopy = $true
}

if($DoCopy)
{
}