#!/bin/bash

curl --request PUT -d five http://localhost:8500/v1/kv/test/prof/TEST_PROF_KONE
curl --request PUT -d six http://localhost:8500/v1/kv/test/prof/TEST_PROF_KTWO
export TEST_PROF_FIRST=seven
export TEST_PROF_SECOND=eight
