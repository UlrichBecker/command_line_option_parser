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

SECTION          = "library"
S                = "${WORKDIR}/git"
LICENSE          = "MIT"
LIC_FILES_CHKSUM = "file://${S}/LICENSE;md5=4da5aa79bdb6df631c5f8ceb2c6a52cb"
SRC_URI          = "git://github.com/UlrichBecker/command_line_option_parser.git;branch=master"
SRCREV           = "${AUTOREV}"
PV               = "1.0-git${SRCPV}"
PR               = "r0"
HEADDER_DIR      = "/usr/include/"
LIB_DIR          = "/usr/lib/"

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile() {
   oe_runmake -C ${S}/src
}

do_install() {
   mkdir -p ${D}${HEADDER_DIR}
   mkdir -p ${D}${LIB_DIR}
   oe_runmake -C ${S}/src HEADDER_DIR=${D}${HEADDER_DIR} LIB_DIR=${D}${LIB_DIR} install
}

#=================================== EOF ======================================
