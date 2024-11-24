#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// docker run --rm -it -v .:/plagiarism_checker plagiarism_checker

// TODO: Implement the methods of the plagiarism_checker_t class
// typedef uint64_t ll;
// typedef __uint128_t vl;

#define BIG_CONSTANT(x) (x##LLU)

uint64_t MurmurHash64A ( const void * key, int len, uint64_t seed )
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

void winnowing(int w, std::vector<ll> & submission,std::vector<ll> & fingerprints){
    int minPos = -1;

    for(int i = 0; i <= submission.size() - w; i++){
        int localMinPos = i;
        for(int j = 1; j < w; j++){
            if(submission[i + j] <= submission[localMinPos]){
                localMinPos = i + j;
            }
        }

        if(localMinPos != minPos){
            minPos = localMinPos;
            fingerprints.push_back(submission[minPos]);
        }
    }
    
}

void hashing(std::vector<int> & submission,std::unordered_set<ll> & hash_map,std::vector<ll> &hash,int chunk){
    int length=submission.size();
    // std::cout<<"surya "<<length<<" "<<chunk<<std::endl;
    if(chunk>length) return;
    hash=std::vector<ll>(length-chunk+1);
    for(int i=0;i<length-chunk+1;i++){
        if(chunk==15){
            int a[15];
            for(int j=0;j<chunk;j++){
                a[j]=submission[i+j];
            }
                hash[i]=MurmurHash64A(a,chunk*4,98765);
        }
        if(chunk==75){
            int a[75];
            for(int j=0;j<chunk;j++){
                a[j]=submission[i+j];
            }
                hash[i]=MurmurHash64A(a,chunk*4,98765);
        }
    }
    // int prime=31;
    // ll modulo=(1LL<<61)-1;

    // ll curr=0;
    // ll pow=1;
    // for(int i=chunk-1;i>=0;i--){
    //     curr=(curr+((vl)submission[i]*(vl)(pow))%modulo)%modulo;
    //     if(i>0) pow=((vl)pow*(vl)prime)%modulo;
    // }
    // hash[0]=curr;
    // // if(hash[0]<0) std::cout<<"surya"<<std::endl;
    // for(int i=1;i<=length-chunk;i++){
    //     hash[i]=hash[i-1];
    //     hash[i]=(hash[i]-((vl)submission[i-1]*(vl)pow)%modulo+modulo)%modulo;
    //     hash[i]=((vl)prime*(vl)hash[i])%modulo;
    //     hash[i]=(hash[i]+(ll)submission[i+chunk-1])%modulo;
    //     // if(hash[i]<0) std::cout<<"surya"<<std::endl;
    // }
    std::vector<ll> winnowed_hash;
    winnowing(4,hash,winnowed_hash);
    hash=winnowed_hash;
    for(ll x : hash){
        // std::cout<<x<<" ";
        hash_map.insert(x);
    }
    // std::cout<<std::endl;
    return;
}
plagiarism_checker_t::plagiarism_checker_t(void){
    
}
plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> 
                            __submissions){
    for(auto i : __submissions){
        timestamp[i]=std::chrono::time_point<std::chrono::high_resolution_clock>{};
        tokenizer_t tokenizer(i->codefile);
        std::vector<int> tokens=tokenizer.get_tokens();
        // database_1[i]=tokens;
        std::vector<ll> dummy;
        prev_tokens[i]=tokens;
        hashing(tokens,database,dummy,15);
        // std::cout<<tokens.size()<<" "<<database_1[i].size()<<" "<<database_2[i].size()<<std::endl;
        hashing(tokens,database_large,dummy,75);
        // flagged[i]=false;
    }
    stop=false;
    worker = std::thread(&plagiarism_checker_t::worker_thread, this);
    // std::cout<<__submissions.size()<<" "<<timestamp.size()<<" "<<database.size()<<std::endl;
}

