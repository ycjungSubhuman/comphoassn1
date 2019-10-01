#!/bin/bash

# Matches all images under data/ and stores report in examples/

function run_all() {
    for path_img in data/*.tif data/*.jpg; do
        name=${path_img%\.*}
        name=${name##*/}

        mkdir -p examples/tiles

        prefix_out=examples/${name}_multi_zncc
        path_orig=examples/${name}_multi_zncc_orig.png
        path_result=examples/${name}_multi_zncc_result.png
        path_single=examples/${name}_single_zncc_result.png
        path_vec=examples/${name}_multi_zncc.txt
        path_single_vec=examples/${name}_single_zncc.txt
        path_output=examples/tiles/${name}.png

        montage -tile 3x1 -title "($(cat $path_vec | tr '\n' ' ')) / ($(cat $path_single_vec | tr '\n' ' '))" -pointsize 24 -geometry 933x799 $path_orig $path_result $path_single $path_output
    done
}

run_all

