APPLICATION = FOFIDES - ForestFireDetectionSystem

BOARD ?=  b-l072z-lrwan1

RIOTBASE ?= $(CURDIR)/../..

CFLAGS += -DDEVELHELP

#Lora OTA
DEVEUI ?= ....
APPEUI ?= ....
APPKEY ?= ........

DRIVER ?= sx1276
LORA_REGION ?= EU868

USEPKG += semtech-loramac

USEMODULE += $(DRIVER)
USEMODULE += fmt
FEATURES_OPTIONAL += periph_rtc

USEMODULE += ztimer
USEMODULE += ztimer_msec
USEMODULE += xtimer
USEMODULE += analog_util
USEMODULE += periph_gpio

FEATURES_REQUIRED += periph_adc

QUIET ?= 1

# Modules to include:
USEMODULE += xtimer

ifndef CONFIG_KCONFIG_USEMODULE_LORAWAN
  # OTAA compile time configuration keys
  CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
  CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
  CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"
endif

include $(RIOTBASE)/Makefile.include
