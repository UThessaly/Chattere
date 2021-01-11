FROM conanio/gcc9 AS builder

USER root

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y build-essential cmake libgtest-dev libboost-test-dev && rm -rf /var/lib/apt/lists/* 

ARG SRC=/src
ARG BUILD=/src/build

RUN mkdir -p ${SRC} 

ADD . ${SRC}

# RUN conan profile update settings.compiler.libcxx=libstdc++11 default

RUN cd ${SRC} \
    && mkdir ${BUILD} \
    && cd build

RUN cd ${BUILD} \
    && cmake ${SRC} \
    -DCMAKE_BUILD_TYPE=Debug 

RUN /usr/bin/cmake --build ${BUILD} --config Debug --target all -- -j 14 ${SRC}

RUN ls ${BUILD}

FROM ubuntu:20.04

USER root

ENV DEBIAN_FRONTEND noninteractive

# RUN apt-get update && apt-get install lua5.3 -y && rm -rf /var/lib/apt/lists/*
RUN apt-get update && apt-get install wget tar make gcc cmake build-essential libreadline-dev unzip sudo lua5.3 -y && rm -rf /var/lib/apt/lists/*

ADD scripts/lua_make.patch /lua_build_patch/Makefile.patch

RUN mkdir lua_build \
    && cd lua_build \
    && wget http://www.lua.org/ftp/lua-5.3.4.tar.gz \
    && tar -zxf lua-5.3.4.tar.gz \
    && cd lua-5.3.4 \
    && cd src \ 
    && patch < /lua_build_patch/Makefile.patch \
    && make "MYCFLAGS=-fPIC" "R=5.3.4" linux \
    && cd .. \
    && make install \
    && ls src \
    && mv src/liblua.so /usr/local/lib 

ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

RUN mkdir /tmp/lua_luarocks_install/ && cd /tmp/lua_luarocks_install/ \
    && wget https://luarocks.org/releases/luarocks-3.5.0.tar.gz \
    && tar zxpf luarocks-3.5.0.tar.gz \ 
    && cd luarocks-3.5.0 \
    && ./configure --with-lua=/usr/local && make && sudo make install

RUN luarocks install luasocket

WORKDIR /usr/app

ARG BUILD=/src/build

COPY --from=builder ${BUILD}/bin/chattere_server /usr/app/chattere_server

RUN whereis lua
RUN ldd /usr/app/chattere_server

ENTRYPOINT [ "/usr/app/chattere_server" ]
