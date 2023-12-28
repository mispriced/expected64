#!/bin/bash

# Script to install different compilers based on input argument

compiler=$1

case $compiler in
  gcc-11)
    sudo apt update && sudo apt install -y gcc-11 g++-11
    echo "CC=gcc-11" >> $GITHUB_ENV
    echo "CXX=g++-11" >> $GITHUB_ENV
    ;;
  gcc-12)
    sudo apt update && sudo apt install -y gcc-12 g++-12
    echo "CC=gcc-12" >> $GITHUB_ENV
    echo "CXX=g++-12" >> $GITHUB_ENV
    ;;
  gcc-13)
    sudo apt update && sudo apt install -y gcc-13 g++-13
    echo "CC=gcc-13" >> $GITHUB_ENV
    echo "CXX=g++-13" >> $GITHUB_ENV
    ;;
  clang-16)
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
    sudo add-apt-repository 'deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main'
    sudo apt update && sudo apt install -y clang-16
    echo "CC=clang-16" >> $GITHUB_ENV
    echo "CXX=clang++-16" >> $GITHUB_ENV
    ;;
  clang-17)
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
    sudo add-apt-repository 'deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-17 main'
    sudo apt update && sudo apt install -y clang-17
    echo "CC=clang-17" >> $GITHUB_ENV
    echo "CXX=clang++-17" >> $GITHUB_ENV
    ;;
  *)
    echo "Unsupported compiler: $compiler"
    exit 1
    ;;
esac
