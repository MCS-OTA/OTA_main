SUMMARY = "Compile HeadUnit qt"
LICENSE = "CLOSED"

SRC_URI = "file://OTA_Handler \
           file://OTA_Prime_ECU"

S = "${WORKDIR}/OTA_Handler/handler_tcp_server"
B = "${WORKDIR}/build"

CXXFLAGS += "-I${STAGING_DIR_TARGET}/usr/include/CommonAPI-3.2"

DEPENDS += "cmake packagegroup-core-buildessential qtbase qtdeclarative qtwebengine qtmultimedia vsomeip common-api-c++ common-api-c++-someip pkgconfig openssl jq"

inherit cmake_qt5 cmake pkgconfig

do_install() {
    install -d ${D}/opt/OTA_Handler/json
    install -d ${D}/opt/OTA_Handler/boot_manager
    install -d ${D}/opt/OTA_Handler/handler_tcp_server
    install -d ${D}/opt/OTA_Handler/handler_tcp_server/tmp
    install -d ${D}/opt/OTA_Prime_ECU
    install -d ${D}${bindir}
    
    install -m 0755 ${B}/handler_tcp_server ${D}/opt/OTA_Handler/handler_tcp_server/handler_tcp_server
    install -m 0755 ${B}/libhandler_tcp_server-someip.so ${D}/opt/OTA_Handler/handler_tcp_server/libhandler_tcp_server-someip.so
    install -m 0755 ${S}/../status.txt ${D}/opt/OTA_Handler/status.txt
    install -m 0755 ${S}/../server.sh ${D}/opt/OTA_Handler/server
    install -m 0755 ${S}/../json/server.json ${D}/opt/OTA_Handler/json/server.json
    cp -r ${WORKDIR}/OTA_Prime_ECU/* ${D}/opt/OTA_Prime_ECU/
    cp -r ${WORKDIR}/OTA_Handler/boot_manager/* ${D}/opt/OTA_Handler/boot_manager/
}

FILES:${PN} = "/opt/OTA_Handler/ \
               /opt/OTA_Prime_ECU/ \
               ${bindir}/"

RDEPENDS:${PN} += "qtwebengine python3-netifaces qtmultimedia python3-mutagen bash gtk+3 openssl jq mosquitto python3-paho-mqtt python3-pyqt5 python3-ecdsa python3-requests"

EXTRA_OECMAKE += "-DCMAKE_SKIP_RPATH=TRUE"

