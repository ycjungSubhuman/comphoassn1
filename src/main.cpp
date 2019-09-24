#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include "match_score.hpp"
#include "multi_scale.hpp"
#include "single_scale.hpp"
#include "util.hpp"

using namespace compho;
namespace chr = std::chrono;

struct AppConfig {
    std::string alg_mode;
    std::string cost;
    float pert_x;
    float pert_y;
    int padding;
    std::string img_path;
    std::string vec_path;
    std::string out_name;
};

AppConfig parse_config(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        std::string str(argv[i]);
        if (str == "--help") {
            std::cout << "Usage: <exe name> <alg_mode: ('single' | 'multi' | "
                         "'recon')>"
                      << std::endl;
            std::cout << "<exe name> single <cost: ('sos' | 'ncc' | 'zncc')> "
                         "<pert_x: float> "
                         "<pert_y: float> <padding: int> <img_path> <out_name>"
                      << std::endl;
            std::cout << "<exe name> multi <cost: ('sos' | 'ncc' | 'zncc')> "
                         "<pert_x: float> "
                         "<pert_y: float> <padding: int> <img_path> <out_name>"
                      << std::endl;
            std::cout << "<exe name> recon <img_path> <vec_path> <out_name>"
                      << std::endl;
            exit(0);
        }
    }

    if (argc < 2) {
        std::cerr << "--help for usage" << std::endl;
    }

    AppConfig result;
    result.alg_mode = std::string(argv[1]);

    if (result.alg_mode == "single" || result.alg_mode == "multi") {
        if (argc != 8) {
            std::cerr << "Put 7 arguments. --help for usage." << std::endl;
            exit(1);
        }

        result.cost = std::string(argv[2]);
        result.pert_x = std::stof(std::string(argv[3]));
        result.pert_y = std::stof(std::string(argv[4]));
        result.padding = std::stoi(std::string(argv[5]));
        result.img_path = std::string(argv[6]);
        result.out_name = std::string(argv[7]);

    } else if (result.alg_mode == "recon") {
        if (argc != 5) {
            std::cerr << "Put 4 arguments. --help for usage." << std::endl;
            exit(1);
        }

        result.img_path = std::string(argv[2]);
        result.vec_path = std::string(argv[3]);
        result.out_name = std::string(argv[4]);
    } else {
        std::cerr << "Unknown alg_mode: " << result.alg_mode << std::endl;
        exit(2);
    }

    return result;
}

std::vector<cv::Mat1f> get_images(const AppConfig &config) {
    cv::Mat1f single_col_image =
        normalize_image(cv::imread(config.img_path, cv::IMREAD_UNCHANGED));
    std::vector<cv::Mat1f> images = split_single_column_image(single_col_image);
    return images;
}

void write_results(const AppConfig &config,
                   const std::vector<cv::Mat1f> &images,
                   const std::vector<cv::Vec2i> &results) {
    write_displacement_file(config.out_name + ".txt", results);
    write_merged_image(config.out_name + "_result.png", images, results);
    write_merged_image(config.out_name + "_orig.png", images,
                       std::vector<cv::Vec2i>(images.size(), cv::Vec2i(0, 0)));
}

void run_optim_mode(const AppConfig &config) {
    cv::Vec2i pert(config.pert_x, config.pert_y);

    std::unordered_map<std::string, std::shared_ptr<MatchingScore>> map_cost = {
        {"sos", std::make_shared<SumOfSquare>()},
        {"ncc", std::make_shared<NormalizedCrossCorrelation>()},
        {"zncc", std::make_shared<ZeroMeanNormalizedCrossCorrelation>()}};

    std::unordered_map<std::string, std::shared_ptr<AlignAlgorithm>>
        map_alg_mode = {
            {"single", std::make_shared<SingleScalePairAlignAlgorithm>(
                           pert, map_cost[config.cost])},
            {"multi", std::make_shared<MultiScalePairAlignAlgorithm>(
                          pert, map_cost[config.cost])}};

    std::shared_ptr<AlignAlgorithm> alg = map_alg_mode[config.alg_mode];

    std::vector<cv::Mat1f> images = get_images(config);
    std::vector<cv::Mat1f> images_cropped = crop_images(images, config.padding);

    chr::system_clock clock;
    chr::system_clock::time_point start = clock.now();
    std::vector<cv::Vec2i> results = alg->align(images_cropped);
    chr::system_clock::time_point end = clock.now();
    chr::milliseconds milli =
        chr::duration_cast<chr::milliseconds>(end - start);
    std::cout << "Time took optimizing: " << milli.count() << " ms"
              << std::endl;

    write_results(config, images, results);
}

void run_recon_mode(const AppConfig &config) {
    std::vector<cv::Mat1f> images = get_images(config);
    std::vector<cv::Vec2i> results;
    read_displacement_file(config.vec_path, results);
    write_results(config, images, results);
}

int main(int argc, char **argv) {
    AppConfig config = parse_config(argc, argv);

    if (config.alg_mode == "recon") {
        run_recon_mode(config);
    } else {
        run_optim_mode(config);
    }

    return 0;
}

