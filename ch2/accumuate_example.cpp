#include <thread>

template<typename Iterator, typename T>
struct accumulate_block {
    void operator()(Iterator start, Iterator last, T& result) {
        std::accumulate(start, last, result);
    }
};

template<typename Iterator, typename T>
struct accumulate_parallel {
    void operator()(Iterator first, Iterator last, T init) {
        unsigned long const length = std::distance(first, last);
        if(!length) {
            return init;
        }
        unsigned long const min_per_thread=25;
        unsigned long const max_threads = (length + min_per_thread - 1)/min_per_thread;
        unsigned long const hardware_threads = std::thread::hardware_concurrency();

        unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
        unsigned long const block_size = length / num_threads;
        std::vector<T> results(num_threads);
        std::vector<std::thread> threads(num_threads - 1); // why - 1? A: could be less than block size, so handled in the main thread
        Iterator block_start = first;
        for(unsigned long i = 0; i < (num_threads - 1); ++i) {
            Iterator block_end = block_start;
            std::advance(block_end, block_size);
            threads[i] = std::thread(
                accumulate_block<Iterator, T>(),
                block_start, block_end, std::ref(results[i])
            );
            block_start = block_end;
        }

        accumulate_block<Iterator, T>()( // accumulate_block here has 2 sets of parentheses, because the first set creates a temporary which is then called with the 2nd set
            block_start, last, resuts[num_threads - 1]
        );

        for(auto& entry : threads) {
            entry.join();
        }
        return std::accumulate(results.begin(), results.end(), init);
    }
};