plagiarism_checker_t::~plagiarism_checker_t(void){
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        stop = true; // Signal the thread to stop
    }
    cv.notify_one(); // Wake up the worker thread
    if (worker.joinable()) {
        worker.join(); // Wait for the thread to finish
    }

    // // if(threads.joinable()) threads.join();
    
    timestamp.clear();
    database.clear();
    // std::cout<<"destructor "<<std::chrono::high_resolution_clock::now()<<std::endl;
    // database_2.clear();
}
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
void plagiarism_checker_t::add_task(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        task_queue.push(std::move(task));
    }
    cv.notify_one(); // Notify the worker thread of the new task
    // worker.detach();
}

// int flagging(std::vector<int> & submission1,std::vector<int> & submission2){
//     int l1=submission1.size();
//     int l2=submission2.size();
//     // std::cout<<l1<<" "<<l2<<std::endl;
//     // submission1=std::vector<int> (10000);
//     // submission2=std::vector<int> (10000);
//     // for(int i=0;i<10000;i++){
//     //     submission1[i]=i+1;
//     //     submission2[i]=i+10001;
//     // }
//     std::vector<int> counted_index(l2,0);
//     int result=0;
//     for(int i=0;i<l1;i++){
//         // position and max matched length for this index i in the second vector
//         int pos_j=-1;
//         int max_length=0; 

//         for(int j=0;j<l2;j++){
//             // vars to store initial values
//             int pi=i,pj=j; 
//             int length=0;
//             while(i<l1 && j<l2 && (submission1[i]==submission2[j])){
//                 length++;
//                 i++;
//                 j++;
//             }
//             if(length>max_length){
//                 pos_j=pj;
//                 max_length=length;
//             }
//             if(length>=75){
//                 // std::cout<<"here"<<std::endl;
//                 return 1;
//             }
//             // if(length>=15) result++;

//             // if(result>=10) return 1;
//             i=pi;
//             j=pj;
//             // if(length>0) std::cout<<"length "<<length<<std::endl;
//         }
//         // if(max_length>=75) return 1;
//         if(max_length>=15){
//             // std::cout<<"surya"<<std::endl;
//             for(int a=i,b=pos_j;a<i+max_length && b<pos_j+max_length;a++,b++){
//                 if(counted_index[b]==0){
//                     result++;
//                     counted_index[b]=1;
//                 }
//             }
//             // result++;
//             // we are not visiting the indices of first vector which are already counted above
//             i=i+max_length-1;
//         } 
//         if(result>=150){
//             std::cout<<"result="<<result<<std::endl;
//             return 1;
//         }
//     }
//     return 0;

//     // std::unordered_map<ll,int> hash_1,hash_2;
//     // if(l1>=75 && l2>=75){
//     //     hashing(submission1,hash_1,75);
//     //     hashing(submission2,hash_2,75);
//     //     for(auto i : hash_1){
//     //         if(hash_2.contains(i.first)){
//     //             // std::cout<<i.first<<" "<<hash_1[i.first]<<" "<<hash_2[i.first]<<std::endl;
//     //             // std::cout<<"here"<<std::endl;
//     //             return 1;
//     //         }
//     //     }
//     // }
//     // hash_1.clear();
//     // hash_2.clear();
//     // hashing(submission1,hash_1,15);
//     // hashing(submission2,hash_2,15);
//     // int count=0;
//     // for(auto i : hash_1){
//     //     if(hash_2.contains(i.first)){
//     //         // std::cout<<"atleast once"<<std::endl;
//     //         count+=hash_1[i.first]*hash_2[i.first];
//     //     }
//     //     if(count>=10){
//     //         // std::cout<<"there"<<std::endl;
//     //         return 1;
//     //     }
//     // }
//     // // std::cout<<count<<std::endl;
//     // return 0;
// }
void hash_text(std::vector<int> & text,std::map<ll, std::vector<int>> & hash_map)
{
    int length=text.size();
    int chunk=15;
    

    // int n = text.size();
    // int p = 5381, d = 33;
    // int h = 1;

    // for (int i = 0; i < 10 - 1; i++) h = (h * d) % p;

    // int t = 0;
    // for (int i = 0; i < 10; i++) t = (d * t + text[i]) % p;

    // if (hash_map.find(t) == hash_map.end()) hash_map[t] = {0};
    // else hash_map[t].push_back(0);

    // // Uses rolling hash function for hash computation efficiency
    // for (int i = 1; i < n - 10; i++){
    //     t = (d * (t - text[i - 1] * h) + text[i + 9]) % p;
    //     if (t < 0) t = t + p;
    //     if (hash_map.find(t) == hash_map.end()) hash_map[t] = {i};
    //     else hash_map[t].push_back(i);
    // }

    // return hash_map;

    for(int i=0;i<length-chunk+1;i++){
        int a[15];
        for(int j=0;j<chunk;j++){
            a[j]=text[i+j];
        }
        hash_map[MurmurHash64A(a,chunk*4,98765)].push_back(i);

    }
}

