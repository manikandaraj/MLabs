#!/bin/bash

echo "Cleaning Up ..."

rm -rf war/gwtclient
rm -rf war/WEB-INF/deploy
rm -rf war/WEB-INF/classes
rm -rf gwt-unitCache
rm -rf build
rm -f gwtClient.war
