# Simple image matching

This project builds two targets

* comphoassn1.a - A library for single-scale / multi-scale image matching
* main.out - A demo executable that works for BGR images stacked vertically

## Build

### Dependencies

* OpenMP
* OpenCV 4.1.1 
* CMake

```bash
mkdir build && cd build
cmake ../ && make
```

## Usage

Run `build/main.out --help` for usage

See `run.bash` for execution example

```
Usage: <exe name> <alg_mode: ('single' | 'multi' | 'recon')>

<exe name> single <cost: ('sos' | 'ncc' | 'zncc')> <pert_x: float> <pert_y: float> <padding: int> <img_path> <out_name>
Used for single-scale image matching

<exe name> multi <cost: ('sos' | 'ncc' | 'zncc')> <pert_x: float> <pert_y: float> <padding: int> <levels: int> <img_path> <out_name>
Used for multi-scale image matching (4-level Gaussian pyramid)

<exe name> recon <img_path> <vec_path> <out_name>
Used for merging BGR images from output displacement file without optimization


cost=sos - Use -(sum of squares) as score function
cost=ncc - Use normalized cross correlation as score function
cost=zncc - Use zero-mean normalized cross correlation as score function

pert_x, pert_y - Radius of translation search
padding - Padding radius to use when center-cropping before beginning the match
levels - Number of levels in multi-level image pyramid 
img_path - A path to a single channel image (BGR image stacked vertically).
out_name - Output name to be used as prefix for a displacement file(${out_name}.txt)/merged image without matching(${out_name}_orig.png)/matched image(${out_name}_result.png)
```

