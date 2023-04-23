# name of your application
APPLICATION = fire-alarm

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

#timer
USEMODULE += xtimer

#analog input
USEMODULE += analog_util

#RIOT device driver for the DHT11 digital sensor
USEMODULE += dht

#FMT module convert values from the sensor as 16-bit fixed 
#point numbers into a decimal string
USEMODULE += fmt

USEMODULE += periph_gpio

FEATURES_REQUIRED += periph_adc

include $(RIOTBASE)/Makefile.include
