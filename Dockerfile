FROM ubuntu:18.04

RUN    apt-get update \
    && apt-get install -q -y \
        cmake \
        clang++-8 \
        git \
        libboost-dev \
        libbz2-dev \
        libexpat1-dev \
        libproj-dev \
        libsparsehash-dev \
        make \
        spatialite-bin \
        zlib1g-dev \
    && rm -rf /var/lib/apt/lists/* /tmp/*

RUN ln -s /usr/bin/clang++-8 /usr/bin/clang++
RUN     mkdir /usr/src/app

WORKDIR /usr/src/app

COPY . .

RUN     mkdir ./build && cd build && cmake .. && make
