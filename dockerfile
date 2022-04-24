FROM ubuntu:21.10
RUN apt-get update -y --no-install-recommends && \
    apt-get install -y --reinstall ca-certificates && \
#    apt-get install -y --no-install-recommends build-essential && \
    apt-get install -y --no-install-recommends gcc && \
    apt-get install -y --no-install-recommends git && \
    apt-get install -y --no-install-recommends curl && \
    apt-get install -y --no-install-recommends wget && \
    apt-get install -y --no-install-recommends bash && \
    apt-get install -y --no-install-recommends make && \
    apt-get install -y --no-install-recommends cmake && \
    apt-get install -y --no-install-recommends mesa-utils && \
    apt-get install -y --no-install-recommends freeglut3 && \
    apt-get install -y --no-install-recommends freeglut3-dev && \
    apt-get install -y --no-install-recommends mesa-common-dev && \
    apt-get install -y --no-install-recommends xorg-dev && \
    apt-get install -y --no-install-recommends libglu1-mesa-dev && \
    rm -rf /var/cache/apt-get && \
    apt-get clean && \
    git config --global http.sslverify false && \
    git clone https://github.com/Capta1n-n9m0/GameOfLife-OpenGL.git GameOfLife
WORKDIR GameOfLife
CMD ["make"]
