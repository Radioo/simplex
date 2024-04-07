FROM debian:bookworm

RUN apt-get update
RUN apt-get install build-essential cmake git libgtk-3-dev -y

WORKDIR /app

CMD ["/bin/bash"]