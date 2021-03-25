#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>

using namespace std;

void write_data_to_file(const vector<double>& data, string filename) {
    ofstream outfile;
    outfile.open(filename);

    for(int i=0; i<data.size(); ++i) {
        outfile << data[i] << "\n";
    }

    outfile.close();
}


double min(double a, double b) {
    return (a < b) ? a : b;
}


// unnormalized target pdf
double target_unnorm_pdf(double x) {
    return sqrt((x) + (x*x) - (x*x*x) + 2);
}


// Functor for normal pdf to be passed as transition kernel
class Normal_PDF {
    double sigma;

public:
    Normal_PDF(double sigma = 1.0) {
        this->sigma = sigma;
    }

    double operator() (double x, double mu) {
       return (double)exp(-pow((x - mu) / sigma, 2) / 2);
    }
};


// Metropolis hastings sampling
template <typename TARGET_PDF, typename TRANSITION_KERNEL_SAMPLER,
         typename TRANSITION_KERNEL_PDF, typename RAND>
vector<double> metropolis_hastings (  TARGET_PDF target_pdf, 
                                      TRANSITION_KERNEL_SAMPLER kernel_sampler, 
                                      TRANSITION_KERNEL_PDF kernel_pdf, 
                                      RAND generator,
                                      const pair<double, double>& support,
                                      double init_point,
                                      unsigned int n_samples = 100000) 
{
    uniform_real_distribution<double> uniform_sampler(0.0, 1.0);
    unsigned int curr_n_samples = 0;
    vector<double> samples;

    double x = init_point;
    while (curr_n_samples < n_samples) {
        double y = x + kernel_sampler(generator);

        if (y >= support.first && y <= support.second) {
            double metropolis_ratio = target_pdf(y) / target_pdf(x);
            double hastings_ratio = kernel_pdf(x, y) / kernel_pdf(y, x);
            
            double accept_prob = min(1, metropolis_ratio * hastings_ratio);
            double uniform_sample = uniform_sampler(generator);
            bool accepted = (uniform_sample < accept_prob);

            if (accepted) {
                x = y;
            }
        }

        curr_n_samples += 1;
        samples.push_back(x);
    }

    return samples;
}


int main(int argc, char const *argv[]) {
    pair<double, double> support (-2.0, 2.0);
    double kernel_sigma = min(1.0, (support.second - support.first)/8.0);
    std::random_device rd;
    std::default_random_engine generator;
    generator.seed( rd() );
    Normal_PDF normal_pdf(kernel_sigma);
    std::normal_distribution<double> normal_kernel(0.0, kernel_sigma);
    vector<double> samples = metropolis_hastings(target_unnorm_pdf, normal_kernel, 
                                            normal_pdf, generator, support, (support.first + support.second)/2.0);
    write_data_to_file(samples, "data_1.txt");
    return 0;
}
