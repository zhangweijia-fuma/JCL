#!/bin/bash
export THREAD_LIB_HOME=/home/user/threadlib
export JAVA_HOME=${THREAD_LIB_HOME}/jdk1.8.0_171
export JRE_HOME=${JAVA_HOME}/jre  
export CLASSPATH=.:${JAVA_HOME}/lib:${JRE_HOME}/lib  
export PATH=$PATH:${JAVA_HOME}/bin:${THREAD_LIB_HOME}/vlc-2.2.4/bin
export VLC_PLUGIN_PATH=${THREAD_LIB_HOME}/vlc-2.2.4/modules
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${THREAD_LIB_HOME}/vlc-qt-0.9/lib:${THREAD_LIB_HOME}/vlc-2.2.4/src/.libs:${THREAD_LIB_HOME}/vlc-2.2.4/lib/.libs

