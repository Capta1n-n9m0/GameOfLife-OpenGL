FROM ubuntu:21.10

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y --no-install-recommends
RUN apt-get install -y --reinstall ca-certificates
RUN apt-get install -y --no-install-recommends gcc
RUN apt-get install -y --no-install-recommends git
RUN apt-get install -y --no-install-recommends curl
RUN apt-get install -y --no-install-recommends wget
RUN apt-get install -y --no-install-recommends bash
RUN apt-get install -y --no-install-recommends make
RUN apt-get install -y --no-install-recommends cmake
RUN apt-get install -y --no-install-recommends mesa-utils
RUN apt-get install -y --no-install-recommends freeglut3
RUN apt-get install -y --no-install-recommends freeglut3-dev
RUN apt-get install -y --no-install-recommends mesa-common-dev
RUN apt-get install -y --no-install-recommends xorg-dev
RUN apt-get install -y --no-install-recommends libglu1-mesa-dev
RUN apt-get install -y --no-install-recommends x11-xserver-utils
RUN apt-get install -y --no-install-recommends x11vnc
RUN apt-get install -y --no-install-recommends xinit
RUN apt-get install -y --no-install-recommends xserver-xorg-video-dummy
RUN apt-get install -y --no-install-recommends websockify
RUN apt-get install -y --no-install-recommends supervisor
RUN apt-get install -y --no-install-recommends tint2
RUN apt-get install -y --no-install-recommends net-tools
RUN apt-get install -y --no-install-recommends libgl1-mesa-dri
RUN apt-get install -y --no-install-recommends menu
RUN rm -rf /var/cache/apt && \
    rm -f /usr/share/applications/x11vnc.desktop

RUN git clone https://github.com/kanaka/noVNC.git /opt/noVNC && \
  cd /opt/noVNC && \
  git checkout 6a90803feb124791960e3962e328aa3cfb729aeb && \
  ln -s vnc_auto.html index.html

# noVNC (http server) is on 6080, and the VNC server is on 5900
EXPOSE 6080 5900

ENV DISPLAY :0

ADD . /GameOfLife

WORKDIR /GameOfLife

RUN make build

ENTRYPOINT ["./gameOfLife"]
