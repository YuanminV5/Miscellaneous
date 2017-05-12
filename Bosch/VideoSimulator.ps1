# <copyright file="VideoSimulator.ps1" company="Bosch Security Systems GmbH">
#   The information contained herein is confidential, proprietary to
#   Bosch Security Systems GmbH, and considered a trade secret.
#   Use of this information by anyone other than authorized employees of
#   Bosch Security Systems GmbH is granted only under a written non-disclosure
#   agreement, expressly prescribing the scope and manner of such use.
# </copyright>

if(Test-Path "$PSScriptRoot\VideoSimulator.psd1" -PathType Leaf)
{
	Import-Module "$PSScriptRoot\VideoSimulator.psd1" -DisableNameChecking
}
else
{
	Write-Error "Please make sure this script is in VideoSimulator folder."
}