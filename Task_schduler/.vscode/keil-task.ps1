[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet('build', 'rebuild', 'flash')]
    [string]$Action
)

$ErrorActionPreference = 'Stop'

$workspaceRoot = Split-Path -Parent $PSScriptRoot
$mdkDirectory = Join-Path $workspaceRoot 'MDK-ARM'
$projectPath = Join-Path $mdkDirectory 'Task_schduler.uvprojx'

$uv4Candidates = @()
if (-not [string]::IsNullOrWhiteSpace($env:KEIL_UV4)) {
    $uv4Candidates += $env:KEIL_UV4
}
$uv4Candidates += @(
    (Join-Path $env:LOCALAPPDATA 'Keil_v5\UV4\UV4.exe'),
    'D:\keil5mdk\Keil_v5\UV4\UV4.exe',
    'C:\Keil_v5\UV4\UV4.exe'
)

$uv4Path = $uv4Candidates |
    Where-Object { Test-Path -LiteralPath $_ -PathType Leaf } |
    Select-Object -First 1

if (-not $uv4Path) {
    Write-Error 'UV4.exe was not found. Set KEIL_UV4 to the full path of UV4.exe.'
    exit 2
}

$commandSwitch = switch ($Action) {
    'build' { '-b' }
    'rebuild' { '-r' }
    'flash' { '-f' }
}

$logPath = Join-Path ([System.IO.Path]::GetTempPath()) "keil-$PID-$Action.log"
$arguments = @(
    $commandSwitch,
    ('"' + $projectPath + '"'),
    '-j0',
    '-o',
    ('"' + $logPath + '"')
)

$taskExitCode = 0
try {
    $startProcessParameters = @{
        FilePath = $uv4Path
        ArgumentList = $arguments
        WorkingDirectory = $mdkDirectory
        PassThru = $true
        Wait = $true
    }
    $process = Start-Process @startProcessParameters

    if (-not (Test-Path -LiteralPath $logPath -PathType Leaf)) {
        Write-Error "Keil did not create a log file: $logPath"
        $taskExitCode = 1
    }
    else {
        $logText = Get-Content -Raw -LiteralPath $logPath
        Write-Output $logText

        if ($process.ExitCode -ne 0) {
            $taskExitCode = $process.ExitCode
        }
        elseif ($Action -eq 'flash') {
            if ($logText -notmatch 'Flash Load finished' -or
                $logText -match '(?im)^\s*(Error:|No ST-LINK|Target DLL has been cancelled)') {
                $taskExitCode = 1
            }
        }
        elseif ($logText -notmatch '(?m)"[^"]+"\s+-\s+0 Error\(s\)') {
            $taskExitCode = 1
        }
    }
}
finally {
    if (Test-Path -LiteralPath $logPath -PathType Leaf) {
        Remove-Item -LiteralPath $logPath -Force
    }
}

exit $taskExitCode
