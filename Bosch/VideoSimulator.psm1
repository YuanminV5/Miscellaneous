# <copyright file="VideoSimulator.psm1" company="Bosch Security Systems GmbH">
#   The information contained herein is confidential, proprietary to
#   Bosch Security Systems GmbH, and considered a trade secret.
#   Use of this information by anyone other than authorized employees of
#   Bosch Security Systems GmbH is granted only under a written non-disclosure
#   agreement, expressly prescribing the scope and manner of such use.
# </copyright>

# if this module is already loaded, do nothing.
if (Get-Module VideoSimulator) {return}

$MaxAllowedSimulator = 100
$IPType = "IPv4"

# Default parameters for the device simulator; these values will be overriden if user passes correct parameters
$DefaultStartIP = "192.168.1.2"
$DefaultNetmask = "255.255.0.0"
$DefaultDeviceProductID = 39
$DefaultDeviceVariantID = 0
$DefaultAppBinPath = "Firmware" # Relative path to where vj_wsock.exe and script are located
$DefaultJpegPath = "Yuv"
$DefaultVideoFile = "Media\testIntIP.mp4" 
$DefaultCfgPath = "Cfg" 
$DefaultRecordBuffSize = 4096
$DefaultHttpPort = 80
$DefaultHttpsPort = 443
$MaxPort = 65535

$DefaultPreConfigPath = "PreConfig"
$SimulatorAppName ="vj_wsock"

$Script:PreConfigPath = $DefaultPreConfigPath

# Persistent file used to track added IP addresses and simulator processes started by VideoSimulator
# Line format: "IP,ProcessId"
$Script:PersistentStore = "$PSScriptRoot\PersistentStore"
$Script:NIC_NAME = "BoschVideoSimulatorLoopbackAdaptor"

Write-Host -ForegroundColor Green @'

 _______ .__   __.  _______   _______     ___   ____    ____  ______    __    __  .______         
