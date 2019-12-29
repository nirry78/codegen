#!/bin/bash
mkdir -p output
mkdir -p output/release
mkdir -p output/debug
pushd output/release && cmake -DCMAKE_BUILD_TYPE=Release ../.. && popd
pushd output/debug && cmake -DCMAKE_BUILD_TYPE=Debug ../.. && popd
