pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                sh 'mkdir ../aossl_deps'
                sh 'sudo cp scripts/deb/build_deps.sh ../aossl_deps'
                sh 'cd ../aossl_deps && sudo ./build_deps.sh'
                sh 'sudo docker pull consul'
                sh 'sudo docker run -d --name=registry --network=dvs consul'
                sh 'sudo docker pull neo4j'
                sh 'sudo docker run -d --publish=7474:7474 --publish=7687:7687 --env=NEO4J_AUTH=none --volume=$HOME/neo4j/data:/data --network=dvs --name=graph-db neo4j'
                sh 'sudo docker pull redis'
                sh 'sudo docker run --network=dvs --name=cache -d redis'
                sh 'sudo docker pull mongo'
                sh 'sudo docker run -d --name --network=dvs document-db mongo'
                sh 'make && make rhel-test'
            }
        }
        stage('Test') {
            steps {
                sh 'cd $TRAVIS_BUILD_DIR/aossl/http/server && ./http_server_test &'
                sh 'cd $TRAVIS_BUILD_DIR/aossl/properties && ./properties_reader_test'
                sh 'cd $TRAVIS_BUILD_DIR/aossl/commandline && ./cli_test name=test'
                sh 'cd $TRAVIS_BUILD_DIR/aossl/http/client && ./http_test'
                sh 'cd $TRAVIS_BUILD_DIR/aossl/logging && ./logging_test'
                sh 'cd $TRAVIS_BUILD_DIR/aossl/uuid && ./uuid_test'
                sh 'cd $TRAVIS_BUILD_DIR/aossl/zmq && ./zmqio_test'
                sh 'cd $TRAVIS_BUILD_DIR/aossl/redis && ./redis_test'
                sh 'cd $TRAVIS_BUILD_DIR/aossl/mongo && ./mongo_test'
                sh 'cd $TRAVIS_BUILD_DIR/scripts && sudo ./test_http_server.sh'
                sh 'cd $TRAVIS_BUILD_DIR/aossl/consul && ./consul_test'
                sh 'cd $TRAVIS_BUILD_DIR/aossl/neo4j && ./neo4j_test''
        }
    }
}
