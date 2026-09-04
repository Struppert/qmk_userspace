ENCODER_MAP_ENABLE = yes

# Vendor-Bug im Fork (~/keychron-qmk, Branch 2025q3): quantum/rgb_matrix/
# animations/pixel_rain_anim.h setzt region_mask, nutzt es aber nie -
# mit -Werror bricht das den Build. Gleicher Fix wie bei v1_max/BLUETOOTH.md
# dokumentiert (dort lkbt51.c betroffen, hier rgb_matrix core).
CFLAGS += -Wno-error=unused-but-set-variable
