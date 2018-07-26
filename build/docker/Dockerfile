FROM ubuntu:bionic

RUN \
  apt-get update && \
  apt-get install -y git cmake g++-8 qtbase5-dev qt5-default qt5-qmake libcairo2-dev

RUN \
  mkdir src && cd src && \
  git clone https://github.com/grame-cncm/midishare.git && \
  git clone https://github.com/grame-cncm/guidolib.git

RUN \
  cd src/midishare/midisharelight/cmake && \
  cmake . && make -j 4 && make install && \
  echo "==========================================" && \
  echo "=======         midishare done        ====" && \
  echo "=========================================="

RUN \
  cd src/guidolib/build && \
  make -j 4 && make install && \
  echo "==========================================" && \
  echo "=======         guidolib done         ====" && \
  echo "=========================================="
