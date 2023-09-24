FROM ubuntu:latest

ENV LD_LIBRARY_PATH=/usr/local/lib

RUN \
  apt-get update && \
  apt-get install -y git cmake g++-8 qtbase5-dev qt5-default qt5-qmake libcairo2-dev openjdk-8-jdk

RUN mkdir grame

WORKDIR /grame
RUN git clone --single-branch -b dev --depth 1 https://github.com/grame-cncm/guidoar.git
WORKDIR /grame/guidoar/build
RUN make 
RUN make install

WORKDIR /grame
RUN git clone --single-branch -b dev --depth 1  --recurse-submodules https://github.com/grame-cncm/guidolib.git
WORKDIR /grame/guidolib/build
RUN make basic 
RUN make Qt  && make install

WORKDIR /grame
