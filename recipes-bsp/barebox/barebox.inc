SUMMARY = "Barebox - a bootloader that inherits the best of U-Boot and the Linux kernel"
HOMEPAGE = "http://www.barebox.org/"
SECTION = "bootloaders"
PROVIDES = "virtual/bootloader"

LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=057bf9e50e1ca857d0eb97bfe4ba8e5d"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit kernel-arch
inherit cml1

DEPENDS = "libusb1-native"

SRC_URI = " \
	http://barebox.org/download/barebox-${PV}.tar.bz2 \
	file://defconfig \
	"

S = "${WORKDIR}/barebox-${PV}"

EXTRA_OEMAKE = "CROSS_COMPILE=${TARGET_PREFIX}"

do_configure_prepend() {
	cp ${WORKDIR}/defconfig ${S}/.config

	# do not compile tools when using KERNEL_CC/KERNEL_LD
	echo CONFIG_BAREBOXENV_TARGET=n >> ${S}/.config
	echo CONFIG_BAREBOXCRC32_TARGET=n >> ${S}/.config
	echo CONFIG_KERNEL_INSTALL_TARGET=n >> ${S}/.config
	echo CONFIG_IMD_TARGET=n >> ${S}/.config
	oe_runmake oldconfig
}

do_compile () {
	# Barebox uses pkg-config only for building native tools
	export PKG_CONFIG_LIBDIR="${STAGING_DIR_NATIVE}${libdir}/pkgconfig:${STAGING_DIR_NATIVE}/usr/share/pkgconfig"
	export PKG_CONFIG_SYSROOT_DIR=

	unset LDFLAGS
	unset CFLAGS
	unset CPPFLAGS
	unset CXXFLAGS
	unset MACHINE

	oe_runmake CC="${KERNEL_CC}" LD="${KERNEL_LD}"
}

do_deploy () {
	install -d ${DEPLOY_DIR_IMAGE}
	if [ -e ${S}/barebox-flash-image ]; then
		install -m 644 -t ${DEPLOY_DIR_IMAGE}/ ${S}/barebox-flash-image
	fi
	for IMAGE in ${S}/images/*.img; do
		if [ -e ${IMAGE} ]; then
			install -m 644 -t ${DEPLOY_DIR_IMAGE}/ ${IMAGE}
		fi
	done
}

addtask deploy before do_build after do_compile