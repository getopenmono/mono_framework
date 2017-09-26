FROM ubuntu:zesty
MAINTAINER  Kristoffer Andersen <ka@openmono.com>
RUN apt-get update -q -y && apt-get install -q -y \
  devscripts \
  qt5-default \
  libc6-i386 \
  wget \
  git \
  sudo \
  build-essential \
  qttools5-dev-tools \
  qtmultimedia5-dev
RUN export GCC_DIR="$HOME/gcc-arm-none-eabi-5_2-2015q4" && \
  export GCC_ARCHIVE="$HOME/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2" && \
  export GCC_URL="https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2" && \
  wget $GCC_URL -O $GCC_ARCHIVE -o /dev/null; tar xfj $GCC_ARCHIVE -C $HOME
ENV BRANCH master
ENV RUNTEST false
ENV DEPLOY false
ENV DEPLOY_TARGET /Desktop
CMD export PATH=$PATH:$HOME/gcc-arm-none-eabi-5_2-2015q4/bin && \
  if [ $TRAVIS_BRANCH ]; then BRANCH=$TRAVIS_BRANCH; fi && \
  echo "Using branch: $BRANCH" && \
  git clone -b $BRANCH https://github.com/getopenmono/mono_framework.git && \
  cd mono_framework && \
  if [ $TRAVIS_COMMIT ]; then echo "Using commit: $TRAVIS_COMMIT"; git checkout -qf $TRAVIS_COMMIT; fi && \
  bash resources/setup_icons.sh resources/icons.mk.tmp && \
  make release; \
  bash -c "if [ $RUNTEST == \"true\" ]; then echo \"Running test cases...\"; cd test && bash suite.sh; fi"; \
  bash -c "if [ $DEPLOY == \"true\" -a -d dist -a -d $DEPLOY_TARGET ]; then echo \"Copy artifacts to $DEPLOY_TARGET\"; cp -r dist/* $DEPLOY_TARGET; fi"
