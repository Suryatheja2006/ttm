#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// docker run --rm -it -v .:/plagiarism_checker plagiarism_checker

// TODO: Implement the methods of the plagiarism_checker_t class

#define BIG_CONSTANT(x) (x##LLU)

// Murmur hash function used for hashing finite length patterns
uint64_t MurmurHash ( const void * key, int len, uint64_t seed )
{
  const uint64_t m = BIG_CONSTANT(0xc6a4a7935bd1e995);
  const int r = 47;

  uint64_t h = seed ^ (len * m);

  const uint64_t * data = (const uint64_t *)key;
  const uint64_t * end = data + (len/8);

  while(data != end)
  {
    uint64_t k = *data++;

    k *= m; 
    k ^= k >> r; 
    k *= m; 
    
    h ^= k;
    h *= m; 
  }

  const unsigned char * data2 = (const unsigned char*)data;

  switch(len & 7)
  {
  case 7: h ^= uint64_t(data2[6]) << 48;
  case 6: h ^= uint64_t(data2[5]) << 40;
  case 5: h ^= uint64_t(data2[4]) << 32;
  case 4: h ^= uint64_t(data2[3]) << 24;
  case 3: h ^= uint64_t(data2[2]) << 16;
  case 2: h ^= uint64_t(data2[1]) << 8;
  case 1: h ^= uint64_t(data2[0]);
          h *= m;
  };
 
  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

// returns hashes of tokens of a file as a set or a vector. Uses MurmurHash
void hashing(std::vector<int> & submission,std::unordered_set<ll> & hash_set,std::vector<ll> &hash,int chunk){
    int length=submission.size();
    if(chunk>length) return;
    hash=std::vector<ll>(length-chunk+1);
    for(int i=0;i<length-chunk+1;i++){
        if(chunk==15){
            int a[15];
            for(int j=0;j<chunk;j++){
                a[j]=submission[i+j];
            }
            hash[i]=MurmurHash(a,chunk*4,98765);
        }
        if(chunk==75){
            int a[75];
            for(int j=0;j<chunk;j++){
                a[j]=submission[i+j];
            }
            hash[i]=MurmurHash(a,chunk*4,98765);
        }
    }
    for(ll x : hash){
        hash_set.insert(x);
    }
    return;
}

// default constructor
plagiarism_checker_t::plagiarism_checker_t(void){
    stop=false;
    worker = std::thread(&plagiarism_checker_t::worker_thread, this);
}

// update the databases with original files
plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> 
                            __submissions){
    for(auto i : __submissions){
        // give zero time stamp for these files
        timestamp[i]=std::chrono::time_point<std::chrono::high_resolution_clock>{};

        tokenizer_t tokenizer(i->codefile);
        std::vector<int> tokens=tokenizer.get_tokens();

        // update the databases
        prev_tokens[i]=tokens;

        std::vector<ll> dummy; // dummy arg to pass to 'hashing' function
        hashing(tokens,database,dummy,15);
        hashing(tokens,database_large,dummy,75);
    }

    stop=false;
    worker = std::thread(&plagiarism_checker_t::worker_thread, this);
}

// destructor
plagiarism_checker_t::~plagiarism_checker_t(void){
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        stop = true; // Signal the thread to stop
    }
    cv.notify_one(); // Wake up the worker thread
    if (worker.joinable()) {
        worker.join(); // Wait for the thread to finish
    }
}

// function running in the background thread
void plagiarism_checker_t::worker_thread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.wait(lock, [this]() { return stop || !task_queue.empty(); });

            if (stop && task_queue.empty()) {
                return; // Exit the thread if stop is signaled and no tasks remain
            }
            
            task = std::move(task_queue.front());
            task_queue.pop();
        }

        // Execute the task
        task();
    }
}

// Add the task (check_plagiarism) for each incoming file
void plagiarism_checker_t::add_task(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        task_queue.push(std::move(task));
    }
    cv.notify_one(); // Notify the worker thread of the new task
}

// patch work plag checker
int plagiarism_checker_t::patch_check(std::vector<int>& tokens){
    
    // get 15 chunk hashes for the current file
    std::vector<ll> hash;
    std::unordered_set<ll> dummy;
    hashing(tokens,dummy,hash,15);
    
    // total matched pattern length
    int match_length=0;

    // properties of current pattern being analyzed
    int len=0;
    int start=0;

    // stores hashes of patterns matched 
    std::unordered_set<ll> visited_hashes;
    std::vector<ll> current_hash;
    int chunk=15;

    for(int i=0;i<hash.size();i++){
        // consider pattern only till the hashes still not visited
        if(database.contains(hash[i]) && (!(visited_hashes.contains(hash[i])))){
            len++;
            current_hash.push_back(hash[i]);// hashes of current pattern
        }
        else if(len!=0){
            match_length+=len+chunk-1;
            for(int x : current_hash) visited_hashes.insert(x);// update visited hashes

            // reset
            current_hash.clear();
            len=0;
        }
    }
    // last pattern
    if(len!=0){
        match_length+=len+chunk-1;
    }

    // min 20 patterns of 15 length each => total match length >= 300
    if(match_length>=300) return 1;

    // check for long pattern match with any of the previous file

    std::vector<ll> hash_large;
    hashing(tokens,dummy,hash_large,75);

    for(int x : hash_large){
        if(database_large.contains(x)) return 1;
    }

    return 0;
}

