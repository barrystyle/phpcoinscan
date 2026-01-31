#include "db.h"

const int read_threads{3};
std::vector<std::string> balances;
ConcurrentMap addressMap;

void worker(int thr_id, int start, int end)
{
    printf("thread %d (%d->%d)\n", thr_id, start, end);

    int position = start;
    while (position < end) {
        uint64_t val;
        std::string key, vals;
        std::stringstream ss(balances[position]);
        std::getline(ss, key, ' ');
        std::getline(ss, vals, ' ');
        val = strtol(vals.c_str(), NULL, 10); 
        addressMap.write(key, val);
        ++position;
    }

    printf("thread %d exit\n", thr_id);
}

    
void initdb()
{
    std::ifstream file("blockchair_bitcoin_addresses_and_balance_LATEST.tsv");

    Timer leg1, leg2;

    //read into vec
    leg1.start();
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
         balances.push_back(line);
    }
    int linec = balances.size();
    leg1.stop();
    printf("read %d entries into vector in %ldms\n", linec, leg1.between_milliseconds());

    //deploy threads
    leg2.start();
    int threads = 0;
    int start_offset = 0;
    int work_size = linec / read_threads;
    std::vector<std::thread> work_threads;
    while (true) {
        if (threads == read_threads) break;
        work_threads.push_back(std::thread(&worker, threads, start_offset, threads == read_threads-1 ? linec : start_offset+work_size));
        start_offset += (work_size + 1);
        threads++;
    }
    for (int i=0; i<threads; i++) {
         work_threads[i].join();
    }
    leg2.stop();
    printf("wrote %d entries into map in %ldms\n", linec, leg2.between_milliseconds());

    balances.clear();
}

uint64_t searchaddress(std::string address)
{
    return addressMap.read(address);
}

