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
RUN cmake ..
RUN make

CMD ["./pokemon"]