// hash the 'text' vector and store the position(s) of each hash
void hash_text(std::vector<int> & text,std::unordered_map<ll, std::vector<int>> & hash_map)
{
    int length=text.size();
    int chunk=15;
    for(int i=0;i<length-chunk+1;i++){
        int a[15];
        for(int j=0;j<chunk;j++){
            a[j]=text[i+j];
        }
        hash_map[MurmurHash(a,chunk*4,98765)].push_back(i);
    }
}

// hash the 'pattern' file and give out vector of its hashes
void hash_pattern(std::vector<int> &pattern,std::vector<ll> &pattern_hash)
{
    int length=pattern.size();
    int chunk=15;
    pattern_hash=std::vector<ll> (length-chunk+1);
    for(int i=0;i<length-chunk+1;i++){
        int a[15];
        for(int j=0;j<chunk;j++){
            a[j]=pattern[i+j];
        }
        pattern_hash[i]=MurmurHash(a,chunk*4,98765);
    }
}

// comparing two files for plagiarism
int flagging(std::vector<int> &text, std::vector<int> &pattern)
{
    int chunk=15; // chunk size

    std::unordered_map<ll, std::vector<int>> text_hash;
    hash_text(text,text_hash);

    std::vector<ll> pattern_hash ;
    hash_pattern(pattern,pattern_hash);

    int p = pattern.size();
    int t = text.size();

    std::vector<bool> visited(t, false);

    ll hashp;
    int total_length = 0;// total matched pattern length
    for (int i = 0; i < p - chunk +1; i++)
    {   
        hashp = pattern_hash[i];
        if (text_hash.find(hashp) != text_hash.end())
        {
            std::vector<int> indices = text_hash[hashp];
            int max = chunk;
            int index = -1;
            for (auto j : indices)
            {
                int len = chunk;
                while ((j + len < t) && (text[j + len] == pattern[i + len]) )
                {
                    len++;
                }

                if ((max <= len))
                {
                    max = len;
                    index = j;
                }
            }

            // z is used to avoid double counting
            int z=0;
            for (int y = 0; y < max; y++){
                if(visited[index+y]) z++;
                visited[index + y] = true;
            }
            if(max>=75) return 1;
            
            total_length = total_length + max-z;
            i = i + max - 1;
        }
    }
    // std::cout<<"matched_length="<<total_length<<std::endl;
    if(total_length>=140) return 1;
    return 0;
}

// main function checking for plagiarism for each submissin
void plagiarism_checker_t::check_plagiarism(std::shared_ptr<submission_t> __submission){

    std::vector<int> tokens=tokenizer_t(__submission->codefile).get_tokens();

    // create local database to avoid clashes with main thread
    std::unordered_map<std::shared_ptr<submission_t>, std::chrono::time_point<std::chrono::high_resolution_clock>> local_timestamp;
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        local_timestamp = timestamp;
    }

    // check for patchwork plagiarism and long pattern match with any of previous file
    if(patch_check(tokens)==1){
        __submission->student->flag_student(__submission);
        __submission->professor->flag_professor(__submission);
        flagged[__submission]=true;
    }

    // remove files that are outside one second range from one_sec and add it to prev_tokens
    while((!one_sec.empty()) && (std::chrono::duration_cast<std::chrono::milliseconds>(local_timestamp[__submission]-local_timestamp[one_sec.front().first]).count()>1000)){
        prev_tokens[one_sec.front().first]=one_sec.front().second;
        one_sec.pop();
    }

    // iterate over each file in one_sec using a copy
    std::queue<std::pair<std::shared_ptr<submission_t>,std::vector<int> > > copy=one_sec;
    while(!copy.empty()){
        if(flagging(tokens,copy.front().second)==1){
            if(!flagged[copy.front().first]){
                copy.front().first->student->flag_student(copy.front().first);
                copy.front().first->professor->flag_professor(copy.front().first);
            }
            if(!flagged[__submission]){
                __submission->student->flag_student(__submission);
                __submission->professor->flag_professor(__submission);
            }
        }
        copy.pop();
    }

    // iterate over all files prev_tokens
    for(auto i : prev_tokens){
        if((!flagged[__submission]) && (flagging(i.second,tokens)==1)){
            __submission->student->flag_student(__submission);
            __submission->professor->flag_professor(__submission);
            flagged[__submission]=true;
        }
    }

    // add current submission to one_sec
    one_sec.push(std::make_pair(__submission,tokens));

    // update the databases with current submission
    std::vector<ll> dummy;
    hashing(tokens,database,dummy,15);
    hashing(tokens,database_large,dummy,75);
}

// notes the timestamp of current submission, add task to task_queue and returns
void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        timestamp[__submission]=std::chrono::high_resolution_clock::now();
    }
    add_task([this, __submission]() { plagiarism_checker_t::check_plagiarism(__submission); });
}

// End TODO