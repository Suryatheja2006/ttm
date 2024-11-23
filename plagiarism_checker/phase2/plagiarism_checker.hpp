#include "structures.hpp"
// -----------------------------------------------------------------------------
#include<thread>
#include<chrono>
#include<mutex>
#include<unordered_map>
#include<unordered_set>
#include<map>
#include <queue>
#include <condition_variable>
#include <functional>
#include <memory>
typedef long long ll;
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here

class plagiarism_checker_t : public std::enable_shared_from_this<plagiarism_checker_t>{
    // You should NOT modify the public interface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> 
                            __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
    std::unordered_map<std::shared_ptr<submission_t>,bool> flagged;
    // std::unordered_map<std::shared_ptr<submission_t>,std::vector<int> > database_1;
    // std::unordered_map<std::shared_ptr<submission_t>,std::unordered_map<ll,int> > database_2;
    std::unordered_set<ll> database;
    std::unordered_set<ll> database_large;
    std::queue<std::pair<std::shared_ptr<submission_t>,std::vector<int> > > one_sec;
    std::unordered_map<std::shared_ptr<submission_t>, std::chrono::time_point<std::chrono::high_resolution_clock>> timestamp;
    // std::mutex db_mutex;
    void check_plagiarism(std::shared_ptr<submission_t> __submission,std::vector<int> tokens);
    int patch_check(std::vector<int> & tokens);
    // std::thread threads;

    std::thread worker; // Single worker thread
    std::queue<std::function<void()>> task_queue; // Task queue
    std::mutex queue_mutex; // Protects the task queue
    std::condition_variable cv; // For thread synchronization
    bool stop; // Flag to signal the thread to stop
    void add_task(std::function<void()> task); // Add a task to the queue
    void worker_thread(); // Worker thread function 

    
    // End TODO
};
