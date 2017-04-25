pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                sh 'mkdir ../aossl_deps'
                sh 'sudo cp scripts/deb/build_deps.sh ../aossl_deps'
                sh 'cd ../aossl_deps && sudo ./build_deps.sh'
                sh 'make'
            }
        }
        stage('Test') {
            steps {
                sh 'mkdir ../aossl_deps'
                sh 'sudo docker pull consul'
  - sudo docker run -d --name=registry --network=dvs consul
  - sudo docker pull neo4j
  - sudo docker run -d --publish=7474:7474 --publish=7687:7687 --env=NEO4J_AUTH=none --volume=$HOME/neo4j/data:/data --network=dvs --name=database neo4j
  - sudo docker pull redis
  - sudo docker run --network=dvs --name=cache -d redis
  - sudo docker pull mongo
  - sudo docker run -d --name database mongo
            }
        }
    }
}
