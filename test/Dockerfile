FROM ubuntu:zesty
MAINTAINER  Kristoffer Andersen <ka@openmono.com>
RUN apt-get update -qq && \
  apt-get install -qq devscripts qt5-default \
   libc6-i386 wget git sudo build-essential qttools5-dev-tools qtmultimedia5-dev
RUN export GCC_DIR="$HOME/gcc-arm-none-eabi-5_2-2015q4" && \
  export GCC_ARCHIVE="$HOME/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2" && \
  export GCC_URL="https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2" && \
  wget $GCC_URL -O $GCC_ARCHIVE; tar xfvj $GCC_ARCHIVE -C $HOME
ENV BRANCH master
CMD export PATH=$PATH:$HOME/gcc-arm-none-eabi-5_2-2015q4/bin && \
  echo "Using branch: $BRANCH" && \
  git clone -b $BRANCH https://github.com/getopenmono/mono_framework.git && \
  cd mono_framework && \
  bash resources/setup_icons.sh resources/icons.mk dist && \
  make release && \
  cd test && bash suite.sh