FROM gcc:14

RUN apt-get update && \
    apt-get install -y cmake && \
    apt-get clean

WORKDIR /usr/src/app
COPY CMakeLists.txt .
COPY data data
COPY external external
COPY include include
COPY src src

WORKDIR /usr/src/app/build
RUN --mount=type=cache,target=/usr/src/app/build,sharing=locked \
    cmake ..
RUN --mount=type=cache,target=/usr/src/app/build,sharing=locked \
    make -C /usr/src/app/build
RUN --mount=type=cache,target=/usr/src/app/build,sharing=locked \
    cp /usr/src/app/build/pokemon /tmp/pokemon
RUN mv /tmp/pokemon /usr/src/app/build/pokemon

CMD ["./pokemon"]

