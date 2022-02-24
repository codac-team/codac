from quay.io/pypa/manylinux2010_x86_64

RUN yum install -y cppunit-devel cmake3 flex bison
RUN cd /tmp
RUN echo 'alias cmake=cmake3' >> ~/.bashrc
RUN source ~/.bashrc  
RUN git clone -b ibex-2.8.9 https://github.com/ibex-team/ibex-lib.git
RUN cd ibex-lib   && \
    mkdir build && \
    cd build   && \
    pwd && \
    cmake3 -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" ../   && \
    make -j2   && \
    make check   && \
    make install

