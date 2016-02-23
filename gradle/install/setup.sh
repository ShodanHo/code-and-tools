http://gradle.org/gradle-download/
https://services.gradle.org/distributions/gradle-2.11-bin.zip

cd ~/projects
unzip gradle-2.11-bin.zip
cd gradle-2.11/

# add GRADLE_HOME and path to ${GRADLE_HOME}/bin
# http://apple.stackexchange.com/questions/12629/how-can-i-add-a-path-to-the-path-environment-variable
echo "export GRADLE_HOME=${PWD}" >> ~/.bashrc
echo "export PATH=${GRADLE_HOME}/bin:$PATH" >> ~/.bashrc

# add daemon
# https://docs.gradle.org/current/userguide/gradle_daemon.html
#
mkdir ~/.gradle
echo "org.gradle.daemon=true" >> ~/.gradle/gradle.properties
