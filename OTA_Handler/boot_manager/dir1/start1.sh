#!/bin/bash


DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
chmod +x "$DIR/green_window"
"$DIR/green_window"
