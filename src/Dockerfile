FROM gcc:latest

COPY ./course_Client.c /DockerClient/client.c

WORKDIR /DockerClient

RUN gcc client.c -o client -lpthread

ENTRYPOINT ["./client"]