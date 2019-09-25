#!/bin/bash

# Matches all images under data/ and stores report in examples/

function run() {
    alg=$1
    fn_score=$2
    radius=$3
    crop=$4
    path_img=$5
    name=${path_img%\.*}
    name=${name##*/}
    path_out=examples/${name}_${alg}_${fn_score}
    path_stdout=examples/${name}_${alg}_${fn_score}_stdout.txt
    path_time=examples/${name}_${alg}_${fn_score}_time.txt

    mkdir -p examples

    echo "running $alg $fn_score $radius $radius $crop $path_img $path_out ..."

    ./build/main.out $alg $fn_score $radius $radius $crop $path_img $path_out \
        > $path_stdout

    time=$(cat $path_stdout | egrep -o '[0-9]+ ms' | sed -E 's/([0-9]+) ms/\1/')
    echo "Took $time ms"
    echo $time > $path_time
}

function run_all() {
    for fn_score in sos ncc zncc; do
        for tif_file in data/*.tif; do
            run multi $fn_score 8 300 $tif_file 
        done

        for jpg_file in data/*.jpg; do
            run multi $fn_score 4 40 $jpg_file 
        done
    done


    for fn_score in sos ncc zncc; do
        for tif_file in data/*.tif; do
            run single $fn_score 128 300 $tif_file 
        done

        for jpg_file in data/*.jpg; do
            run single $fn_score 64 40 $jpg_file 
        done
    done

}

OMP_NUM_THREADS=8
run_all

