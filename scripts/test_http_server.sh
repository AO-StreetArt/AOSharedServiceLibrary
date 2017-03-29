#!/bin/bash

#This script will use CURL to test the http test server
curl http://127.0.0.1:12345
curl http://127.0.0.1:12345/test
curl -X POST -d "test" http://127.0.0.1:12345
