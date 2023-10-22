#!/bin/bash
cmake -S . -B build
pushd build
make
pushd ..
build/Botanic-Resurgance
