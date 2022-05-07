# To build run: sudo docker build -tyoucannotgoback:latest .
# For debugging you may need:
# DOCKER_BUILDKIT=0 sudo docker build -tyoucannotgoback:latest .

FROM  gcc:latest

LABEL org.opencontainers.image.authors="Tim Martin <timboe@users.noreply.github.com>"
LABEL org.opencontainers.image.source="git@github.com:timboe/YouCannotGoBack.git"
LABEL uk.co.timmartin.ycgb="true"

RUN apt-get update \
    && apt-get -y install \
        sudo \
        wget \
    && rm -rf /var/lib/apt/lists/* \
    && addgroup ycgb \
    && adduser --shell /bin/bash --ingroup ycgb ycgb \
    && adduser ycgb sudo \
    && passwd -d ycgb

USER ycgb

SHELL ["/bin/bash", "-c"]

WORKDIR /home/ycgb
RUN sudo chown ycgb:ycgb /home/ycgb

RUN wget https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz
RUN tar -xf gcc-arm-11.2-2022.02-x86_64-arm-none-eabi.tar.xz

RUN wget https://download.panic.com/playdate_sdk/Linux/PlaydateSDK-1.10.0.tar.gz
RUN tar -xf PlaydateSDK-1.10.0.tar.gz

COPY --chown=ycgb:ycgb . ./YouCannotGoBack/playdate

ENV PATH="/home/ycgb/gcc-arm-11.2-2022.02-x86_64-arm-none-eabi/bin:${PATH}"
ENV PLAYDATE_SDK_PATH="/home/ycgb/PlaydateSDK-1.10.0"

RUN cd YouCannotGoBack/playdate && make

# once this step works you would add commands here to copy/publish your build artifacts somewhere

CMD ["/bin/bash"]
