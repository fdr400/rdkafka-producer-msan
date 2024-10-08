# rdkafka-producer-msan
Reproduction for use-of-unitialized-value in rdtime.h

# Bug
```
==3601446==WARNING: MemorySanitizer: use-of-uninitialized-value
    #0 0x5555559e8cfd in rd_timeout_init_timespec /home/fdr400/tmp/librdkafka/src/rdtime.h:255:21
    #1 0x5555559e7a76 in rd_kafka_q_serve /home/fdr400/tmp/librdkafka/src/rdkafka_queue.c:543:9
    #2 0x55555564f71e in rd_kafka_thread_main /home/fdr400/tmp/librdkafka/src/rdkafka.c:2155:17
    #3 0x7ffff789c86a in start_thread nptl/pthread_create.c:444:30
    #4 0x7ffff7929c3b in clone3 misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:78

SUMMARY: MemorySanitizer: use-of-uninitialized-value /home/fdr400/tmp/librdkafka/src/rdtime.h:255:21 in rd_timeout_init_timespec
Exiting
```

# How to reproduce?
1. Download and build `librdkafka`.
```bash
$ git@github.com:confluentinc/librdkafka.git
$ cd librdkafka
$ cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DRDKAFKA_BUILD_STATIC=ON \
    -DRDKAFKA_BUILD_EXAMPLES=OFF \
    -DRDKAFKA_BUILD_TESTS=OFF \
    -DWITH_SSL=OFF -DWITH_SASL=OFF -DWITH_CURL=OFF \
    -DWITH_ZLIB=OFF -DWITH_LIBDL=OFF\
    -DWITH_ZSTD=OFF -DENABLE_LZ4_EXT=OFF \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_C_FLAGS="-fsanitize=memory -fno-omit-frame-pointer" \
    -S./ -B./build
$ cmake --build build -- -j $(nproc)
$ export KAFKA_SOURCE=$(pwd)
$ cd ..
```

2. Clone this repo and build main file.
```bash
$ git clone git@github.com:fdr400/rdkafka-producer-msan.git
$ cd rdkafka-producer-msan
$ cmake \
    -DRD_KAFKA_STATIC_LIB_PATH=$KAFKA_SOURCE/build/src/librdkafka.a \
    -DRD_KAFKA_INCLUDE_PATH=$KAFKA_SOURCE/src \
    -S./ -B./build
$ ./build/rdkafka-producer # <- memory sanitizer fails
$ cd ..
```

3. Clone this PR.
```bash
$ git clone git@github.com:fdr400/librdkafka.git librdkafka-fix
$ cd librdkafka-fix
$ git checkout fix-msan-rdtime
$ cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DRDKAFKA_BUILD_STATIC=ON \
    -DRDKAFKA_BUILD_EXAMPLES=OFF \
    -DRDKAFKA_BUILD_TESTS=OFF \
    -DWITH_SSL=OFF -DWITH_SASL=OFF -DWITH_CURL=OFF \
    -DWITH_ZLIB=OFF -DWITH_LIBDL=OFF\
    -DWITH_ZSTD=OFF -DENABLE_LZ4_EXT=OFF \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_C_FLAGS="-fsanitize=memory -fno-omit-frame-pointer" \
    -S./ -B./build
$ cmake --build build -- -j $(nproc)
$ export KAFKA_FIX_SOURCE=$(pwd)
$ cd ../rdkafka-producer-msan
$ cmake \
    -DRD_KAFKA_STATIC_LIB_PATH=$KAFKA_FIX_SOURCE/build/src/librdkafka.a \
    -DRD_KAFKA_INCLUDE_PATH=$KAFKA_FIX_SOURCE/src \
    -S./ -B./build
$ cmake --build build
$ ./build/rdkafka-producer # <- OK
```