|   ____||  \ |  | |       \ |   ____|   /   \  \   \  /   / /  __  \  |  |  |  | |   _  \        
|  |__   |   \|  | |  .--.  ||  |__     /  ^  \  \   \/   / |  |  |  | |  |  |  | |  |_)  |       
|   __|  |  . `  | |  |  |  ||   __|   /  /_\  \  \      /  |  |  |  | |  |  |  | |      /        
|  |____ |  |\   | |  '--'  ||  |____ /  _____  \  \    /   |  `--'  | |  `--'  | |  |\  \----.   
|_______||__| \__| |_______/ |_______/__/     \__\  \__/     \______/   \______/  | _| `._____|   
                                                                                                  
____    ____  __   _______   _______   ______                                                     
\   \  /   / |  | |       \ |   ____| /  __  \                                                    
 \   \/   /  |  | |  .--.  ||  |__   |  |  |  |                                                   
  \      /   |  | |  |  |  ||   __|  |  |  |  |                                                   
   \    /    |  | |  '--'  ||  |____ |  `--'  |                                                   
    \__/     |__| |_______/ |_______| \______/                                                    
                                                                                                  
     _______. __  .___  ___.  __    __   __          ___   .___________.  ______   .______        
    /       ||  | |   \/   | |  |  |  | |  |        /   \  |           | /  __  \  |   _  \       
   |   (----`|  | |  \  /  | |  |  |  | |  |       /  ^  \ `---|  |----`|  |  |  | |  |_)  |      
    \   \    |  | |  |\/|  | |  |  |  | |  |      /  /_\  \    |  |     |  |  |  | |      /       
.----)   |   |  | |  |  |  | |  `--'  | |  `----./  _____  \   |  |     |  `--'  | |  |\  \----.  
|_______/    |__| |__|  |__|  \______/  |_______/__/     \__\  |__|      \______/  | _| `._____|  
                                                                                                  
                                                                                               
Available powershell commands on VideoSimulator can be retrieved via "Get-Command -Module VideoSimulator".
To start VideoSimulator instances, use command Start-BoschVideoSimulator(retrive help via Get-Help Start-BoschVideoSimulator -detailed).
'@

function Create-BoschVideoSimulatorLoopbackAdapter()
{
    <#    
    .SYNOPSIS
    Create the loopback interface

    .DESCRIPTION
    create the loop back interface  for running the device simulator.
        
    .EXAMPLE 
    # Create-BoschVideoSimulatorLoopbackAdaptor
    #>
   
    .\devcon.exe -r install $env:windir\Inf\Netloop.inf *MSLOOP | Out-Null

    $loopbackAdaptor = Get-NetAdapter|where{$_.DriverDescription -eq "Microsoft KM-TEST Loopback Adapter"}

    # Rename the loopback NIC to "BoschVideoSimulatorLoopbackAdaptor"
    Rename-NetAdapter -Name $loopbackAdaptor.name -NewName $Script:NIC_NAME
    Set-NetIPInterface -InterfaceIndex $loopbackAdaptor.ifIndex -InterfaceMetric "254" -WeakHostReceive Enabled -WeakHostSend Enabled  -Dhcp Enabled 
}

try
{        
    $Adapter = Get-NetAdapter -ErrorAction Stop | ? {$_.Status -eq "up"} | Select-Object -first 1
    if (!$Adapter)
    {
        Write-Error "No available network connection. Please make sure you have WIFI or Ethernet connection."
    }
    #v0.2
    # This script script block will be enabled for next version for using the loopback adaptor for running the bosch video simulator.
    #$LoopBackAdaptorExist  =  Get-NetAdapter | where-object Name -eq $Script:NIC_NAME
    #if ($LoopBackAdaptorExist -eq $null)
    #{
    #    Create-BoschVideoSimulatorLoopbackAdapter
    #}
    # endof the script block.
} 
catch
{  
  Write-Error "Error occurred while getting network adapter configuration of $env:computer."
}     

# Pre-defined devices with "productID_variantID" 
$VideoDeviceFamily = @{

    ALL_VIDEO_DEVICES               = (255, 0)
    DINION_IP_7000_HD 				= (29, 0)
    DINION_IP_7000_HD_VR            = (29, 61)
    DINION_IP_7000_HD_IAV_IE        = (29, 33)
    NDC_274_P 						= (31, 0)
    AUTODOME_IP_7000_HD 			= (35, 0)
    AUTODOME_IP_7000_HD_IE          = (35, 5)
    VIP_X16_XF_E 					= (37, 0)
    TINYON_IP_2000 					= (38, 0)
    FLEXIDOME_IP_INDOOR_5000_HD 	= (39, 0)
    DINION_IP_IMAGER_9000_HD 		= (40, 0)
    FLEXIDOME_IP_CORNER_9000_MP 	= (41, 0)
    DINION_IP_5000_HD 				= (44, 0)
    VIDEOJET_MULTI_7000 			= (46, 0)
    EXTEGRA_IP_DYNAMIC_9000_FX 		= (49, 0)
    AUTODOME_IP_4000_HD 			= (51, 0)
    DINION_IP_BULLET_4000_HD 		= (52, 0)
    FLEXIDOME_IP_PANORAMIC_7000_MP  = (53, 0)
    JUPITER_1080P 					= (56, 0)
    DINION_IP_STARLIGHT_7000_HD		= (57, 0)
    PLUTO_1080P 					= (58, 0)
    DINION_IP_THERMAL_8000 			= (61, 0)
    AUTODOME_STARLIGHT_4000_HD		= (63, 0)
}

function Show-PreDefinedBoschVideoDevices()
{
    <#    
    .SYNOPSIS
        Show all pre-defined Bosch video devices.	
        
    .DESCRIPTION
        All pre-defined Bosch video devices will be shown.
         
    .EXAMPLE 
        Show-PreDefinedBoschVideoDevices		
    #>

    $VideoDeviceFamily.GetEnumerator() | ? {$_.Key -notcontains "ALL_VIDEO_DEVICES"} | % {$_.Key}
}

function Start-BoschVideoSimulator()
{
    <#    
    .SYNOPSIS
        Start simulator instances with given parameters.

    .DESCRIPTION
    Start simulator instances with given parameters. If no parameter is given explicitly, a single
    device instance will be started with default parameters.

    .PARAMETER Mode
    Specify in what mode the simulator will be started. Valid value: Config, Execution
    - Config:
    This mode is used to configure a given device and save a configuration file which will be used later to start multiple instances 
    of same device type. The configuration file will be saved to location specified by parameter $PreConfigPath.
         
    - Execution:
    This mode is used to start multiple device instances with pre-configuration file saved in pre-configuration path. 
    If no pre-configuration file is available in the location then the default setting will be used.
    Default: Execution

    .PARAMETER NumberOfDevices
        Number of device instances to be started. In "Config" mode this parameter will be ignored and only single instance will be started.
        This value shall not exceed maximum limit of 100 devices due to the high execution resource(CPU, RAM) usage.
        Default: 1
                   
    .PARAMETER StartIPAddress
        Start address of IP range used for simulator instances. 
        Default: 192.168.1.2

    .PARAMETER Netmask
        Netmask for the newly added IP addresses.
        Default: 255.255.0.0
                   
    .PARAMETER DeviceFamily
        Type of device to be started. 
        - If this parameter is set, $DeviceProductID and $DeviceVariantID will be ignored.
        - If ALL_VIDEO_DEVICES selected, all pre-defined devices will be started in loop until reaching $NumberOfDevices.

    .PARAMETER AppBin
        The application binary(WebGUI files) used to start the device.
        Valid value: "app1_amba_A5.bin", "app1_amba_S2.bin", "app1_tc.bin", "app1_vipx.bin"
        Default: "app1_amba_A5.bin"

    .PARAMETER DeviceProductID
        Product ID of the device to be started. This paramter will be ignored if $DeviceFamily is set.
        Default: 39
            
    .PARAMETER DeviceVariantID
        Variant ID of the device to be started. This paramter will be ignored if $DeviceFamily is set.
        Default: 0

    .PARAMETER StreamingImagePath
        Path of YUV image for jpeg streaming. 
        This must be a relative path to where this script and simulator executable are located.
        Default: YUV

    .PARAMETER VideoFile
        Path of video file used for live streaming and recording. 
        This must be a relative path to where this script and simulator executable are located.
        Default: Media\testIntIP.mp4

    .PARAMETER RecordBufferSize
        Video recording buffer size in KB. Default value is sufficient in most cases. May need to be increased for high resolution videos.
        Default: 4096

    .PARAMETER PreConfigPath
        Path to configuration file used to start instances of devices. 
        This must be a relative path to where this script and simulator executable are located.

    .PARAMETER CfgPrefix
        The prefix for configuration file.

    .PARAMETER HttpPort
        The HTTP Port the simulators will listen.
		Default: 80

    .PARAMETER HttpsPort
        The HTTPS Port the simulators will listen.
		Default: 443

	.PARAMETER CmdLineOptions
		Provide command line options to Simulator executable. Type 'vj_wsock.exe -help' for more information.
         
    .EXAMPLE
        # Start simulator with default parameters
        Start-BoschVideoSimulator
          
    .EXAMPLE 
        # Pre-configure a device with configuration prefix 
        Start-BoschVideoSimulator -Mode Config -DeviceFamily TINYON_IP_2000 -PreConfigPath PreConfigDevice -CfgPrefix TINYON_Device

    .EXAMPLE 
        # Start devices with given pre-configuration file from PreConfigpath path
        Start-BoschVideoSimulator -Mode Execution -NumberOfDevices 10 -StartIPAddress 192.168.1.5 -Netmask 255.255.255.0 
                                -DeviceFamily TINYON_IP_2000 -PreConfigPath PreConfigDevice -CfgPrefix TINYON_Device
    .EXAMPLE
        # Start devices not in the pre-defined DeviceFamily list, here VOT-320V IVA (Viper)
        Start-BoschVideoSimulator -Mode Execution -NumberOfDevices 10 -StartIPAddress 192.168.1.5 -Netmask 255.255.255.0 
                                -DeviceProductID 24 -DeviceVariantID 0 -AppBin app1_amba_A2.bin
    .EXAMPLE 
        # Start all devices in pre-defined DeviceFamily list
        Start-BoschVideoSimulator -NumberofDevices 20 -DeviceFamily ALL_VIDEO_DEVICES
          
    #>

    [CmdletBinding()]
    Param
    (
    [Parameter(Position = 0, Mandatory = $false)]
    [string]
    [ValidateSet("Config","Execution")]
    $Mode = "Execution",

    [Parameter(Position = 1, Mandatory = $false)]
    [int]
    [ValidateScript({$_ -le $MaxAllowedSimulator})]
    $NumberOfDevices = 1,
        
    [Parameter(Position = 2, Mandatory = $false)]
    [string]
    [ValidateScript({[BOOL]($_ -as [IPADDRESS])})]
    $StartIPAddress = $DefaultStartIP,
        
    [Parameter(Position = 3, Mandatory = $false)]
    [string]
    [ValidateScript({[BOOL]($_ -as [IPADDRESS])})]
    $Netmask = $DefaultNetmask,
        
    [Parameter(Position = 5, Mandatory = $false)]
    [int]
    $DeviceProductID = $DefaultDeviceProductID,

    [Parameter(Position = 6, Mandatory = $false)]
    [int]
    $DeviceVariantID = $DefaultDeviceVariantID,

    [Parameter(Position = 7, Mandatory = $false)]
    [string]
    [ValidateSet("app1_amba_A5.bin", "app1_amba_S2.bin", "app1_tc.bin", "app1_vipx.bin")]
    $AppBin = "app1_amba_A5.bin",

    [Parameter(Position = 8, Mandatory = $false)]
    [string]
    [ValidateScript({Test-Path "$PSScriptRoot\$_" -PathType Container})]
    $StreamingImagePath = $DefaultJpegPath,

    [Parameter(Position = 9, Mandatory = $false)]
    [string]
    [ValidateScript({Test-Path "$PSScriptRoot\$_" -PathType Leaf})]
    $VideoFile = $DefaultVideoFile,

    [Parameter(Position = 10, Mandatory = $false)]
    [int]
    $RecordBufferSize = $DefaultRecordBuffSize,
        
    [Parameter(Position = 11, Mandatory = $false)]
    [string]
    $PreConfigPath,

    [Parameter(Position = 12, Mandatory = $false)]
    [string]
    $CfgPrefix,

    [Parameter(Position = 13, Mandatory = $false)]
    [int]
    [ValidateScript({$_ -le $MaxPort -and $_ -gt 0})]
    $HttpPort = $DefaultHttpPort,

    [Parameter(Position = 14, Mandatory = $false)]
    [int]
    [ValidateScript({$_ -le $MaxPort -and $_ -gt 0})]
    $HttpsPort = $DefaultHttpsPort,

    [Parameter(Position = 15, Mandatory = $false)]
    [string]
    $CmdLineOptions
    )

    DynamicParam 
    {
    # Define dynamic parameter DeviceFamily with validate set from keys of $VideoDeviceFamily
    $ParameterName = 'DeviceFamily'

    $RuntimeParameterDictionary = New-Object System.Management.Automation.RuntimeDefinedParameterDictionary
    $AttributeCollection = New-Object System.Collections.ObjectModel.Collection[System.Attribute]
            
    $ParameterAttribute = New-Object System.Management.Automation.ParameterAttribute
    $ParameterAttribute.Mandatory = $false
    $ParameterAttribute.Position = 4

    $AttributeCollection.Add($ParameterAttribute)

    # Set ValidateSet 
    $ValidateSetAttribute = New-Object System.Management.Automation.ValidateSetAttribute($VideoDeviceFamily.Keys)

    $AttributeCollection.Add($ValidateSetAttribute)
    $RuntimeParameter = New-Object System.Management.Automation.RuntimeDefinedParameter($ParameterName, [string], $AttributeCollection)
    $RuntimeParameterDictionary.Add($ParameterName, $RuntimeParameter)

    return $RuntimeParameterDictionary
    }

    Begin 
    {
        # Bind the parameter to a friendly variable
        $DeviceFamily = $PsBoundParameters[$ParameterName]
    }
    
    Process
    {
    # Split IP address and convert last byte to integer
    $IPBytes = $StartIPAddress.split('.')

    if (([int]$IPBytes[0] -gt 255) -or ([int]$IPBytes[1] -gt 255) -or ([int]$IPBytes[2] -gt 255) -or ([int]$IPBytes[3] -gt 255))
	{
        Write-Error "Invalid IP address: $StartIPAddress"
		return
	}
	
	
    # Skip reserved IP 0 and 1
    if (([int]$IPBytes[3] -eq 0) -or ([int]$IPBytes[3] -eq 1))
    {
        $IPBytes[3] = [int]$IPBytes[3] + 1
    }

    $ExecutionCfgPath = $DefaultCfgPath

    if ($Mode.CompareTo("Config") -eq 0)
    {
        Write-Output "VideoSimulator is in Config mode, parameter NumberOfDevices will be ignored and only single instance will be started."
        $NumberOfDevices = 1

        if (!$PreConfigPath)
        {
            $PreConfigPath = $Script:PreConfigPath
        }

        if (!(Test-Path "$PSScriptRoot\$PreConfigPath" -PathType Container))
        {
            try
            {
                mkdir "$PSScriptRoot\$PreConfigPath"
            }
            catch
            {
                Write-Error "Failed to create pre-configuration path $PSScriptRoot\$PreConfigPath. Please check parameter PreConfigPath."
            }
        }

        $Script:PreConfigPath = $PreConfigPath
        $ExecutionCfgPath = $PreConfigPath
    }
    
    if ($NumberOfDevices -gt $MaxAllowedSimulator)
    {
        Write-Error "Number of devices to be started reaches more than allowed limit ..." + $MaxAllowedSimulator
        return 
    }

    # Update persistent store: remove records that are obsolete
    Write-Host "Synchronizing IP addresses added by VideoSimulator..."
        
    $RecordsInStore = GetRecordsFromStore
	
    if ($RecordsInStore.SimulatorMode -and ($RecordsInStore.SimulatorMode.CompareTo("Execution") -ne 0 -or $RecordsInStore.SimulatorMode.CompareTo("Execution") -ne 0))
	{
		Write-Error "It seems there are still simulators running, please call Stop-BoschVideoSimulator first, if you don't run all devices explicitly in Execution mode."
		return
	}
	
    $RecordsInStore.Set_Item("SimulatorMode", $Mode)
	
	
    if (-Not $RecordsInStore.Simulators)
    {
		$RecordsInStore.Set_Item("Simulators", @{})
    }

	$newSimulators = @{}
	$RecordsInStore.Simulators.GetEnumerator() | ForEach-Object `
	{
		$Process = Get-Process -Id $_.Value -ErrorAction SilentlyContinue
		if ($Process -and ($Process.Name -contains $SimulatorAppName))
		{
			$newSimulators.Add($_.Key, $_.Value)
		}
		else
		{
			RemoveIPFromNetworkCard -IP $_.Key
			$Pattern = $_.Key + "*.bin"
			Remove-Item $PSScriptRoot\$DefaultCfgPath\$Pattern -ErrorAction SilentlyContinue 
			sleep -Milliseconds 50
		}
	}
	$RecordsInStore.Set_Item("Simulators", $newSimulators)

    # if ($RecordsInStore.Simulators.Count -gt ($MaxAllowedSimulator -1))
    # {
        # Write-Host "Number of running devices reaches allowed limit ..." + $MaxAllowedSimulator
        # return 
    # }

    # If network card DHCP is enable, switch to static and set static ip address
	
	if(-Not $RecordsInStore.DhcpState)
	{
		$Adapter = Get-NetAdapter -ErrorAction Stop | ? {$_.Status -eq "up"} | Select-Object -first 1
		if (!$Adapter)
		{
			Write-Error "No available network connection. Please make sure you have WIFI or Ethernet connection."
		}
		$NetworkIPConfiguration = GetNetworkIpConfiguration
		$RecordsInStore.Set_item("DhcpState", $NetworkIPConfiguration.DHCPEnabled)
	
        if ($RecordsInStore.DhcpState -eq $true)
        {
            $IPAddress  = $NetworkIPConfiguration.IpAddress[0]            
            $SubnetMask  = $NetworkIPConfiguration.IPSubnet[0]            
            $DefaultGateway = $NetworkIPConfiguration.DefaultIPGateway            
            $DNSServers  = $NetworkIPConfiguration.DNSServerSearchOrder 

            $NetworkIPConfiguration.EnableStatic($IPAddress, $SubnetMask) | Out-Null
            Sleep -Seconds 2

            $NetworkIPConfiguration.SetGateways($DefaultGateway) | Out-Null
            $NetworkIPConfiguration.SetDNSServerSearchOrder($DNSServers) | Out-Null
        }
    }

    Write-Host "Adding IP addresses..."
    $AddedIPAddresses= @()
    while (($AddedIPAddresses.Count -lt $NumberOfDevices) -and ($AddedIPAddresses.Count -lt ($MaxAllowedSimulator - $RecordsInStore.Count)))
    {
		if([int]$IPBytes[3] -ge 255)
		{
			$IPBytes[3] = 2 # just start with 2, as we had some problems with IP x.y.z.1(?)
			$IPBytes[2] = [int]$IPBytes[2] + 1
			if([int]$IPBytes[2] -ge 255)
			{
				$IPBytes[2] = 0
				$IPBytes[1] = [int]$IPBytes[1] + 1
				if([int]$IPBytes[1] -ge 255)
				{
					$IPBytes[1] = 0
					$IPBytes[0] = [int]$IPBytes[0] + 1
				}
			}
		}

        $DeviceIP = $IPBytes[0] + '.' + $IPBytes[1] + '.' + $IPBytes[2] + '.' + $IPBytes[3]
		$IPBytes[3] = [int]$IPBytes[3] + 1
		
        if ($RecordsInStore.Simulators.ContainsKey("$DeviceIP"))
        {
            Write-Host "IP $DeviceIP already exists and occupied"
            continue
        }

        try
        {
            $PrefixLength = NetmaskToPrefixLength -Netmask $Netmask
            $Adapter | New-NetIPAddress -AddressFamily $IPType -IPAddress  $DeviceIP -PrefixLength $PrefixLength -ErrorAction Stop | Out-Null #TODO: netmask -> prefix 
            $AddedIPAddresses += $DeviceIP
            Write-Output "IP address $DeviceIP is added"
            sleep -Milliseconds 50
			$RecordsInStore.Simulators.Add($DeviceIP, $null)
        }
        catch
        {
            $IPExists = Get-NetIPAddress -IPAddress $DeviceIP -ErrorAction SilentlyContinue
            if ([BOOL]$IPExists)
            {
                Write-Output "$DeviceIP already exists, this IP will be skipped"
            }
            else
            {
                Write-Output "Failed to add IP $DeviceIP, this IP will be skipped"
            }
        }
    }
        
    if ($AddedIPAddresses.Count -eq 0)
    {
        Write-Output "Failed to add IP address for device. No device will be started"
        return
    }

    Write-Output "Number of added IP address:" $AddedIPAddresses.Count
    Write-Output "Waiting for network configuration taking effect..."
        
    sleep 5
        
    Write-Output "Starting simulator instances..."
    foreach ($IP in $AddedIPAddresses)
    {
        [string] $DeviceIP = $IP
        [string] $DeviceName = ""
        $CopyDeviceProductID = $DeviceProductID
        $CopyDeviceVariantID = $DeviceVariantID

        if ([BOOL]$DeviceFamily -and ($VideoDeviceFamily.ContainsKey($DeviceFamily)))
        {
            $CopyDeviceProductID = [int]$VideoDeviceFamily.$DeviceFamily[0]
            $CopyDeviceVariantID = [int]$VideoDeviceFamily.$DeviceFamily[1]
            $DeviceName = [string]$DeviceFamily
                
            if ($DeviceName.CompareTo("ALL_VIDEO_DEVICES") -eq 0)
            {
                $Index = ($AddedIPAddresses.IndexOf($IP) % ($VideoDeviceFamily.Count - 1))
                $Item =  $VideoDeviceFamily.GetEnumerator() | ? {$_.Key.CompareTo("ALL_VIDEO_DEVICES") -ne 0} | Select-Object -Index $Index 
                $CopyDeviceProductID =$Item.Value[0]
                $CopyDeviceVariantID = $Item.Value[1]
                $DeviceName = $Item.Key
            }
        }
        else 
        {
            # Look up pre-defined device list to find device name
            $DeviceName = "Unknown"
            foreach ($Item in $VideoDeviceFamily.GetEnumerator())
            {
                if (($Item.Value[0] -eq $CopyDeviceProductID) -and ($Item.Value[1] -eq $CopyDeviceVariantID))
                {
                    $DeviceName = $Item.Key
                    break;
                }
            }
        }

        if (!$PreConfigPath)
        {
            $PreConfigPath = $Script:PreConfigPath
        }

        $CatCfgPrefix = $CfgPrefix

        if (!$CatCfgPrefix)
        {
            if ($Mode.CompareTo("Config") -eq 0)
            {
                $CatCfgPrefix = $DeviceName
            }
            else
            {
                $CatCfgPrefix = [string]$DeviceIP	
            }
        }	
            
        $CatCfgPrefix = $CatCfgPrefix + "_"
            
        # In case of Execution mode, copy pre-configuration files to execution cfg path and rename them with IP address
        if (($Mode.CompareTo("Execution") -eq 0) -and ($DeviceName.CompareTo("Unknown") -ne 0) -and (Test-Path "$PSScriptRoot\$PreConfigPath" -PathType Container))
        {
            $Pattern = "$CatCfgPrefix(cfg|cfg_bbss|cfg_perm|cfg_perm2|ramdisk).bin"

            Get-ChildItem "$PSScriptRoot\$PreConfigPath" | ? {$_.Name -match $Pattern} | ForEach-Object `
            {
                $NewName = $_.Name.Replace("$CatCfgPrefix", "$DeviceIP"+"_")
                Copy-Item -Path $_.FullName -Destination "$PSScriptRoot\$ExecutionCfgPath\$NewName"
            }
        }

        if ($Mode.CompareTo("Execution") -eq 0)
        {
            $CatCfgPrefix = "$DeviceIP" + "_"
        }

        $AppBinFullPath = "$DefaultAppBinPath\$AppBin"

        $IndFile = $VideoFile;
        $IndFile = $IndFile -creplace ".mp4",".ind"
        if(( $IndFile -ne $VideoFile ) -and ( Test-Path "$PSScriptRoot\$IndFile" -PathType Leaf ))
        {
            $VideoAndIndFile = "$VideoFile -ind_file $IndFile"
        }
        else
        {
            $VideoAndIndFile = $VideoFile;
        }

        try
        {
			$ProccessObject = Start-Process -FilePath "$PSScriptRoot\$SimulatorAppName.exe" -WindowStyle Minimized -PassThru -ErrorAction Stop -ArgumentList "-ip $DeviceIP -subnet $Netmask -productID $CopyDeviceProductID -variantID $CopyDeviceVariantID -app_bin $AppBinFullPath -jpg_path $StreamingImagePath -mp4_file $VideoAndIndFile -rec_buf_kb $RecordBufferSize -cfg_path $ExecutionCfgPath  -cfg_prefix $CatCfgPrefix -http_port $HttpPort -https_port $HttpsPort $CmdLineOptions"

            # Keep a record in persistent store
			$RecordsInStore.Simulators.Set_Item($DeviceIP, $ProccessObject.Id)
            Write-Host IP:$DeviceIP Netmask: $Netmask DeviceFamily: $DeviceName ProductID: $CopyDeviceProductID VariantID: $CopyDeviceVariantID ConfigPath: $ExecutionCfgPath VideoFile: $VideoAndIndFile
        }
        catch
        {
            Write-Host "Failed to start simulator for IP address $DeviceIP"
            RemoveIPFromNetworkCard -IP $DeviceIP
        }

        sleep -Milliseconds 100
    }
    #Parent script reference
	SetRecordsToStore($RecordsInStore)
	
    echo $AddedIPAddresses
    }

    End
    {
    }
}

function Stop-BoschVideoSimulator()
{
    <#    
        .SYNOPSIS
         Stop all running device simulator instances.

        .DESCRIPTION
         All device simulator instances will be killed and corresponding IP addresses will be removed from network adapter.
        
        .EXAMPLE 
           Stop-BoschVideoSimulator
    #>
	Write-Output "Killing device simulator processes..."
	Stop-Process -processname  $SimulatorAppName -ErrorAction SilentlyContinue | Out-Null
	
	Write-Output "Removing added IP addresses..."

	# Remove added IP
	$RecordsInStore = GetRecordsFromStore
	
	if($RecordsInStore.Simulators)
	{
		$RecordsInStore.Simulators
		$RecordsInStore.Simulators.GetEnumerator() | ForEach-Object `
		{
			RemoveIPFromNetworkCard -IP $_.Key

			# Delete configuration files in case of Execution mode
			if ((-not $RecordsInStore.SimulatorMode) -or $RecordsInStore.SimulatorMode.CompareTo("Execution") -eq 0)
			{
				$Pattern = $_.Key + "*.bin"
				Remove-Item $PSScriptRoot\$DefaultCfgPath\$Pattern -ErrorAction SilentlyContinue 
			}
		}	
	}

	if ($RecordsInStore.DhcpState -and $RecordsInStore.DhcpState -eq $true)
	{
		$NetworkIPConfiguration = GetNetworkIpConfiguration
	
		if(-Not $NetworkIPConfiguration.DHCPEnabled)
		{
			$NetworkIPConfiguration.SetDNSServerSearchOrder() | Out-Null
			$NetworkIPConfiguration.EnableDHCP() | Out-Null
		}
	}

	sleep -Seconds 4
	
	$RecordsInStore.Clear() | Out-Null
	SetRecordsToStore($RecordsInStore)
	
	Write-Output "Device simulators stopped"
}

function  Kill-BoschVideoSimulator()
{
    <#    
        .SYNOPSIS
         Kill the device simulator instance running on a specific IP address.

        .DESCRIPTION
         The device simulator instance will be killed and IP address will be removed from network adapter.
        
        .PARAMETER $IPAddress
         The IPAddress on which device simulator is running

        .EXAMPLE 
          # Kill device simulator instance running on IP 192.168.1.5
          Kill-BoschVideoSimulator -IPAddress 192.168.1.5
    #>
    [CmdletBinding()]
    Param
    (
        [Parameter(Position = 0, Mandatory = $true)]
        [string]
        [ValidateScript({[bool]($_ -as [IPADDRESS])})]
        $IPAddress
    )

    # Kill process, remove added IP and record in persistent store  
    $RecordsInStore = GetRecordsFromStore    
    if ($RecordsInStore.Simulators -and $RecordsInStore.Simulators.ContainsKey($IPAddress))
    {
        try
        {
			if($RecordsInStore.Simulators.$IPAddress -ne $null)
			{
				Stop-Process -Id $RecordsInStore.Simulators.$IPAddress -Confirm:$false -ErrorAction Stop
			}
        }
        catch
        {
            Write-Host "Failed to stop device running on $IPAddress"
            return
        }

        RemoveIPFromNetworkCard -IP $IPAddress

        # Remove record in persistent store  
        $RecordsInStore = GetRecordsFromStore      
        $RecordsInStore.Simulators.Remove($IPAddress) | Out-Null

        if ((-not $RecordsInStore.SimulatorMode) -or $RecordsInStore.SimulatorMode.CompareTo("Execution") -eq 0)
        {
            # Delete configuration files in case of Execution mode
            $Pattern = "$IPAddress*.bin"
            Remove-Item $PSScriptRoot\$DefaultCfgPath\$Pattern -ErrorAction SilentlyContinue
        }

        Write-Host "Device running on $IPAddress is killed"
		
        if ($RecordsInStore.Simulators.Count -eq 0)
        {
			Write-Output "No more simulators running, resetting network interface"
			Stop-BoschVideoSimulator | Out-Null
		}
		else
		{
			SetRecordsToStore($RecordsInStore)
        }
        				
    }
    else
    {
        Write-Host "No device running on $IPAddress"
    }
}

function Set-PreConfigurationPath()
{	
    <#    
        .SYNOPSIS
         Set path of pre-configuration file

        .DESCRIPTION
         Set path of pre-configuration file
        
        .PARAMETER $Path
         The path where pre-configuration file for device simulator will be stored. 
         This must be a relative path to where this script and simulator executable are located.
        
        .EXAMPLE 
          # Store pre-configuration file in folder $PSScriptRoot\Preconfig 
          Set-PreConfigurationPath -Path PreConfig
    #>
    [CmdletBinding()]
    Param
    (
        [Parameter(Position = 0, Mandatory = $true)]
        [string]
        [ValidateScript({Test-Path "$PSScriptRoot\$_" -PathType Container})]
        $Path
    )

    $Script:PreConfigPath = $Path
}


function SetRecordsToStore()
{
	param([object]$records)
	
	if($records -and $records.Count -gt 0)
	{
		$records | ConvertTo-Json -depth 10 | Out-File $Script:PersistentStore
	}
	else
	{
		Remove-Item $Script:PersistentStore
	}
		
	
}

function GetRecordsFromStore()
{
    if (Test-Path $Script:PersistentStore -PathType Leaf)
	{
		$records = (Get-Content $Script:PersistentStore) -join "`n" | ConvertFrom-Json
		$records = ConvertJSONToHash($records)

#        Get-Content $Script:PersistentStore | ForEach-Object `
#        {
#            $Var = $_.Split(',')
#            try
#            {
#                $Records.Add($Var[0], $Var[1])
#            }
#            catch
#            {
#                $Records.Set_Item($Var[0], $Var[1])
#            }
        # }
     }
	 else
	 {
		$records = @{}
	 }

    $records
}

function ConvertJSONToHash
{
    param(
        $root
    )
    $hash = @{}

    $keys = $root | gm -MemberType NoteProperty | select -exp Name

    $keys | %{
        $key=$_
        $obj=$root.$($_)
        if($obj -match "@{")
        {
            $nesthash=ConvertJSONToHash $obj
            $hash.add($key,$nesthash)
        }
        else
        {
           $hash.add($key,$obj)
        }

    }
    return $hash
}

function GetNetworkIpConfiguration()
{
		$Adapter = Get-NetAdapter -ErrorAction Stop | ? {$_.Status -eq "up"} | Select-Object -first 1
		if (!$Adapter)
		{
			Write-Error "No available network connection. Please make sure you have WIFI or Ethernet connection."
		}
		$NetworkIPConfiguration = Get-WmiObject Win32_NetworkAdapterConfiguration -ComputerName $env:computername -ErrorAction Stop | ? {$_.IPEnabled} | Select-Object -first 1
		$NetworkIPConfiguration
}

function RemoveIPFromNetworkCard()
{
    # Remove IP address from network adapter.
    param([string]$IP)

    try
    {
        Get-NetIPAddress -IPAddress $IP -ErrorAction SilentlyContinue| Remove-NetIPAddress -Confirm:$false | Out-Null
        Write-Host "IP address $IP removed"
    }
    catch
    {
        Write-Host "Failed to remove IP address $IP"
    }
}

function NetmaskToPrefixLength()
{
    # Convert netmask to prefix length, e.g. 255.255.0.0 -> 16.
    param([string]$Netmask)

    $PrefixLength = 0

    if ([BOOL]($Netmask -as [IPADDRESS]))
    {
        $Netmask.split('.') | foreach `
        {
            $ByteValue = [Byte](0 + $_)
            $PrefixLength = $PrefixLength + (ConsecutiveBitCount -Value $ByteValue)
        }
    }
    else
    {
        Write-Warning "Input netmask is invalid."
    }

    $PrefixLength
}

function ConsecutiveBitCount()
{
    # Count how many consecutive bits are set in a byte from highest bit to lowest bit. 
    param([Byte]$Value)

    [Byte] $Mask = 128 # 1000 0000
    $Count = 0

    while ((($Value -band $Mask) -ne 0) -and ($Mask -ne 0))
    {
        $Count = $Count + 1
        $Mask = $Mask -shr 1
    }

    $Count
}
