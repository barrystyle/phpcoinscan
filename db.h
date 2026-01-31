#ifndef DB_H
#define DB_H

#include <map>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>
#include <shared_mutex>
#include <unordered_map>

#include <timer.h>

class ConcurrentMap {
public:
int read(const std::string &key) {
  auto it = map.find(key);
  if (it != map.end()) {
    return it->second;
  }
  return {};
}

void write(const std::string &key, const int&value) {
  std::unique_lock<std::shared_mutex> lock(m_mutex);
  map[key] = value;
}

size_t size() const {
  std::shared_lock<std::shared_mutex> lock(m_mutex);
  return map.size();
}

private:
  std::unordered_map<std::string, int> map;
  mutable std::shared_mutex m_mutex;
};

void initdb();
uint64_t searchaddress(std::string address);

#endif // DB_H
