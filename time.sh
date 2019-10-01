#!/bin/bash

# Matches all images under data/ and stores report in examples/

function avg() {
    acc=0

    for f in $@; do
        acc=$(echo "$acc + $(cat $f)" | bc)
    done

    echo "$acc / $#" | bc
}

function run_all() {
    for mode in single multi; do
        val=$(avg examples/*v*${mode}*zncc_time.txt)
        echo "$mode / small : $val"
        val=$(avg examples/*u*${mode}*zncc_time.txt)
        echo "$mode / big : $val"
    done
}

run_all

