#!/bin/bash

curl --request PUT -d five http://localhost:8500/v1/kv/test/prof/KONE
curl --request PUT -d six http://localhost:8500/v1/kv/test/prof/KTWO
export FIRST=seven
export SECOND=eight
