LD_LIBRARY_PATH=/usr/lib:/opt/OTA_Handler/build/
COMMONAPI_CONFIG=/opt/IC_someip/commonapi4someip.ini
VSOMEIP_CONFIGURATION=/opt/OTA_Handler/json/client.json
VSOMEIP_APPLICATION_NAME=ota-client

export LD_LIBRARY_PATH
export COMMONAPI_CONFIG
export VSOMEIP_CONFIGURATION
export VSOMEIP_APPLICATION_NAME

/opt/OTA_Handler/build/handler_tcp_client
