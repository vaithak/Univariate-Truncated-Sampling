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

// unnormalized target pdf
double target_unnorm_pdf(double x) {
    return sqrt((x) + (x*x) - (x*x*x) + 2);
}


// Functor for uniform pdf to be passed as proposal pdf
class Uniform_PDF {
    double a, b;

public:
    Uniform_PDF(double a, double b) {
        this->a = a;
        this->b = b;
    }

    double operator() (double x) {
       return 1.0/(b - a);
    }
};


// Accept Reject sampling when the max value of (target/proposal) is given
template <typename TARGET_PDF, typename PROPOSAL_SAMPLER, typename PROPOSAL_PDF, typename RAND>
vector<double> accept_reject (  TARGET_PDF target_pdf, 
                                PROPOSAL_SAMPLER proposal_sampler, 
                                PROPOSAL_PDF proposal_pdf, 
                                RAND generator,
                                double max_val, 
                                unsigned int n_samples = 10000) 
{
    uniform_real_distribution<double> uniform_sampler(0.0, max_val);
    unsigned int curr_n_samples = 0;
    vector<double> samples;

    unsigned int max_iter = 10*n_samples;
    unsigned int iter = 0;

    while (curr_n_samples < n_samples) {
        double curr_proposed_sample = proposal_sampler(generator);
        double uniform_sample = uniform_sampler(generator);

        bool accepted = (uniform_sample*proposal_pdf(curr_proposed_sample) <= target_pdf(curr_proposed_sample));
        if (accepted) {
            samples.push_back(curr_proposed_sample);
            curr_n_samples += 1;
        }

        iter += 1;
        if (iter >= max_iter)
            break;
    }

    if (curr_n_samples != n_samples) {
        cout << "Only " << curr_n_samples << " could be sampled !!!";
    }

    return samples;
}


/**
  Accept Reject sampling when the max value is to be calculated


 'n_samples_max_val' is for number of random samples to use for calculating 
                     the max value of target_pdf / proposal_pdf 
 **/
template <typename TARGET_PDF, typename PROPOSAL_SAMPLER, typename PROPOSAL_PDF, typename RAND>
vector<double> accept_reject (  TARGET_PDF target_pdf, 
                                PROPOSAL_SAMPLER proposal_sampler, 
                                PROPOSAL_PDF proposal_pdf, 
                                RAND generator,
                                const pair<double, double>& support,
                                int n_samples_max_val = 1000,
                                unsigned int n_samples = 10000 ) 
{
    // calculate max value of target_pdf divided by proposal pdf
    double max_val = 0.0;
    uniform_real_distribution<double> uniform_sampler(support.first, support.second);

    for (int i=0; i<n_samples_max_val; ++i) {
        double x = uniform_sampler(generator);
        double curr_res = target_pdf(x) / proposal_pdf(x);

        if (curr_res > max_val)
            max_val = curr_res;
    }

    max_val = (max_val*1.1); // to avoid numerical errors
    cout<<"Max val (with margin): "<<max_val<<endl;

    return accept_reject(target_pdf, proposal_sampler, proposal_pdf, generator, max_val, n_samples);
}


int main(int argc, char const *argv[]) {
    pair<double, double> support (-2.0, 2.0);
    default_random_engine generator;
    uniform_real_distribution<double> proposal_sampler(support.first, support.second);
    Uniform_PDF uniform_pdf(support.first, support.second);

    vector<double> samples = accept_reject(target_unnorm_pdf, proposal_sampler, uniform_pdf, generator, support);
    write_data_to_file(samples, "data_1.txt");
    return 0;
}
