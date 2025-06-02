#!/bin/bash

LD_LIBRARY_PATH=/home/ota/Documents/install_folder/lib:/home/ota/Documents/Instrument-Cluster/IC_someip/v0.0.0/IC_someip/build
COMMONAPI_CONFIG=/home/ota/Documents/Instrument-Cluster/commonapi4someip.ini
VSOMEIP_CONFIGURATION=/home/ota/Documents/Instrument-Cluster/json/IC.json
VSOMEIP_APPLICATION_NAME=IC-service

export LD_LIBRARY_PATH
export COMMONAPI_CONFIG
export VSOMEIP_CONFIGURATION
export VSOMEIP_APPLICATION_NAME

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
chmod +x "$DIR/IC_someip"
"$DIR/IC_someip"
