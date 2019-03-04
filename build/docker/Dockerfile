FROM ubuntu:bionic

ENV LD_LIBRARY_PATH=/usr/local/lib

RUN \
  apt-get update && \
  apt-get install -y git cmake g++-8 qtbase5-dev qt5-default qt5-qmake libcairo2-dev openjdk-8-jdk

RUN mkdir grame

WORKDIR /grame
RUN git clone --single-branch -b dev --depth 1 https://github.com/grame-cncm/midishare.git
WORKDIR /grame/midishare/midisharelight/cmake
RUN  cmake . && make -j 4 && make install

WORKDIR /grame
RUN git clone --single-branch -b dev --depth 1 https://github.com/grame-cncm/guidoar.git
WORKDIR /grame/guidoar/build
RUN make -j 4
RUN make install

WORKDIR /grame
RUN git clone --single-branch -b dev --depth 1 https://github.com/grame-cncm/guidolib.git
WORKDIR /grame/guidolib/build
RUN make basic -j 4
RUN make Qt -j 4 && make install

WORKDIR /grame
