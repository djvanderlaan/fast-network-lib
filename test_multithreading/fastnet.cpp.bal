#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>


#include <thread>
#include <future>


bool my_fun(const std::vector<double>& v0, unsigned int i0, unsigned int iend, 
    std::vector<double>& v1) {
  double max_val = 0.0;
  for (unsigned int i = i0; i < iend; ++i) {
    const double v = std::sqrt(v0[i]); 
    max_val = std::max(max_val, std::abs(v - 1.0));
    v1[i] = v;
  }
  //std::cout << "max_val=" << max_val << "\n";
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


int main(int argc, char* argv[]) {
  unsigned int n = 1000;
  unsigned int nthread = 2;

  auto chunks = chunk(n, nthread);
  for (auto& v: chunks) std::cout << v << ";";
  std::cout << "\n";

  std::vector<double> v0(n);
  std::vector<double> v1(n);

  for (unsigned int i = 0; i < v0.size(); ++i) v0[i] = i+1.0;

  /*
  std::vector<double>& p0 = v0;
  std::vector<double>& p1 = v1;
  while (!my_fun(p0, 0, v0.size(), p1)) {
    my_fun(p0, 0, v0.size(), p1);
    std::swap(p0, p1);
  }
  // Final result is in p1;
  std::vector<double>& result = p1;
  */

  //std::promise<bool> promise;
  //std::future<bool> future = promise.get_future();
  //std::thread thread(my_fun_promise, std::ref(v0), 0, v0.size(), std::ref(v1), std::move(promise));
  //std::cout << "Thread result = " << future.get() << "\n";
  //thread.join();

  std::vector<double>& p0 = v0;
  std::vector<double>& p1 = v1;
  while (true) {
    std::vector<std::thread> threads;
    std::vector<std::promise<bool>> promises;
    std::vector<std::future<bool>> futures;
    for (unsigned int i = 0; i < nthreads; ++i) {
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();
    std::thread thread(my_fun_promise, std::ref(p0), 0, v0.size(), std::ref(p1), std::move(promise));
      
    }

    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();
    std::thread thread(my_fun_promise, std::ref(p0), 0, v0.size(), std::ref(p1), std::move(promise));
    bool r = future.get();
    thread.join();
    if (r) break;
    std::swap(p0, p1);
  }
  // Final result is in p1;
  std::vector<double>& result = p1;


  //for (const auto& v: result) std::cout << v << "\n";
  return 0;
}

