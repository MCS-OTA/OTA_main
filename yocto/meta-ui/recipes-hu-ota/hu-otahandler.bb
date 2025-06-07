SUMMARY = "Compile HeadUnit qt"
LICENSE = "CLOSED"

SRC_URI = "file://OTA_Handler"

S = "${WORKDIR}/OTA_Handler/handler_tcp_server_exter"
B = "${WORKDIR}/build"

CXXFLAGS += "-I${STAGING_DIR_TARGET}/usr/include/CommonAPI-3.2"

DEPENDS += "cmake packagegroup-core-buildessential qtbase qtdeclarative qtwebengine qtmultimedia vsomeip common-api-c++ common-api-c++-someip pkgconfig openssl jq"

inherit cmake_qt5 cmake pkgconfig

do_install() {
    install -d ${D}/opt/OTA_Handler/handler_tcp_server_exter
    install -d ${D}/opt/OTA_Handler/handler_tcp_server_exter/tmp
    install -d ${D}/opt/OTA_Handler/json
    install -d ${D}/opt/OTA_Handler/boot_manager
    install -d ${D}${bindir}
    
    install -m 0755 ${B}/handler_tcp_server_exter ${D}/opt/OTA_Handler/handler_tcp_server_exter/handler_tcp_server_exter
    install -m 0755 ${B}/libhandler_tcp_server_exter-someip.so ${D}/opt/OTA_Handler/handler_tcp_server_exter/libhandler_tcp_server_exter-someip.so
    install -m 0755 ${S}/../server_exter.sh ${D}/opt/OTA_Handler/server_exter
    install -m 0755 ${S}/../status.txt ${D}/opt/OTA_Handler/status.txt
    install -m 0755 ${S}/../json/server_exter.json ${D}/opt/OTA_Handler/json/server_exter.json
    cp -r ${WORKDIR}/OTA_Handler/boot_manager/* ${D}/opt/OTA_Handler/boot_manager/
}

FILES:${PN} = "/opt/OTA_Handler/ \
               ${bindir}/ \
               "

RDEPENDS:${PN} += "qtwebengine python3-netifaces qtmultimedia python3-mutagen bash gtk+3 openssl jq mosquitto python3-paho-mqtt python3-pyqt5 python3-ecdsa"

EXTRA_OECMAKE += "-DCMAKE_SKIP_RPATH=TRUE"

