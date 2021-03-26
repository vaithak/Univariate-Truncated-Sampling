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
double target_unnorm_pdf_1(double x) {
    return sqrt((x) + (x*x) - (x*x*x) + 2);
}

// unnormalized bimodal target pdf
double target_unnorm_pdf_2(double x) {
    return sqrt(4.5*(x-1) + pow(x-1, 2) - 4*pow(x-1, 3) - pow(x-1, 4) + 3);
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
    // First distribution
    pair<double, double> support_1 (-2.0, 2.0);
    double kernel_sigma_1 = min(1.0, (support_1.second - support_1.first)/8.0);
    std::random_device rd_1;
    std::default_random_engine generator_1;
    generator_1.seed( rd_1() );
    Normal_PDF normal_pdf_1(kernel_sigma_1);
    std::normal_distribution<double> normal_kernel_1(0.0, kernel_sigma_1);
    vector<double> samples_1 = metropolis_hastings(target_unnorm_pdf_1, normal_kernel_1, 
                                            normal_pdf_1, generator_1, support_1, (support_1.first + support_1.second)/2.0);
    write_data_to_file(samples_1, "data_1.txt");


    // Second distribution
    pair<double, double> support_2 (-3.0, 2.2);
    double kernel_sigma_2 = min(1.0, (support_2.second - support_2.first)/8.0);
    std::random_device rd_2;
    std::default_random_engine generator_2;
    generator_2.seed( rd_2() );
    Normal_PDF normal_pdf_2(kernel_sigma_2);
    std::normal_distribution<double> normal_kernel_2(0.0, kernel_sigma_2);
    vector<double> samples_2 = metropolis_hastings(target_unnorm_pdf_2, normal_kernel_2, 
                                            normal_pdf_2, generator_2, support_2, (support_2.first + support_2.second)/2.0);
    write_data_to_file(samples_2, "data_2.txt");
    return 0;
}
