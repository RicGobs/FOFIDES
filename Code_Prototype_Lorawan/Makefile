# name of your application
APPLICATION = FOFIDES - ForestFireDetectionSystem

# If no BOARD is found in the environment, use this default:
BOARD ?=  b-l072z-lrwan1

#CONTINUE_ON_EXPECTED_ERRORS=1


# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

#Lora OTA

DEVEUI ?= ....
APPEUI ?= ....
APPKEY ?= ........


# Default radio driver is Semtech SX1276 (used by the B-L072Z-LRWAN1 board)
DRIVER ?= sx1276

# Default region is Europe and default band is 868MHz
LORA_REGION ?= EU868

# Include the Semtech-loramac package
USEPKG += semtech-loramac

USEMODULE += $(DRIVER)
USEMODULE += fmt
FEATURES_OPTIONAL += periph_rtc

# Modules to include:
USEMODULE += ztimer
USEMODULE += ztimer_msec
USEMODULE += xtimer
USEMODULE += analog_util
USEMODULE += periph_gpio

FEATURES_REQUIRED += periph_adc

# Change this to 0 show compiler invocation lines by default:
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
