#!/bin/bash

echo "############################################"
echo "# Smart Pump Firmware Project Setup Script #"
echo "############################################"
echo ""

echo "Check for command virtualenv"
which virtualenv
if [ $? -ne 0 ]; then
    echo "Command virtualenv not available. Abort"
	exit 1
fi

echo "Check for command pip"
which virtualenv
if [ $? -ne 0 ]; then
    echo "Command pip not available. Abort."
	exit 1
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
echo "Create virtual environment"
virtualenv -p $(which python2) $SCRIPT_DIR/venv
if [ $? -ne 0 ]; then
    echo "Creation of virtual environment '$SCRIPT_DIR/venv' failed. Abort."
	exit 1
fi

echo "Install dependencies from $SCRIPT_DIR/requirements.txt"
source $SCRIPT_DIR/venv/bin/activate 
pip install -r $SCRIPT_DIR/requirements.txt
if [ $? -ne 0 ]; then
    echo "Installation of dependencies failed. Abort."
	exit 1
fi

echo "Create platformio project"
PROJECT_DIR=$SCRIPT_DIR/firmware
# IDEs supported by platformio: atom|clion|codeblocks|eclipse|emacs|netbeans|qtcreator|sublimetext|vim|visualstudio|vscode
mkdir $PROJECT_DIR && platformio init -b nodemcu -d $PROJECT_DIR --ide clion
if [ $? -ne 0 ]; then
	echo "Creation of platformio project in directory $PROJECT_DIR failed. Abort."
	exit 1
fi
rm -rf $PROJECT_DIR/src && ln -s $SCRIPT_DIR/src $PROJECT_DIR/src


echo "Install project dependencies"
cd $PROJECT_DIR
platformio lib install ArduinoJson


echo "Initialization of platformio project in directory $PROJECT_DIR FINISHED."
