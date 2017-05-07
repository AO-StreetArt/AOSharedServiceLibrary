#!/bin/bash

# This script will copy a template from the examples folder into a new project

if [ "$#" -ne 2 ]; then
    echo "The correct usage of this script is './project_gen.sh template-name project-name'"
    exit 64
fi
TEMPLATE_NAME=$1
PROJECT_NAME=$2

printf "Generating AOSSL Project: $PROJECT_NAME - $TEMPLATE_NAME"

# Copy the template to a new folder
cp -r ../examples/$TEMPLATE_NAME ../../
mv ../../$TEMPLATE_NAME ../../$PROJECT_NAME

# Copy the Dockerfile for AOSSL into the new project
cp ../Dockerfile ../../$PROJECT_NAME

exit 0
