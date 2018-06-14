###############################################################################
##                                                                           ##
##            Yocto-recipe to building shared-library for the                ##
##                    command-line-option-parser                             ##
##                                                                           ##
##---------------------------------------------------------------------------##
## Author:   Ulrich Becker                                                   ##
## File:     clop-c.bb                                                       ##
## Date:     13.06.2018                                                      ##
## Revision:                                                                 ##
###############################################################################

SUMMARY          = "Shared-library of the command-line-option-parser for C/C++"

SECTION          = "libs"
S                = "${WORKDIR}/git"
LICENSE          = "MIT"
LIC_FILES_CHKSUM = "file://${S}/LICENSE;md5=4da5aa79bdb6df631c5f8ceb2c6a52cb"
SRC_URI          = "git://github.com/UlrichBecker/command_line_option_parser.git;branch=master"
SRCREV           = "${AUTOREV}"
PV               = "1.0-git${SRCPV}"
PR               = "r0"

TARGET_CC_ARCH   += "${LDFLAGS}"

FILES_${PN}      += "${libdir}/lib*.so.*"
FILES_${PN}-dev  += "${libdir}/lib*.so"
FILES_${PN}-dev  += "${includedir}/*.h"

do_compile() {
   oe_runmake -C ${S}/src
}

do_install() {
   mkdir -p ${D}${includedir}
   mkdir -p ${D}${libdir}
   oe_runmake -C ${S}/src HEADDER_DIR=${D}${includedir} LIB_DIR=${D}${libdir} install
}

#=================================== EOF ======================================
