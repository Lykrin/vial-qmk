ENCODER_MAP_ENABLE = yes

# Vial setup
VIA_ENABLE = yes
VIAL_ENABLE = yes
VIALRGB_ENABLE = yes
ENCODER_MAP_ENABLE = yes
KEY_LOCK_ENABLE = yes
LTO_ENABLE = yes           # Enables Link Time Optimization
MOUSEKEY_ENABLE = yes      # Mouse control
EXTRAKEY_ENABLE = yes      # Audio control and system control
UNICODE_ENABLE = yes       # Unicode

# Saving space on atmega32u4
ifeq ($(strip $(CONVERT_TO)), )
    OLED_ENABLE = no
    MOUSEKEY_ENABLE = no
    COMBO_ENABLE = no
    KEY_OVERRIDE_ENABLE = no
endif

## build targets
# Liatris
CONVERT_TO = liatris
# promicro
# BOOTLOADER = caterina
