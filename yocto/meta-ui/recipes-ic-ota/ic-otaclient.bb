SUMMARY = "Compile HeadUnit qt"
LICENSE = "CLOSED"

SRC_URI = "file://OTA_Handler"

S = "${WORKDIR}/OTA_Handler/handler_tcp_client"
B = "${WORKDIR}/build"

CXXFLAGS += "-I${STAGING_DIR_TARGET}/usr/include/CommonAPI-3.2"

DEPENDS += "cmake packagegroup-core-buildessential qtbase qtdeclarative qtwebengine qtmultimedia vsomeip common-api-c++ common-api-c++-someip pkgconfig openssl jq"

inherit cmake_qt5 cmake pkgconfig

do_install() {
    install -d ${D}/opt/OTA_Handler/json
    install -d ${D}/opt/OTA_Handler/boot_manager
    install -d ${D}/opt/OTA_Handler/handler_tcp_client
    install -d ${D}/opt/OTA_Handler/handler_tcp_client/update
    install -d ${D}${bindir}
    
    install -m 0755 ${B}/handler_tcp_client ${D}/opt/OTA_Handler/handler_tcp_client/handler_tcp_client
    install -m 0755 ${B}/libhandler_tcp_client-someip.so ${D}/opt/OTA_Handler/handler_tcp_client/libhandler_tcp_client-someip.so
    install -m 0755 ${S}/chunkStatusFile.txt ${D}/opt/OTA_Handler/handler_tcp_client/chunkStatusFile.txt
    install -m 0755 ${S}/../client.sh ${D}/opt/OTA_Handler/client
    install -m 0755 ${S}/../json/client.json ${D}/opt/OTA_Handler/json/client.json
    cp -r ${WORKDIR}/OTA_Handler/boot_manager/* ${D}/opt/OTA_Handler/boot_manager/
}

FILES:${PN} = "/opt/OTA_Handler/ \
               ${bindir}/ \
               "

RDEPENDS:${PN} += "qtwebengine python3-netifaces qtmultimedia python3-mutagen bash gtk+3 openssl jq python3-ecdsa"

EXTRA_OECMAKE += "-DCMAKE_SKIP_RPATH=TRUE"

