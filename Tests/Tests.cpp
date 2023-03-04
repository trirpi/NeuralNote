#include <JuceHeader.h>
#include "Features.h"
#include "BasicPitchCNN.h"
#include <vector>

#include <chrono>

int main()
{
    std::vector<float> x;

    Features feature_calculator;
    size_t num_input_samples = 100000;
    x.resize(num_input_samples);

    size_t num_out_frames;

    BasicPitchCNN mModel;
    std::vector<float> onsets(NUM_FREQ_OUT, 0.0);
    std::vector<float> notes(NUM_FREQ_OUT, 0.0);
    std::vector<float> contours(NUM_FREQ_IN, 0.0);
    std::vector<float> in_data(NUM_FREQ_IN * NUM_HARMONICS, 0.0);

    auto start_time = std::chrono::high_resolution_clock::now();
    const float* stacked_cqt_data =
        feature_calculator.computeFeatures(x.data(), num_input_samples, num_out_frames);

    std::cout << num_out_frames << std::endl;

    for (size_t i = 0; i < num_out_frames; i++)
    {
        std::copy(stacked_cqt_data + i * NUM_HARMONICS * NUM_FREQ_IN,
                  stacked_cqt_data + (i + 1) * NUM_HARMONICS * NUM_FREQ_IN,
                  in_data.begin());

        mModel.frameInference(in_data, contours, notes, onsets);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> execution_duration = end_time - start_time;

    std::cout << "Execution time: " << execution_duration.count() << " seconds" << std::endl;

    std::cout << "Success" << std::endl;
}