idf_build_get_property(idf_target IDF_TARGET)
if("${idf_target}" STREQUAL "esp32p4")
list(APPEND extra_requires esp-hosted-tanmatsu)
endif()

idf_component_register(
	SRCS
		"main.c"
		"common/display.c"
		"common/theme.c"
		"icons.c"
		"message_dialog.c"
		"textedit.c"
		"util_ssh.c"
		"settings_ssh.c"
		"menu_ssh.c"
		"menu_ssh_edit.c"
		#"terminal.c"
		#"chakrapetchmedium.c"
		#"sdcard.c"
		#"device_settings.c"
		#"device_information.c"
		#"filesystem_utils.c"
		#"ntp.c"

	PRIV_REQUIRES
		# from tanmatsu-template
		esp_lcd
		fatfs
		nvs_flash
		bt
		badge-bsp
		tanmatsu-wifi
		${extra_requires}
		# required for our app
		lwip
		robotman2412__pax-gfx
		robotman2412__pax-codecs
		nicolaielectronics__wifi-manager
		skuodi__libssh2_esp
		badgeteam__terminal-emulator
		gui
		# maybe no longer required?
		#wpa_supplicant
		#esp_https_ota
		#app_update
		#esp_bootloader_format
		#custom-certificates
		#wifi-manager
		#esp-tls
		#pax-gfx
		#pax-codecs
		#efuse
		#timezone
		#json
		#freertos
	INCLUDE_DIRS
		"."
	EMBED_FILES
		${files}
		"icons/esc.png"
		"icons/f1.png"
		"icons/f2.png"
		"icons/f3.png"
		"icons/f4.png"
		"icons/f5.png"
		"icons/f6.png"
		"icons/error.png"
		"icons/help.png"
		"icons/extension.png"
		"icons/terminal.png"
)

idf_build_set_property(COMPILE_OPTIONS "-Wno-error=unused-variable" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-error=unused-const-variable" APPEND)

