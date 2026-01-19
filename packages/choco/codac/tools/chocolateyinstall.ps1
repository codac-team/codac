$ErrorActionPreference = 'Stop'; # Stop on all errors.

# Source variables which are shared between install and uninstall.
. $PSScriptRoot\sharedVars.ps1

function Assert-SafeUrl {
	param(
		[string]$Url,
		[string]$AllowedPrefix
	)

	$isLocal    = Test-Path $Url -PathType Any -ErrorAction SilentlyContinue
	$isAllowed  = $Url.StartsWith($AllowedPrefix, [StringComparison]::OrdinalIgnoreCase)
	if (-not ($isLocal -or $isAllowed)) {
		throw "Invalid URL '$Url'. Must be a local path or start with '$AllowedPrefix'."
	}
}

$toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"

$pp = Get-PackageParameters
$packageDir = Join-Path "$toolsDir" ".." -Resolve
$installDir = Join-Path "$packageDir" ".." -Resolve
if ($pp.InstallDir -or $pp.InstallationPath) { 
	$installDir = $pp.InstallDir + $pp.InstallationPath 
}
Write-Host "Codac is going to be installed in '$installDir'"

$root = Join-Path $installDir "codac"
New-Item -ItemType Directory -Force -Path $root | Out-Null

$AllowedUrlPrefix = 'https://github.com/codac-team/codac'

if (!$pp['url']) { 
	$url = 'https://github.com/codac-team/codac/releases/download/v1/codac_x86_mingw13.zip'
	$checksum = 'EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE'
	$url64 = 'https://github.com/codac-team/codac/releases/download/v1/codac_x64_mingw13.zip'
	$checksum64 = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF'
	$packageArgs = @{
		packageName   = $env:ChocolateyPackageName
		unzipLocation = Join-Path "$root" ".."
		url           = $url
		url64bit      = $url64
		checksum      = $checksum
		checksumType  = 'sha256'
		checksum64    = $checksum64
		checksumType64= 'sha256'
	}
	Install-ChocolateyZipPackage @packageArgs

	$runtime = "mingw"
}
else {
	$url = $pp['url']
	$checksum = $pp['checksum']
	Assert-SafeUrl $url $AllowedUrlPrefix
	$packageArgs = @{
		packageName   = $env:ChocolateyPackageName
		unzipLocation = Join-Path "$root" ".."
		url           = $url
		url64bit      = $url
		checksum      = $checksum
		checksumType  = 'sha256'
		checksum64    = $checksum
		checksumType64= 'sha256'
	}
	Install-ChocolateyZipPackage @packageArgs

	# Analyze url to guess what to add to Windows PATH or registry...
	$arch = "x64"
	if ($url -match "x86") {
		$arch = "x86"
	}
	elseif ($url -match "arm64") {
		$arch = "arm64"
	}
	$runtime = 'mingw'
	if ($url -match 'vc(\d{1,2})') {
		$ver = [int]$matches[1]
		if ($ver -ge 8 -and $ver -le 22) {
			$runtime = "vc$ver"
		}
	}
}

$cmakepathtoadd = "$root\share\$CMakePackageName\cmake"
if ((!$pp['NoRegistry']) -and (Test-Path $cmakepathtoadd)) {
	New-Item "$CMakeSystemRepositoryPath\$CMakePackageName" -ItemType directory -Force
	New-ItemProperty -Name "$CMakePackageName$CMakePackageVer`_$arch" -PropertyType String -Value $cmakepathtoadd -Path "$CMakeSystemRepositoryPath\$CMakePackageName" -Force
}
$pathtoadd = "$root\bin"
if ((!$pp['NoPath']) -and (Test-Path $pathtoadd)) {
	Install-ChocolateyPath $pathtoadd -PathType 'Machine'
}

for ($i = 1; $i -le 99; $i++) {
	if ($pp['url'+$i]) {
		$url = $pp['url'+$i]
		$checksum = $pp['checksum'+$i]
		Assert-SafeUrl $url $AllowedUrlPrefix
		$packageArgs = @{
			packageName   = $env:ChocolateyPackageName
			unzipLocation = Join-Path "$root" ".."
			url           = $url
			url64bit      = $url
			checksum      = $checksum
			checksumType  = 'sha256'
			checksum64    = $checksum
			checksumType64= 'sha256'
		}
		Install-ChocolateyZipPackage @packageArgs

		# Analyze url to guess what to add to Windows PATH or registry...
		$arch = "x64"
		if ($url -match "x86") {
			$arch = "x86"
		}
		elseif ($url -match "arm64") {
			$arch = "arm64"
		}
		$runtime = 'mingw'
		if ($url -match 'vc(\d{1,2})') {
			$ver = [int]$matches[1]
			if ($ver -ge 8 -and $ver -le 22) {
				$runtime = "vc$ver"
			}
		}

		$cmakepathtoadd = "$root\share\$CMakePackageName\cmake"
		if ((!$pp['NoRegistry']) -and (Test-Path $cmakepathtoadd)) {
			New-Item "$CMakeSystemRepositoryPath\$CMakePackageName" -ItemType directory -Force
			New-ItemProperty -Name "$CMakePackageName$CMakePackageVer`_$arch" -PropertyType String -Value $cmakepathtoadd -Path "$CMakeSystemRepositoryPath\$CMakePackageName" -Force
		}
		$pathtoadd = "$root\bin"
		if ((!$pp['NoPath']) -and (Test-Path $pathtoadd)) {
			Install-ChocolateyPath $pathtoadd -PathType 'Machine'
		}
	}
}
