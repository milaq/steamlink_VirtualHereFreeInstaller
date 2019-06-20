#!/bin/bash
#

TOP=$(cd `dirname "${BASH_SOURCE[0]}"` && pwd)
if [ "${MARVELL_SDK_PATH}" = "" ]; then
	MARVELL_SDK_PATH="$(cd "${TOP}/../.." && pwd)"
fi
if [ "${MARVELL_ROOTFS}" = "" ]; then
	source "${MARVELL_SDK_PATH}/setenv.sh" || exit 1
fi
cd "${TOP}"

make $MAKE_J || exit 2

export DESTDIR="${PWD}/steamlink/apps/VirtualHereFreeInstaller"

# Copy the files to the app directory
mkdir -p "${DESTDIR}"
cp -v -r res/ "${DESTDIR}"
cp -v install.sh "${DESTDIR}"
cp -v vhf_installer_screen "${DESTDIR}"
cp -v icon.png "${DESTDIR}"

# Create the table of contents and icon
cat >"${DESTDIR}/toc.txt" <<__EOF__
name=VirtualHereFree Installer
icon=icon.png
run=install.sh
__EOF__

# Pack it up
name=$(basename ${DESTDIR})
pushd "$(dirname ${DESTDIR})"
tar zcvf $name.tgz $name || exit 3
rm -rf $name
popd

# All done!
echo "Build complete!"
echo
echo "Put the steamlink folder onto a USB drive, insert it into your Steam Link, and cycle the power to install."
