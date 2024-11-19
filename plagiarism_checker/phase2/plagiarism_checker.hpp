#include "structures.hpp"
// -----------------------------------------------------------------------------
#include<thread>
#include<chrono>
#include<mutex>
#include<unordered_map>
#include<map>
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
    std::unordered_map<std::shared_ptr<submission_t>,std::vector<int> > database;
    std::unordered_map<std::shared_ptr<submission_t>, time_t> timestamp;
    std::mutex db_mutex;
    void check_plagiarism(std::shared_ptr<submission_t> __submission);
    std::shared_ptr<plagiarism_checker_t> shared_from_this();
    std::thread threads;
    // End TODO
};