// returns hash values of the pattern
// Hash computation is inspired from GeeksForGeeks - "https://www.geeksforgeeks.org/rabin-karp-algorithm-for-pattern-searching/"
void hash_pattern(std::vector<int> &pattern,std::vector<ll> &pattern_hash)
{
    // int k=10;
    // int m = pattern.size();
    // int h = 1;
    // int p = 5381, d = 33;
    // std::vector<int> hashes(m - k + 1);

    // for (int i = 0; i < 10 - 1; i++) h = (h * d) % p;

    // int t = 0;
    // for (int i = 0; i < 10; i++) t = (d * t + pattern[i]) % p;

    // hashes[0] = t;
    // // Uses rolling hash function for hash computation efficiency
    // for (int i = 1; i < m - 10; i++){
    //     t = (d * (t - pattern[i - 1] * h) + pattern[i + 9]) % p;
    //     if (t < 0)
    //         t = t + p;
    //     hashes[i] = t;
    // }
    // // std::vector<int> winnowed_hash;
    // // winnowing(4,hashes,winnowed_hash);
    // // hashes=winnowed_hash;
    // return hashes;
    int length=pattern.size();
    int chunk=15;
    pattern_hash=std::vector<ll> (length-chunk+1);
    for(int i=0;i<length-chunk+1;i++){
        int a[15];
        for(int j=0;j<chunk;j++){
            a[j]=pattern[i+j];
        }
        pattern_hash[i]=MurmurHash64A(a,chunk*4,98765);
    }
}

