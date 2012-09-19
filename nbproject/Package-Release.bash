#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build
NBTMPDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tmp-packaging
TMPDIRNAME=tmp-packaging
OUTPUT_PATH=${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libconio.${CND_DLIB_EXT}
OUTPUT_BASENAME=libconio.${CND_DLIB_EXT}
PACKAGE_TOP_DIR=/usr/

# Functions
function checkReturnCode
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}
function makeDirectory
# $1 directory path
# $2 permission (optional)
{
    mkdir -p "$1"
    checkReturnCode
    if [ "$2" != "" ]
    then
      chmod $2 "$1"
      checkReturnCode
    fi
}
function copyFileToTmpDir
# $1 from-file path
# $2 to-file path
# $3 permission
{
    cp "$1" "$2"
    checkReturnCode
    if [ "$3" != "" ]
    then
        chmod $3 "$2"
        checkReturnCode
    fi
}

# Setup
cd "${TOP}"
mkdir -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
rm -rf ${NBTMPDIR}
mkdir -p ${NBTMPDIR}

# Copy files and create directories and links
cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/lib"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}lib/${OUTPUT_BASENAME}" 0644


# Ensure proper rpm build environment
RPMMACROS=~/.rpmmacros
NBTOPDIR=/tmp/cnd/rpms

if [ ! -f ${RPMMACROS} ]
then
    touch ${RPMMACROS}
fi

TOPDIR=`grep _topdir ${RPMMACROS}`
if [ "$TOPDIR" == "" ]
then
    echo "**********************************************************************************************************"
    echo Warning: rpm build environment updated:
    echo \"%_topdir ${NBTOPDIR}\" added to ${RPMMACROS}
    echo "**********************************************************************************************************"
    echo %_topdir ${NBTOPDIR} >> ${RPMMACROS}
fi  
mkdir -p ${NBTOPDIR}/RPMS

# Create spec file
cd "${TOP}"
SPEC_FILE=${NBTMPDIR}/../${OUTPUT_BASENAME}.spec
rm -f ${SPEC_FILE}

cd "${TOP}"
echo BuildRoot: ${TOP}/${NBTMPDIR} >> ${SPEC_FILE}
echo Summary: Sumary... >> ${SPEC_FILE}
echo Name: libconio.so >> ${SPEC_FILE}
echo Version: 1.0 >> ${SPEC_FILE}
echo Release: 1 >> ${SPEC_FILE}
echo Group: Applications/System >> ${SPEC_FILE}
echo License: BSD-type >> ${SPEC_FILE}
echo '%description' >> ${SPEC_FILE}
echo 'Description...' >> ${SPEC_FILE}
echo  >> ${SPEC_FILE}
echo '%files' >> ${SPEC_FILE}
echo \"/${PACKAGE_TOP_DIR}lib/${OUTPUT_BASENAME}\" >> ${SPEC_FILE}
echo '%dir' >> ${SPEC_FILE}

# Create RPM Package
cd "${TOP}"
LOG_FILE=${NBTMPDIR}/../${OUTPUT_BASENAME}.log
rpmbuild --buildroot ${TOP}/${NBTMPDIR}  -bb ${SPEC_FILE} > ${LOG_FILE}
makeDirectory "${NBTMPDIR}"
checkReturnCode
cat ${LOG_FILE}
RPM_PATH=`cat $LOG_FILE | grep '\.rpm' | tail -1 |awk -F: '{ print $2 }'`
RPM_NAME=`basename ${RPM_PATH}`
mv ${RPM_PATH} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
checkReturnCode
echo RPM: ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/${RPM_NAME}

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
