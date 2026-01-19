# Some of these variables might not be used in links to simplify parsing of files...
$procArch = $env:PROCESSOR_ARCHITECTURE
if ($env:ChocolateyForceX86 -eq $true) { $arch = "x86" }
elseif ($procArch -and $procArch.ToUpper() -eq 'ARM64') { $arch = "arm64" }
elseif (Get-ProcessorBits 32) { $arch = "x86" }
else { $arch = "x64" }
$MinGWMVer = "13"
$CMakeRegistryPath = "HKCU:\SOFTWARE\Kitware\CMake"
$CMakeSystemRepositoryPath = "HKLM:\SOFTWARE\Kitware\CMake\Packages"
$CMakePackageName = "Codac"
$CMakePackageVer = "1"
