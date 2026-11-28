# =============================================================================
# build.ps1 - one-command build helper for the OpenCV C++ quickstart project
#
#   .\build.ps1                     # build everything (Release)
#   .\build.ps1 01_image_io         # build a single target
#   .\build.ps1 01_image_io -Run    # build then run it (working dir = project root)
#   .\build.ps1 -Config Debug       # build the Debug configuration
#   .\build.ps1 -List               # list targets (derived from src/*.cpp)
#   .\build.ps1 -Reconfigure        # force a clean cmake configure
#   .\build.ps1 -SkipConfigure      # skip the pre-build configure (faster, see note)
#
# Note: a configure pass runs before every build (it takes ~0.2s once cached).
# That is required because the target list comes from a directory glob: when you
# add or delete src/*.cpp, the build system only learns about it during the
# configure step, not during an already-running build.
#
# Messages are intentionally ASCII-only: Windows PowerShell 5.1 reads .ps1 files
# without a BOM using the ANSI code page, which would mangle Chinese text.
# =============================================================================
param(
    [Parameter(Position = 0)]
    [string]$Target = '',

    [ValidateSet('Release', 'Debug', 'RelWithDebInfo')]
    [string]$Config = 'Release',

    [switch]$Run,
    [switch]$Reconfigure,
    [switch]$SkipConfigure,
    [switch]$List,

    # Everything after the target name is forwarded to the program being run.
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$ProgramArgs = @()
)

$ErrorActionPreference = 'Stop'

$Root      = $PSScriptRoot
$BuildDir  = Join-Path $Root 'build'
$OpenCvDir = 'E:/opencv/cmake/opencv4'
$Generator = 'Visual Studio 16 2019'

function Find-CMake {
    $cmd = Get-Command cmake -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    $fallback = 'E:\CMake\cmake-3.31.12-windows-x86_64\bin\cmake.exe'
    if (Test-Path -LiteralPath $fallback) { return $fallback }
    throw 'cmake not found. Add CMake to PATH, or install it and retry.'
}

# Targets are exactly the .cpp files in src/ (one executable per file).
function Get-Targets {
    Get-ChildItem -LiteralPath (Join-Path $Root 'src') -Filter *.cpp -ErrorAction SilentlyContinue |
        ForEach-Object { $_.BaseName } | Sort-Object
}

$cmake = Find-CMake

if ($List) {
    $targets = Get-Targets
    if (-not $targets) {
        Write-Host 'No .cpp files in src/ yet - create one, then run .\build.ps1' -ForegroundColor Yellow
    } else {
        Write-Host 'Available targets (from src/*.cpp):'
        $targets | ForEach-Object { Write-Host "  $_" }
    }
    exit 0
}

# --- configure ---------------------------------------------------------------
$cacheFile = Join-Path $BuildDir 'CMakeCache.txt'
if ($SkipConfigure -and -not $Reconfigure -and (Test-Path -LiteralPath $cacheFile)) {
    Write-Host '[configure] skipped (-SkipConfigure)' -ForegroundColor DarkGray
} else {
    Write-Host "[configure] $Generator x64, OpenCV_DIR=$OpenCvDir" -ForegroundColor Cyan
    & $cmake -S $Root -B $BuildDir -G $Generator -A x64 "-DOpenCV_DIR=$OpenCvDir"
    if ($LASTEXITCODE -ne 0) { throw "cmake configure failed (exit $LASTEXITCODE)" }
}

# Drop generated projects left behind by src files that no longer exist.
$known = Get-Targets
Get-ChildItem -LiteralPath $BuildDir -Filter *.vcxproj -ErrorAction SilentlyContinue |
    Where-Object { $known -notcontains $_.BaseName -and $_.BaseName -notin @('ALL_BUILD', 'ZERO_CHECK') } |
    ForEach-Object {
        Write-Host "[clean] removing stale project $($_.Name)" -ForegroundColor DarkYellow
        Remove-Item -LiteralPath $_.FullName -Force -ErrorAction SilentlyContinue
    }

# --- build -------------------------------------------------------------------
$buildArgs = @('--build', $BuildDir, '--config', $Config, '--parallel', '8')
if ($Target) { $buildArgs += @('--target', $Target) }

Write-Host "[build] $($buildArgs -join ' ')" -ForegroundColor Cyan
& $cmake @buildArgs
$code = $LASTEXITCODE

if ($code -ne 0) {
    Write-Host ''
    Write-Host "Build failed (exit $code)." -ForegroundColor Red
    if ($Target -and ($known -notcontains $Target)) {
        Write-Host "No src/$Target.cpp exists, so target '$Target' does not exist. Available targets:" -ForegroundColor Yellow
        $known | ForEach-Object { Write-Host "  $_" }
        Write-Host ''
        Write-Host 'Target names come from file names and use underscores, e.g. 00_first_program (not 00_first-program).'
    } elseif ($Target) {
        Write-Host "Target '$Target' exists: the failure is in your code - read the compiler output above."
    }
    exit $code
}

# --- run ---------------------------------------------------------------------
if ($Run) {
    if (-not $Target) { throw '-Run needs a target, e.g. .\build.ps1 08_calib3d_pnp -Run' }
    $exe = Join-Path $BuildDir "$Config\$Target.exe"
    if (-not (Test-Path -LiteralPath $exe)) { throw "executable not found: $exe" }
    Write-Host "[run] $exe" -ForegroundColor Cyan

    # The source is compiled as UTF-8 (/utf-8); make the console decode stdout as UTF-8 too.
    $previousConsoleEncoding = [Console]::OutputEncoding
    $previousPipelineEncoding = $OutputEncoding
    $utf8ConsoleEncoding = New-Object System.Text.UTF8Encoding($false)
    try {
        [Console]::OutputEncoding = $utf8ConsoleEncoding
        $OutputEncoding = $utf8ConsoleEncoding
        Push-Location $Root  # programs write to output/ relative to the working dir
        try {
            if ($ProgramArgs.Count -gt 0) {
                Write-Host "[args] $($ProgramArgs -join ' ')" -ForegroundColor DarkGray
                & $exe @ProgramArgs
            } else {
                & $exe
            }
            $runCode = $LASTEXITCODE
        } finally {
            Pop-Location
        }
    } finally {
        [Console]::OutputEncoding = $previousConsoleEncoding
        $OutputEncoding = $previousPipelineEncoding
    }
    exit $runCode
}

Write-Host "[done] $Config binaries in build\$Config\" -ForegroundColor Green
