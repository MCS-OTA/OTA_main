Splash_package = "vlc \
		  playvideo \
		  systemd \
		  systemd-compat-units \
		  "

Qt_package = "qtdeclarative \
	      qtdeclarative-tools \
	      qt5-config \
	      qtbase \
	      qtbase-dev \
	      qtdeclarative-dev \
	      qtbase-tools \
	      qtbase-plugins \
	      qtquickcontrols2 \
	      qtquickcontrols2-mkspecs \
	      ${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'qtgraphicaleffects-qmlplugins', '', d)} \
	      qtwayland \
		  udisks2 \
		  ntpdate \
		  tzdata \
	      "
	            
Gstreamer_package = "gstreamer1.0 \
		     gstreamer1.0-plugins-base \
		     gstreamer1.0-plugins-good \
		     "
		     
Wayland_package = "wayland \
		   weston \
		   cmake \
		   make \
		   packagegroup-core-buildessential \
		   "

Someip_package = "vsomeip \ 
		  common-api-c++ \ 
		  common-api-c++-someip \
		  "

UI_package = "headunit \
              select \
              hu-otahandler \
              openssh \
              boost \
              "

Wifi_package = "wpa-supplicant \
               wifi-connect-autoexec \
               "
            
OTA_package = "openssl \
               "


IMAGE_INSTALL:append = " ${Splash_package} \
			${Gstreamer_package} \
			${Someip_package} \
			${UI_package} \
			${Qt_package} \
			${Wifi_package} \
			${Wayland_package} \
			"
      
