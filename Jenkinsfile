pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        bat(script: build_jenkins.bat, returnStatus: true)
      }
    }
  }
}