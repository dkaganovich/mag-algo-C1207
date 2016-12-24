#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <queue>
#include <chrono>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include <utility>

namespace
{
  typedef std::vector<int> Stream;

  struct Data {
    int val;
    double prob;

    bool operator<(const Data& o) const { return prob > o.prob; }
  };

  inline double weight_eq(int ord) {
    return 1.0;
  }

  inline double weight_i(int ord) {
    return 1.0 * ord;
  }

  inline double weight_ii(int ord) {
    return 1.0 * ord * ord;
  }

  void initialize_stream(int n, Stream& stream) {
    for (int i = 0; i < n; ++i) {
      stream[i] = i;
    }

    // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // std::default_random_engine gen(seed);

    // std::shuffle(stream.begin(), stream.end(), gen);
  }

  void sample(const Stream& stream, int n, int k, double(*weight_func)(int), int trials, std::ostream& os) {
    os << k * trials << "\n";

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::default_random_engine gen(seed);
    std::uniform_real_distribution<double> rnd(0.0, 1.0);

    std::priority_queue<Data> sample;// max
    for (int i = 0; i < trials; ++i) 
    {
      for (Stream::const_iterator it = stream.begin(); it != stream.end(); ++it) {
        double prob = std::pow(rnd(gen), 1.0 / (*weight_func)(it - stream.begin() + 1));
        if (sample.size() < k) {
          sample.push({ *it, prob });
        } else {
          if (sample.top().prob < prob) {
            sample.pop();
            sample.push({ *it, prob });
          }
        }
      }
      for (int i = 0; i < k; ++i) {
        os << sample.top().val << "\n";
        sample.pop();
      }
    }
  }

  void run_test(const Stream& stream, int n, int k, int trials, const char* of) {
    std::ofstream ofs;
    ofs.open(of, std::ofstream::out);
    sample(stream, n, k, weight_eq, trials, ofs);
    sample(stream, n, k, weight_i, trials, ofs);
    sample(stream, n, k, weight_ii, trials, ofs);
    ofs.close();
  }
}

int main(int argc, char** argv)
{
  Stream stream100(100);
  initialize_stream(100, stream100);
  run_test(stream100, 100, 5, 1000, "100-5.dat");
  run_test(stream100, 100, 10, 1000, "100-10.dat");
  run_test(stream100, 100, 20, 1000, "100-20.dat");
  run_test(stream100, 100, 100, 1000, "100-100.dat");

  Stream stream1000(1000);
  initialize_stream(1000, stream1000);
  run_test(stream1000, 1000, 5, 1000, "1000-5.dat");
  run_test(stream1000, 1000, 10, 1000, "1000-10.dat");
  run_test(stream1000, 1000, 20, 1000, "1000-20.dat");
  run_test(stream1000, 1000, 100, 1000, "1000-100.dat");

  Stream stream10000(10000);
  initialize_stream(10000, stream10000);
  run_test(stream10000, 10000, 5, 1000, "10000-5.dat");
  run_test(stream10000, 10000, 10, 1000, "10000-10.dat");
  run_test(stream10000, 10000, 20, 1000, "10000-20.dat");
  run_test(stream10000, 10000, 100, 1000, "10000-100.dat");
  
  return 0;
}