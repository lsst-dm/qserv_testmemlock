FROM debian:jessie
MAINTAINER Fabrice Jammes <fabrice.jammes@in2p3.fr>

# Start with this long step not to re-run it on
# each Dockerfile update
RUN apt-get -y update && \
    apt-get -y install apt-utils && \
    apt-get -y install g++ vim && \
    apt-get -y upgrade && \
    apt-get -y clean

ENV workdir /qserv_testmlk

COPY src/* $workdir/

RUN groupadd qserv && \
    useradd -m -g qserv qserv && \
    usermod -s /bin/bash qserv && \
    chown qserv:qserv $workdir 

USER qserv

WORKDIR $workdir/

RUN ./build.sh

CMD ./run.sh
