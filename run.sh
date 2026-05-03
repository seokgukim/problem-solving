#!/usr/bin/bash

set -euo pipefail

usage() {
  cat <<'EOF'
Usage: ./run.sh SOURCE_FILE [INPUT_FILE] [OUTPUT_FILE]

SOURCE_FILE is required.
INPUT_FILE defaults to stdin when omitted.
OUTPUT_FILE defaults to stdout when omitted.

Supported extensions:
  .c             -> gcc
  .cc/.cpp/.cxx  -> g++
  .rb            -> ruby
  .py            -> python3, fallback python
EOF
}

require_command() {
  local command_name="$1"
  if ! command -v "$command_name" >/dev/null 2>&1; then
    echo "Missing required command: $command_name" >&2
    exit 1
  fi
}

resolve_command() {
  local joined=""
  local command_name

  for command_name in "$@"; do
    if command -v "$command_name" >/dev/null 2>&1; then
      printf '%s\n' "$command_name"
      return 0
    fi

    if [[ -n "$joined" ]]; then
      joined+=" or "
    fi
    joined+="$command_name"
  done

  echo "Missing required command: $joined" >&2
  exit 1
}

cleanup() {
  if [[ -n "${executable_path:-}" ]]; then
    rm -f "$executable_path"
  fi
  if [[ -n "${tmp_dir:-}" ]]; then
    rm -rf "$tmp_dir"
  fi
}

if [[ $# -lt 1 || $# -gt 3 ]]; then
  usage >&2
  exit 1
fi

source_file="$1"
input_path="${2:-}"
output_path="${3:-}"

if [[ ! -f "$source_file" ]]; then
  echo "Source file not found: $source_file" >&2
  exit 1
fi

if [[ -n "$input_path" && ! -f "$input_path" ]]; then
  echo "Input file not found: $input_path" >&2
  exit 1
fi

extension="${source_file##*.}"
tmp_dir="$(mktemp -d)"
trap cleanup EXIT

declare -a command_to_run
case "$extension" in
  c)
    require_command gcc
    executable_path="$tmp_dir/program"
    gcc "$source_file" -O2 -o "$executable_path"
    command_to_run=("$executable_path")
    ;;
  cc|cpp|cxx)
    require_command g++
    executable_path="$tmp_dir/program"
    g++ "$source_file" -O2 -std=c++17 -o "$executable_path"
    command_to_run=("$executable_path")
    ;;
  rb)
    require_command ruby
    command_to_run=(ruby "$source_file")
    ;;
  py)
    python_command="$(resolve_command python3 python)"
    command_to_run=("$python_command" "$source_file")
    ;;
  *)
    echo "Unsupported source file extension: .$extension" >&2
    exit 1
    ;;
esac

if [[ -n "$input_path" && -n "$output_path" ]]; then
  "${command_to_run[@]}" <"$input_path" >"$output_path"
elif [[ -n "$input_path" ]]; then
  "${command_to_run[@]}" <"$input_path"
elif [[ -n "$output_path" ]]; then
  "${command_to_run[@]}" >"$output_path"
else
  # Preserve stdin exactly as provided, including blank lines, whether it comes
  # from an interactive terminal or a pipe.
  "${command_to_run[@]}"
fi
