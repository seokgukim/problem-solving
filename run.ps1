#!/usr/bin/env pwsh

$ErrorActionPreference = 'Stop'

function Show-Usage {
    Write-Error @'
Usage: ./run.ps1 SOURCE_FILE [INPUT_FILE] [OUTPUT_FILE]

SOURCE_FILE is required.
INPUT_FILE defaults to stdin when omitted.
OUTPUT_FILE defaults to stdout when omitted.

Supported extensions:
  .c             -> gcc
  .cc/.cpp/.cxx  -> g++
  .rb            -> ruby
  .py            -> python3, fallback python
'@
}

function Resolve-CommandPath {
    param(
        [Parameter(Mandatory = $true)]
        [string[]]$CommandNames
    )

    foreach ($commandName in $CommandNames) {
        $command = Get-Command $commandName -CommandType Application -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($command) {
            return $command.Source
        }
    }

    throw "Missing required command: $($CommandNames -join ' or ')"
}

function Cleanup {
    if ($script:ExecutablePath -and (Test-Path -LiteralPath $script:ExecutablePath)) {
        Remove-Item -LiteralPath $script:ExecutablePath -Force
    }

    if ($script:TempDir -and (Test-Path -LiteralPath $script:TempDir)) {
        Remove-Item -LiteralPath $script:TempDir -Recurse -Force
    }
}

function Invoke-Program {
    param(
        [Parameter(Mandatory = $true)]
        [string]$CommandPath,

        [Parameter()]
        [string[]]$CommandArgs = @(),

        [Parameter()]
        [string]$InputPath = '',

        [Parameter()]
        [string]$OutputPath = ''
    )

    $redirectInput = ($InputPath -ne '') -or [Console]::IsInputRedirected
    $redirectOutput = ($OutputPath -ne '')

    $processStartInfo = [System.Diagnostics.ProcessStartInfo]::new()
    $processStartInfo.FileName = $CommandPath
    $processStartInfo.UseShellExecute = $false
    $processStartInfo.RedirectStandardInput = $redirectInput
    $processStartInfo.RedirectStandardOutput = $redirectOutput

    foreach ($argument in $CommandArgs) {
        [void]$processStartInfo.ArgumentList.Add($argument)
    }

    $process = [System.Diagnostics.Process]::new()
    $process.StartInfo = $processStartInfo

    $sourceStream = $null
    $destinationStream = $null
    $inputTask = $null
    $outputTask = $null

    try {
        [void]$process.Start()

        if ($redirectInput) {
            if ($InputPath) {
                $sourceStream = [System.IO.File]::OpenRead($InputPath)
            } else {
                $sourceStream = [Console]::OpenStandardInput()
            }

            $inputTask = $sourceStream.CopyToAsync($process.StandardInput.BaseStream)
        }

        if ($redirectOutput) {
            $destinationStream = [System.IO.File]::Open(
                $OutputPath,
                [System.IO.FileMode]::Create,
                [System.IO.FileAccess]::Write,
                [System.IO.FileShare]::Read
            )

            $outputTask = $process.StandardOutput.BaseStream.CopyToAsync($destinationStream)
        }

        if ($inputTask) {
            $null = $inputTask.GetAwaiter().GetResult()
            $process.StandardInput.Close()
        }

        if ($outputTask) {
            $null = $outputTask.GetAwaiter().GetResult()
        }

        $process.WaitForExit()
        if ($process.ExitCode -ne 0) {
            exit $process.ExitCode
        }
    } finally {
        if ($sourceStream) {
            $sourceStream.Dispose()
        }

        if ($destinationStream) {
            $destinationStream.Dispose()
        }

        $process.Dispose()
    }
}

if ($args.Count -lt 1 -or $args.Count -gt 3) {
    Show-Usage
    exit 1
}

$sourceFile = $args[0]
$inputPath = if ($args.Count -ge 2) { $args[1] } else { '' }
$outputPath = if ($args.Count -ge 3) { $args[2] } else { '' }

if (-not (Test-Path -LiteralPath $sourceFile -PathType Leaf)) {
    throw "Source file not found: $sourceFile"
}

if ($inputPath -and -not (Test-Path -LiteralPath $inputPath -PathType Leaf)) {
    throw "Input file not found: $inputPath"
}

$script:TempDir = Join-Path ([System.IO.Path]::GetTempPath()) ([System.IO.Path]::GetRandomFileName())
$null = New-Item -ItemType Directory -Path $script:TempDir
$script:ExecutablePath = $null

try {
    $extension = [System.IO.Path]::GetExtension($sourceFile).ToLowerInvariant()
    $gccPath = $null
    $gppPath = $null
    $rubyPath = $null
    $pythonPath = $null

    switch ($extension) {
        '.c' {
            $gccPath = Resolve-CommandPath @('gcc')
            $script:ExecutablePath = Join-Path $script:TempDir 'program.exe'
            & $gccPath $sourceFile '-O2' '-o' $script:ExecutablePath
            if ($LASTEXITCODE -ne 0) {
                exit $LASTEXITCODE
            }
            Invoke-Program -CommandPath $script:ExecutablePath -InputPath $inputPath -OutputPath $outputPath
        }
        '.cc' {
            $gppPath = Resolve-CommandPath @('g++')
            $script:ExecutablePath = Join-Path $script:TempDir 'program.exe'
            & $gppPath $sourceFile '-O2' '-std=c++17' '-o' $script:ExecutablePath
            if ($LASTEXITCODE -ne 0) {
                exit $LASTEXITCODE
            }
            Invoke-Program -CommandPath $script:ExecutablePath -InputPath $inputPath -OutputPath $outputPath
        }
        '.cpp' {
            $gppPath = Resolve-CommandPath @('g++')
            $script:ExecutablePath = Join-Path $script:TempDir 'program.exe'
            & $gppPath $sourceFile '-O2' '-std=c++17' '-o' $script:ExecutablePath
            if ($LASTEXITCODE -ne 0) {
                exit $LASTEXITCODE
            }
            Invoke-Program -CommandPath $script:ExecutablePath -InputPath $inputPath -OutputPath $outputPath
        }
        '.cxx' {
            $gppPath = Resolve-CommandPath @('g++')
            $script:ExecutablePath = Join-Path $script:TempDir 'program.exe'
            & $gppPath $sourceFile '-O2' '-std=c++17' '-o' $script:ExecutablePath
            if ($LASTEXITCODE -ne 0) {
                exit $LASTEXITCODE
            }
            Invoke-Program -CommandPath $script:ExecutablePath -InputPath $inputPath -OutputPath $outputPath
        }
        '.rb' {
            $rubyPath = Resolve-CommandPath @('ruby')
            Invoke-Program -CommandPath $rubyPath -CommandArgs @($sourceFile) -InputPath $inputPath -OutputPath $outputPath
        }
        '.py' {
            $pythonPath = Resolve-CommandPath @('python3', 'python')
            Invoke-Program -CommandPath $pythonPath -CommandArgs @($sourceFile) -InputPath $inputPath -OutputPath $outputPath
        }
        default {
            throw "Unsupported source file extension: $extension"
        }
    }
} finally {
    Cleanup
}
