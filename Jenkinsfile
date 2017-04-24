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
            }
        }
    }
}