// returns the length of the exact match
int flagging(std::vector<int> &text, std::vector<int> &pattern)
{
    // std::cout<<"surya"<<std::endl;
    int k=15;
    // calls the hash functions
    std::map<ll, std::vector<int>> text_hash;
    hash_text(text,text_hash);
    std::vector<ll> pattern_hash ;
    hash_pattern(pattern,pattern_hash);
    int p = pattern.size();
    int t = text.size();

    std::vector<bool> visited(t, false);

    ll hashp;
    int total_length = 0;

    // iterates over the pattern index by index if there is no match
    for (int i = 0; i < p - k +1; i++)
    {   
        // matches the hash value of pattern and text
        hashp = pattern_hash[i];
        if (text_hash.find(hashp) != text_hash.end())
        {
            // std::cout<<"surya"<<std::endl;
            std::vector<int> indices = text_hash[hashp];
            int max = k;
            int index = -1;

            // runs for maximum match length over each match of hash value
            for (auto j : indices)
            {
                int len = k;
                // bool ismatch = true;

                // // checks for feasible matches (considering overlap conditions)
                // for (int z = 0; z < 15; z++)
                // {
                //     if (text[j + z] != pattern[i + z]) ismatch = false;
                //     // if (visited[j + z] == true) ismatch = false;
                //     if(ismatch == false) break;
                // }
                
                // if match is feasible, checks for further indices 
                if(true){
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
            }
            // Updates if the feasible match has maximum length and adds to the total_length
            // if (index != -1)
            // {
                int z=0;
                for (int y = 0; y < max; y++){
                    if(visited[index+y]) z++;
                    visited[index + y] = true;
                }
                if(max>=75) return 1;
                total_length = total_length + max-z;
                i = i + max - 1;
            // }
        }
    }
    std::cout<<"matched_length="<<total_length<<std::endl;
    if(total_length>=130) return 1;
    return 0;
}
// int flagging(std::unordered_map<ll,int> ld11,std::unordered_map<ll,int> ld12,std::unordered_map<ll,int> ld21, std::unordered_map<ll,int> ld22){
    
//     if(!ld11.empty() && !ld21.empty()){
//         for(auto i : ld11){
//             if(ld21.contains(i.first)){
//                 // std::cout<<i.first<<" "<<hash_1[i.first]<<" "<<hash_2[i.first]<<std::endl;
//                 // std::cout<<"here"<<std::endl;
//                 return 1;
//             }
//         }
//     }

//     int count=0;
//     for(auto i : ld12){
//         if(ld22.contains(i.first)){
//             // std::cout<<"atleast once"<<std::endl;
//             count+=ld12[i.first]*ld22[i.first];
//             // std::cout<<count<<std::endl;
//         }
//         if(count>=10){
//             // std::cout<<"there "<<count<<std::endl;
//             return 1;
//         }
//     }

//     return 0;
// }
ll new_hashing(std::vector<int> & pattern){
    int prime=31;
    ll modulo=(1LL<<61)-1;
    ll hv=0;
    ll pow=1;
    for(int x : pattern){
        hv=(hv+((vl)pow*(vl)x)%modulo)%modulo;
        pow=((vl)pow*(vl)prime)%modulo;
    }
    return hv;
}
int plagiarism_checker_t::patch_check(std::vector<int>& tokens){
    std::vector<ll> hash;
    
    std::unordered_set<ll> dummy;
    hashing(tokens,dummy,hash,15);
    std::unordered_set<ll> patterns_matched;
    int match_length=0;
    int l=0;
    int start=0;
    std::unordered_set<ll> visited_hashes;
    std::vector<ll> visited_hash;
    int chunk=15;
    for(int i=0;i<hash.size();i++){
        
        if(database.contains(hash[i]) && (!(visited_hashes.contains(hash[i])))){
            l++;
            visited_hash.push_back(hash[i]);
        }
        else if(l!=0){
            match_length+=l+chunk-1;
            for(int x : visited_hash) visited_hashes.insert(x);
            visited_hash.clear();
            l=0;
        }
    }
    if(l!=0){
        match_length+=l+chunk-1;
    }
    if(match_length>=300) return 1;

    std::vector<ll> hash_large;
    hashing(tokens,dummy,hash_large,75);
    for(int x : hash_large){
        if(database_large.contains(x)) return 1;
    }

    return 0;
}

void plagiarism_checker_t::check_plagiarism(std::shared_ptr<submission_t> __submission){
    std::vector<int> tokens=tokenizer_t(__submission->codefile).get_tokens();
    std::unordered_map<std::shared_ptr<submission_t>, std::chrono::time_point<std::chrono::high_resolution_clock>> local_timestamp;
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        local_timestamp = timestamp;
    }

    if(patch_check(tokens)==1){
        __submission->student->flag_student(__submission);
        __submission->professor->flag_professor(__submission);
        flagged[__submission]=true;
    }

    while((!one_sec.empty()) && (std::chrono::duration_cast<std::chrono::milliseconds>(local_timestamp[__submission]-local_timestamp[one_sec.front().first]).count()>1000)){
        prev_tokens[one_sec.front().first]=one_sec.front().second;
        one_sec.pop();
    }
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
    for(auto i : prev_tokens){
            if((!flagged[__submission]) && (flagging(i.second,tokens)==1)){
                __submission->student->flag_student(__submission);
                __submission->professor->flag_professor(__submission);
                flagged[__submission]=true;
            }
    }

    one_sec.push(std::make_pair(__submission,tokens));
    std::vector<ll> dummy;
    hashing(tokens,database,dummy,15);
    hashing(tokens,database_large,dummy,75);
}

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        timestamp[__submission]=std::chrono::high_resolution_clock::now();
    }
    add_task([this, __submission]() { plagiarism_checker_t::check_plagiarism(__submission); });
}

// End TODO