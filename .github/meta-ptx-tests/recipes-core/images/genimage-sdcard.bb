inherit genimage

LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI += "file://genimage.config"

DEPENDS += "genext2fs-native"

do_genimage[depends] += " \
    core-image-minimal:do_image_complete \
    virtual/bootloader:do_deploy \
"
