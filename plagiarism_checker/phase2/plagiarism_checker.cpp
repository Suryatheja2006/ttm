#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// docker run --rm -it -v .:/plagiarism_checker plagiarism_checker

// TODO: Implement the methods of the plagiarism_checker_t class
typedef long long ll;
typedef __int128_t vl;

void winnowing(int w, std::vector<ll> & submission,std::vector<ll> & fingerprints){
    int minPos = -1;

    for(int i = 0; i <= submission.size() - w; i++){
        int localMinPos = i;
        for(int j = 1; j < w; j++){
            if(submission[i + j] < submission[localMinPos]){
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
    int prime=31;
    ll modulo=(1LL<<61)-1;

    ll curr=0;
    ll pow=1;
    for(int i=chunk-1;i>=0;i--){
        curr=(curr+((vl)submission[i]*(vl)(pow))%modulo)%modulo;
        if(i>0) pow=((vl)pow*(vl)prime)%modulo;
    }
    hash[0]=curr;
    // if(hash[0]<0) std::cout<<"surya"<<std::endl;
    for(int i=1;i<=length-chunk;i++){
        hash[i]=hash[i-1];
        hash[i]=(hash[i]-((vl)submission[i-1]*(vl)pow)%modulo+modulo)%modulo;
        hash[i]=((vl)prime*(vl)hash[i])%modulo;
        hash[i]=(hash[i]+(ll)submission[i+chunk-1])%modulo;
        // if(hash[i]<0) std::cout<<"surya"<<std::endl;
    }
    std::vector<ll> winnowed_hash;
    // winnowing(10,hash,winnowed_hash);
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

int flagging(std::vector<int> & submission1,std::vector<int> & submission2){
    int l1=submission1.size();
    int l2=submission2.size();
    // std::cout<<l1<<" "<<l2<<std::endl;
    // submission1=std::vector<int> (10000);
    // submission2=std::vector<int> (10000);
    // for(int i=0;i<10000;i++){
    //     submission1[i]=i+1;
    //     submission2[i]=i+10001;
    // }
    std::vector<int> counted_index(l2,0);
    int result=0;
    for(int i=0;i<l1;i++){
        // position and max matched length for this index i in the second vector
        int pos_j=-1;
        int max_length=0; 

        for(int j=0;j<l2;j++){
            // vars to store initial values
            int pi=i,pj=j; 
            int length=0;
            while(i<l1 && j<l2 && (submission1[i]==submission2[j])){
                length++;
                i++;
                j++;
            }
            if(length>max_length){
                pos_j=pj;
                max_length=length;
            }
            if(length>=75) return 1;
            // if(length>=15) result++;

            // if(result>=10) return 1;
            i=pi;
            j=pj;
        }
        // if(max_length>=75) return 1;
        if(max_length>=15){
            for(int a=i,b=pos_j;a<i+max_length && b<pos_j+max_length;a++,b++){
                if(counted_index[b]==0){
                    result++;
                    counted_index[b]=1;
                }
            }
            // result++;
            // we are not visiting the indices of first vector which are already counted above
            i=i+max_length-1;
        } 
        if(result>=150) return 1;
    }
    return 0;

    // std::unordered_map<ll,int> hash_1,hash_2;
    // if(l1>=75 && l2>=75){
    //     hashing(submission1,hash_1,75);
    //     hashing(submission2,hash_2,75);
    //     for(auto i : hash_1){
    //         if(hash_2.contains(i.first)){
    //             // std::cout<<i.first<<" "<<hash_1[i.first]<<" "<<hash_2[i.first]<<std::endl;
    //             // std::cout<<"here"<<std::endl;
    //             return 1;
    //         }
    //     }
    // }
    // hash_1.clear();
    // hash_2.clear();
    // hashing(submission1,hash_1,15);
    // hashing(submission2,hash_2,15);
    // int count=0;
    // for(auto i : hash_1){
    //     if(hash_2.contains(i.first)){
    //         // std::cout<<"atleast once"<<std::endl;
    //         count+=hash_1[i.first]*hash_2[i.first];
    //     }
    //     if(count>=10){
    //         // std::cout<<"there"<<std::endl;
    //         return 1;
    //     }
    // }
    // // std::cout<<count<<std::endl;
    // return 0;
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
    // std::cout<<"here "<<tokens.size()<<std::endl;
    hashing(tokens,dummy,hash,15);
    // std::unordered_set<ll> local_database;
    // {

    // }
    std::unordered_set<ll> patterns_matched;
    int match_length=0;
    int l=0;
    int start=0;
    std::vector<int> pattern;
    int chunk=15;
    for(int i=0;i<hash.size();i++){
        // std::cout<<"there "<<i<<" "<<hash.size()<<std::endl;
        
        if(database.contains(hash[i])){
            // std::cout<<"surya"<<std::endl;
            l++;
            if(start==0){
                for(int j=0;j<15;j++) pattern.push_back(tokens[i+j]);
                start=1;
            }
            else pattern.push_back(tokens[i+14]);
        }
        else if(l!=0){
            ll hv=new_hashing(pattern);
            if(!patterns_matched.contains(hv)) match_length+=l+chunk-1;

            patterns_matched.insert(hv);
            l=0;
            start=0;
            pattern.clear();
        }
    }
    if(l!=0) match_length+=l+chunk-1;
    std::cout<<match_length<<std::endl;
    if(match_length>=0) return 1;

    std::vector<ll> hash_large;
    hashing(tokens,dummy,hash_large,75);
    for(int x : hash_large){
        if(database_large.contains(x)) return 1;
    }

    return 0;


}

void plagiarism_checker_t::check_plagiarism(std::shared_ptr<submission_t> __submission,std::vector<int> tokens){
    // std::cout<<"hello"<<std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // std::cout<<"here at "<<std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())<<std::endl;
    // std::unordered_map<std::shared_ptr<submission_t>,std::vector<int>> local_database_1;
    // std::unordered_map<std::shared_ptr<submission_t>,std::unordered_map<ll,int> > local_database_2;
    std::unordered_map<std::shared_ptr<submission_t>, std::chrono::time_point<std::chrono::high_resolution_clock>> local_timestamp;
    // std::unordered_map<std::shared_ptr<submission_t>,bool> local_flagged;
    {
    //     // Lock to safely copy shared resources
        std::lock_guard<std::mutex> lock(queue_mutex);
        // local_database_1 = database_1;  // Make local copies to reduce lock contention
        // local_database_2 = database_2;
        local_timestamp = timestamp;
        // local_flagged = flagged;
    //     return;
    }
    // cv.notify_one();
    // for(auto i : local_database_1){
    //     if((i.first!=__submission) && (local_timestamp[i.first]<=local_timestamp[__submission])){
    //         if(((!flagged[__submission]) || ((std::chrono::duration_cast<std::chrono::milliseconds>(local_timestamp[__submission]-local_timestamp[i.first]).count()<1000) && (!flagged[i.first]))) && (flagging(i.second,local_database_1[__submission])==1)){
    //             if(!flagged[__submission]){
    //                 // std::cout<<local_timestamp[__submission]<<std::endl;
    //                 __submission->student->flag_student(__submission);
    //                 __submission->professor->flag_professor(__submission);
    //                 flagged[__submission]=true;
    //             }

    //             if(std::chrono::duration_cast<std::chrono::milliseconds>(local_timestamp[__submission]-local_timestamp[i.first]).count()<1000){
    //                 if(!flagged[i.first]){
    //                     // std::cout<<"previous one"<<std::endl;
    //                     i.first->student->flag_student(i.first);
    //                     i.first->professor->flag_professor(i.first);
    //                     flagged[i.first]=true;
    //                 }
    //             }
                
    //         }
    //     }
    // }
    
    // return;

    if(patch_check(tokens)==1){
        __submission->student->flag_student(__submission);
        __submission->professor->flag_professor(__submission);
        flagged[__submission]=true;
    }
    std::cout<<"one_sec size "<<one_sec.size()<<std::endl;

    while((!one_sec.empty()) && (std::chrono::duration_cast<std::chrono::milliseconds>(local_timestamp[__submission]-local_timestamp[one_sec.front().first]).count()>2000)){
        std::cout<<"submission : "<<std::chrono::duration_cast<std::chrono::milliseconds>(local_timestamp[__submission].time_since_epoch()).count()<<std::endl;
        std::cout<<"front : "<<std::chrono::duration_cast<std::chrono::milliseconds>(local_timestamp[one_sec.front().first].time_since_epoch()).count()<<std::endl;
        
        std::cout<<"popping"<<std::endl;
        one_sec.pop();
    }
    std::queue<std::pair<std::shared_ptr<submission_t>,std::vector<int> > > copy=one_sec;
    std::cout<<"one_sec size"<<one_sec.size()<<std::endl;
    while(!copy.empty()){
        if(flagging(tokens,copy.front().second)==1){
            if(!flagged[__submission] || true){
                __submission->student->flag_student(__submission);
                __submission->professor->flag_professor(__submission);
            }
            if(!flagged[copy.front().first] || true){
                std::cout<<"surya"<<std::endl;
                copy.front().first->student->flag_student(copy.front().first);
                copy.front().first->professor->flag_professor(copy.front().first);
            }
        }
        copy.pop();

    }

    one_sec.push(std::make_pair(__submission,tokens));
    // std::cout<<"there"<<std::endl;
    std::vector<ll> dummy;
    hashing(tokens,database,dummy,15);
    hashing(tokens,database_large,dummy,75);
}

// std::shared_ptr<plagiarism_checker_t> plagiarism_checker_t::shared_from_this(){
//     return std::shared_ptr<plagiarism_checker_t>(this);
// }

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    // auto now = std::chrono::system_clock::now();
    // std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    // std::cout << "Current time: " << std::ctime(&now_c);
    {

        std::lock_guard<std::mutex> lock(queue_mutex);
        timestamp[__submission]=std::chrono::high_resolution_clock::now();
        // std::cout<<timestamp[__submission]<<std::endl;
        // hashing(tokens,database_1[__submission],75);
        // database_1[__submission]=tokens;
        // hashing(tokens,database_2[__submission],15);
        // std::cout<<tokens.size()<<" "<<database_1[__submission].size()<<" "<<database_2[__submission].size()<<std::endl;
    }
    std::vector<int> tokens=tokenizer_t(__submission->codefile).get_tokens();
    // cv.notify_one();
    add_task([this, __submission,tokens]() { plagiarism_checker_t::check_plagiarism(__submission,tokens); });

    // plagiarism_checker_t::check_plagiarism(__submission);


    // // std::cout<<tokens.size()<<std::endl;

    
    // if(threads.joinable()) threads.join();
    // threads=std::thread(&plagiarism_checker_t::check_plagiarism,this,__submission);
    // threads.detach();




    // std::cout<<"here"<<std::endl;
    // std::cout<<"detached at "<<std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())<<std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(3));

    // auto self = shared_from_this(); // Create a shared pointer to `this`
    // std::thread([self, __submission]() {
    //     self->check_plagiarism(__submission); // Use shared pointer to keep `this` alive
    // }).detach();
}

// End TODO