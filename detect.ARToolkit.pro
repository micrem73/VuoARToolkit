TEMPLATE = aux
cache()

NODE_SOURCES += \
        detect.ARToolkit.c

OTHER_FILES += $$NODE_SOURCES

VUO_FRAMEWORK_PATH = ../../../framework
ARTOOLKIT_PATH = ~/SDKs/ARToolKit5-bin-5.3.2r1-OSX
VUO_USER_MODULES_PATH = ~/Library/Application\ Support/Vuo/Modules


node.input = NODE_SOURCES
node.output = ${QMAKE_FILE_IN_BASE}.vuonode
node.commands = $${VUO_FRAMEWORK_PATH}/vuo-compile --header-search-path $${ARTOOLKIT_PATH}/include --output ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN} \
	&& mkdir -p "$${VUO_USER_MODULES_PATH}" \
	&& cp ${QMAKE_FILE_OUT} "$${VUO_USER_MODULES_PATH}"
QMAKE_EXTRA_COMPILERS += node

QMAKE_CLEAN = *.vuonode
