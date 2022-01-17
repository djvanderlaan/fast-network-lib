#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>


#include <thread>
#include <future>

#include <benchmark/benchmark.h>


// Determine for each coordinate (x,y) the closest neighbour
bool nn1(const std::vector<double>& x, const std::vector<double>& y, 
    size_t i0, size_t iend, std::vector<size_t>& nn) {

  size_t n = x.size();
  
  for (size_t i = i0; i < iend; ++i) {
    const double xi = x[i];
    const double yi = y[i];
    size_t current_nn = 0;
    double current_d  = std::numeric_limits<double>::infinity();
    for (size_t j = 0; j < n; ++j) {
      const double dx = (xi - x[j]);
      const double dy = (yi - y[j]);
      const double d = std::sqrt(dx * dx + dy * dy);
      if (d < current_d) {
        current_d = d;
        current_nn = j;
      }
    }
    nn[i] = current_nn;
  }
  return true;
}

void nn1_promise(const std::vector<double>& x, const std::vector<double>& y, 
    size_t i0, size_t iend, std::vector<size_t>& nn, std::promise<bool> promise) {
  bool res = nn1(x, y, i0, iend, nn);
  promise.set_value(res);
}

bool my_fun(const std::vector<double>& v0, unsigned int i0, unsigned int iend, 
    std::vector<double>& v1) {
  double max_val = 0.0;
  for (unsigned int i = i0; i < iend; ++i) {
    const double v = std::sqrt(v0[i]);
    max_val = std::max(max_val, std::abs(v - 1.0));
    v1[i] = v;
  }
  return max_val < 0.001;
}

void my_fun_promise(const std::vector<double>& v0, unsigned int i0, unsigned int iend, 
    std::vector<double>& v1, std::promise<bool> promise) {
  bool res = my_fun(v0, i0, iend, v1);
  promise.set_value(res);
}





std::vector<size_t> chunk(size_t n, size_t nchunks) {
  size_t chunk_size = std::ceil(static_cast<double>(n)/
      static_cast<double>(nchunks));
  std::vector<size_t> result(nchunks+1);
  result[0] = 0;
  for (size_t i = 0; i < nchunks; ++i) {
    result[i+1] = std::min(n, result[i] + chunk_size);
  }
  return result;
}




static void BM_threaded(benchmark::State& state) {
  //unsigned int n = 1000000;
  //unsigned int nthread = 4;

  unsigned int n = state.range(0);
  unsigned int nthread = state.range(1);

  auto chunks = chunk(n, nthread);

  std::vector<double> v0(n);
  std::vector<double> v1(n);

  for (unsigned int i = 0; i < v0.size(); ++i) v0[i] = i+1.0;


  for (auto _ : state) {
    std::vector<double>& p0 = v0;
    std::vector<double>& p1 = v1;
    for (unsigned int iter = 0; iter < 20; ++iter) {
    while (true) {
      std::vector<std::thread> threads;
      std::vector<std::future<bool>> futures;
      for (unsigned int i = 0; i < nthread; ++i) {
        std::promise<bool> promise;
        futures.emplace_back(promise.get_future());
        threads.emplace_back(my_fun_promise, std::ref(p0), chunks[i], chunks[i+1], 
          std::ref(p1), std::move(promise));
      }
      bool result = true;
      for (unsigned int i = 0; i < nthread; ++i) {
        result &= futures[i].get();
        threads[i].join();
      }
      if (result) break;
      std::swap(p0, p1);
    }
  }
  }


  //double max = 0;
  //for (const auto& v: result) max = std::max(max, v);
  //std::cout << "Max = " << max << "\n";
}


static void BM_async(benchmark::State& state) {
  unsigned int n = state.range(0);
  unsigned int nthread = state.range(1);

  auto chunks = chunk(n, nthread);

  std::vector<double> v0(n);
  std::vector<double> v1(n);
  for (unsigned int i = 0; i < v0.size(); ++i) v0[i] = i+1.0;


  for (auto _ : state) {
    std::vector<double>& p0 = v0;
    std::vector<double>& p1 = v1;
    for (unsigned int iter = 0; iter < 20; ++iter) {
    while (true) {
      std::vector<std::future<bool>> futures;
      for (unsigned int i = 0; i < nthread; ++i) {
        futures.emplace_back(std::async(std::launch::async, my_fun, std::ref(p0), chunks[i], chunks[i+1], 
          std::ref(p1)));
      }
      bool result = true;
      for (unsigned int i = 0; i < nthread; ++i) {
        result &= futures[i].get();
      }
      if (result) break;
      std::swap(p0, p1);
    }
  }
  }
}

