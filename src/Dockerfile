FROM gcc:latest

COPY ./client/client.c /DockerClient/client/client.c

COPY ./common.c /DockerClient/common.c

COPY ./common.h /DockerClient/common.h

WORKDIR /DockerClient

RUN gcc client/client.c common.c -o cli -lpthread

ENTRYPOINT ["./cli"]