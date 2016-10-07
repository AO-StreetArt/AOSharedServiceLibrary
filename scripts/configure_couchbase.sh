#!/bin/bash

# Setup Administrator username and password
curl -v -X POST http://127.0.0.1:8091/settings/web -d 'password=password&username=Administrator&port=SAME'

# Setup Services
curl -u Administrator:password -v -X POST http://127.0.0.1:8091/node/controller/setupServices -d 'services=kv%2Cn1ql%2Cindex%2Cfts'

# Setup Bucket
curl -u Administrator:password -v -X POST http://127.0.0.1:8091/pools/default/buckets -d 'flushEnabled=1&threadsNumber=3&replicaIndex=0&replicaNumber=0&evictionPolicy=valueOnly&ramQuotaMB=597&bucketType=membase&name=default&authType=sasl&saslPassword='

# Setup Index RAM Quota
curl -u Administrator:password -X POST http://127.0.0.1:8091/pools/default -d 'memoryQuota=5000' -d 'indexMemoryQuota=269'
