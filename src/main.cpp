#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

#include "match_cost.hpp"
#include "single_scale.hpp"
#include "util.hpp"

using namespace compho;

struct AppConfig {
    std::string alg_mode;
    std::string cost;
    std::string img_path;
    float pert_x;
    float pert_y;
};

AppConfig parse_config(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        std::string str(argv[i]);
        if (str == "--help") {
            std::cout << "Usage: <exe name> <alg_mode: ('single' | 'multi')> "
                         "<cost: ('sos' | 'ncc' | 'zncc')> <pert_x: float> "
                         "<pert_y: float> <img_path>"
                      << std::endl;
            exit(0);
        }
    }

    if (argc != 4) {
        std::cerr << "Put 4 arguments. --help for usage." << std::endl;
        exit(1);
    }

    AppConfig result;
    result.alg_mode = std::string(argv[1]);
    result.cost = std::string(argv[2]);
    result.pert_x = std::stof(std::string(argv[3]));
    result.pert_y = std::stof(std::string(argv[4]));
    result.img_path = std::string(argv[5]);
}

int main(int argc, char **argv) {
    AppConfig config = parse_config(argc, argv);

    cv::Vec2i pert(config.pert_x, config.pert_y);

    std::unordered_map<std::string, std::shared_ptr<MatchingCost>> map_cost = {
        {"sos", std::make_shared<SumOfSquare>()},
        {"ncc", std::make_shared<NormalizedCrossCorrelation>()},
        {"zncc", std::make_shared<ZeroMeanNormalizedCrossCorrelation>()}};

    std::unordered_map<std::string, std::shared_ptr<AlignAlgorithm>>
        map_alg_mode = {
            {"single", std::make_shared<SingleScalePairAlignAlgorithm>(
                           pert, map_cost[config.cost])}};

    std::shared_ptr<AlignAlgorithm> alg = map_alg_mode[config.alg_mode];

    cv::Mat single_col_image = cv::imread(config.img_path);
    std::vector<cv::Mat> images =
        split_single_column_image(single_col_image, 3);
    std::vector<cv::Vec2i> results = alg->align(images);

    write_displacement_file("result.txt", results);

    return 0;
}