static void BM_single(benchmark::State& state) {
  unsigned int n = state.range(0);

  std::vector<double> v0(n);
  std::vector<double> v1(n);
  for (unsigned int i = 0; i < v0.size(); ++i) v0[i] = i+1.0;

  for (auto _ : state) {
    std::vector<double>& p0 = v0;
    std::vector<double>& p1 = v1;
    for (unsigned int iter = 0; iter < 20; ++iter) {
    while (true) {
      bool result = my_fun(p0, 0, p0.size(), p1);
      if (result) break;
      std::swap(p0, p1);
    }
  }
  }
}

static unsigned int g_seed = 123;
inline int fastrand() {
  g_seed = (214013*g_seed+2531011);
  return (g_seed>>16)&0x7FFF;
}

double runif() {
  return (double)(fastrand()%32768)/(double)32768;
}


static void BM_threaded_nn1(benchmark::State& state) {
  //unsigned int n = 1000000;
  //unsigned int nthread = 4;

  unsigned int n = state.range(0);
  unsigned int nthread = state.range(1);

  auto chunks = chunk(n, nthread);

  std::vector<double> x(n);
  std::vector<double> y(n);
  std::vector<size_t> result(n);

  for (unsigned int i = 0; i < n; ++i) {
    x[i] = runif();
    y[i] = runif();
  }

  for (auto _ : state) {
    std::vector<std::thread> threads;
    std::vector<std::future<bool>> futures;
    for (unsigned int i = 0; i < nthread; ++i) {
      std::promise<bool> promise;
      futures.emplace_back(promise.get_future());
      threads.emplace_back(nn1_promise, std::ref(x), std::ref(y), chunks[i], chunks[i+1], 
        std::ref(result), std::move(promise));
    }
    bool result = true;
    for (unsigned int i = 0; i < nthread; ++i) {
      result &= futures[i].get();
      threads[i].join();
    }
  }
  //double max = 0;
  //for (const auto& v: result) max = std::max(max, v);
  //std::cout << "Max = " << max << "\n";
}


static void BM_threaded_nn1_nofuture(benchmark::State& state) {
  //unsigned int n = 1000000;
  //unsigned int nthread = 4;

  unsigned int n = state.range(0);
  unsigned int nthread = state.range(1);

  auto chunks = chunk(n, nthread);

  std::vector<double> x(n);
  std::vector<double> y(n);
  std::vector<size_t> result(n);

  for (unsigned int i = 0; i < n; ++i) {
    x[i] = runif();
    y[i] = runif();
  }

  for (auto _ : state) {
    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < nthread; ++i) {
      threads.emplace_back(nn1, std::ref(x), std::ref(y), chunks[i], chunks[i+1], 
        std::ref(result));
    }
    for (unsigned int i = 0; i < nthread; ++i) {
      threads[i].join();
    }
  }
}



BENCHMARK(BM_threaded_nn1)->Args({10000, 1})->Args({10000, 2})->Args({10000, 3})->Args({10000, 4})->UseRealTime();
BENCHMARK(BM_threaded_nn1_nofuture)->Args({10000, 1})->Args({10000, 2})->Args({10000, 3})->Args({10000, 4})->UseRealTime();




BENCHMARK_MAIN();


/*
int main(int argc, char* argv[]) {
  unsigned int n = 1000000;
  unsigned int nthread = 4;

  auto chunks = chunk(n, nthread);
  for (auto& v: chunks) std::cout << v << ";";
  std::cout << "\n";

  std::vector<double> v0(n);
  std::vector<double> v1(n);

  for (unsigned int i = 0; i < v0.size(); ++i) v0[i] = i+1.0;

  //std::vector<double>& p0 = v0;
  //std::vector<double>& p1 = v1;
  //while (!my_fun(p0, 0, v0.size(), p1)) {
  //  my_fun(p0, 0, v0.size(), p1);
  //  std::swap(p0, p1);
  //}
  //// Final result is in p1;
  //std::vector<double>& result = p1;

  //std::promise<bool> promise;
  //std::future<bool> future = promise.get_future();
  //std::thread thread(my_fun_promise, std::ref(v0), 0, v0.size(), std::ref(v1), std::move(promise));
  //std::cout << "Thread result = " << future.get() << "\n";
  //thread.join();

  std::vector<double>& p0 = v0;
  std::vector<double>& p1 = v1;
  while (true) {
    std::vector<std::thread> threads;
    std::vector<std::future<bool>> futures;
    for (unsigned int i = 0; i < nthread; ++i) {
      std::promise<bool> promise;
      futures.emplace_back(promise.get_future());
      threads.emplace_back(my_fun_promise, std::ref(p0), 0, v0.size(), std::ref(p1), 
        std::move(promise));
    }
    bool result = true;
    for (unsigned int i = 0; i < nthread; ++i) {
      result &= futures[i].get();
      threads[i].join();
    }
    if (result) break;
    std::swap(p0, p1);
  }
  // Final result is in p1;
  std::vector<double>& result = p1;


  double max = 0;
  for (const auto& v: result) max = std::max(max, v);
  std::cout << "Max = " << max << "\n";
  return 0;
}

*/
