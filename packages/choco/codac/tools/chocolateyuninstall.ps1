$ErrorActionPreference = 'Stop'; # Stop on all errors.

# Source variables which are shared between install and uninstall.
. $PSScriptRoot\sharedVars.ps1

$toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"

$pp = Get-PackageParameters
$packageDir = Join-Path "$toolsDir" ".." -Resolve
$installDir = Join-Path "$packageDir" ".." -Resolve
if ($pp.InstallDir -or $pp.InstallationPath) { 
	$installDir = $pp.InstallDir + $pp.InstallationPath 
}
Write-Host "Codac is going to be uninstalled from '$installDir'"

$root = Join-Path $installDir "codac"

if (Get-Command Uninstall-ChocolateyPath -ErrorAction SilentlyContinue -CommandType Function) {
    Uninstall-ChocolateyPath "$root\bin" -PathType 'Machine'
}
else {
    $newpath = [environment]::GetEnvironmentVariable("Path","Machine")
    $newpath = ($newpath.Split(';') | Where-Object { $_ -ne "$root\bin" }) -join ';'
    [environment]::SetEnvironmentVariable("Path",$newpath,"Machine")
}

Remove-ItemProperty -Path $CMakeSystemRepositoryPath\$CMakePackageName -Name "$CMakePackageName$CMakePackageVer`_$arch" -ErrorAction SilentlyContinue

if (Test-Path $root) {
    if ((Resolve-Path $root).Path -notcontains (Resolve-Path $packageDir).Path) {
        Remove-Item -Recurse -Force $root
    }
}
