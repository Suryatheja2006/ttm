#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
typedef long long ll;
typedef __int128_t vl;

plagiarism_checker_t::plagiarism_checker_t(void){
    
}
plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> 
                            __submissions){
    for(auto i : __submissions){
        timestamp[i]=0;
        tokenizer_t tokenizer(i->codefile);
        database[i]=tokenizer.get_tokens();
    }
    // std::cout<<__submissions.size()<<" "<<timestamp.size()<<" "<<database.size()<<std::endl;
}

plagiarism_checker_t::~plagiarism_checker_t(void){
    timestamp.clear();
    database.clear();
}

// void hashing(std::vector<int> & submission,std::map<ll,int> & hash_map,int chunk){
//     int length=submission.size();
//     // std::cout<<"surya "<<length<<" "<<chunk<<std::endl;
//     std::vector<ll> hash(length-chunk+1);
//     int prime=31;
//     ll modulo=(1LL<<61)-1;

//     ll curr=0;
//     ll pow=1;
//     for(int i=chunk-1;i>=0;i--){
//         curr=(curr+((vl)submission[i]*(vl)(pow))%modulo)%modulo;
//         if(i>0) pow=((vl)pow*(vl)prime)%modulo;
//     }
//     hash[0]=curr;
//     // if(hash[0]<0) std::cout<<"surya"<<std::endl;
//     for(int i=1;i<=length-chunk;i++){
//         hash[i]=hash[i-1];
//         hash[i]=(hash[i]-((vl)submission[i-1]*(vl)pow)%modulo+modulo)%modulo;
//         hash[i]=((vl)prime*(vl)hash[i])%modulo;
//         hash[i]=(hash[i]+(ll)submission[i+chunk-1])%modulo;
//         // if(hash[i]<0) std::cout<<"surya"<<std::endl;
//     }
//     for(ll x : hash){
//         // std::cout<<x<<" ";
//         hash_map[x]++;
//     }
//     // std::cout<<std::endl;
//     return;
// }

int flag(std::vector<int> & submission1,std::vector<int> & submission2){
    int l1=submission1.size();
    int l2=submission2.size();
    // std::cout<<l1<<" "<<l2<<std::endl;
    // submission1=std::vector<int> (10000);
    // submission2=std::vector<int> (10000);
    // for(int i=0;i<10000;i++){
    //     submission1[i]=i+1;
    //     submission2[i]=i+10001;
    // }
    // std::vector<int> counted_index(l2,0);
    int result=0;
    for(int i=0;i<l1;i++){
        // position and max matched length for this index i in the second vector
        // int pos_j=-1;
        // int max_length=0; 
        for(int j=0;j<l2;j++){
            // vars to store initial values
            int pi=i,pj=j; 
            int length=0;
            while(i<l1 && j<l2 && (submission1[i]==submission2[j])){
                length++;
                i++;
                j++;
            }
            // if(length>max_length){
            //     // pos_j=pj;
            //     max_length=length;
            // }
            if(length>=75) return 1;
            if(length>=15) result++;

            if(result>=10) return 1;
            i=pi;
        }
        // if(max_length>=75) return 1;
        // if(max_length>=15){
        //     // for(int a=i,b=pos_j;a<i+max_length && b<pos_j+max_length;a++,b++){
        //     //     if(counted_index[b]==0){
        //     //         result++;
        //     //         counted_index[b]=1;
        //     //     }
        //     // }
        //     result++;
        //     // we are not visiting the indices of first vector which are already counted above
        //     // i=i+max_length-1;
        // }
    }
    return 0;

    // std::map<ll,int> hash_1,hash_2;
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

void check_plagiarism(std::unordered_map<std::shared_ptr<submission_t>,std::vector<int>> database,
                        std::unordered_map<std::shared_ptr<submission_t>, time_t> timestamp,
                        std::shared_ptr<submission_t> __submission){
    // std::cout<<"hello"<<std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // std::cout<<"here at "<<std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())<<std::endl;

    for(auto i : database){
        if((i.first!=__submission) && (timestamp[i.first]<=timestamp[__submission])){
            if(flag(i.second,database[__submission])==1){
                __submission->student->flag_student(__submission);
                __submission->professor->flag_professor(__submission);
                if((timestamp[__submission]-timestamp[i.first])<1){
                    i.first->student->flag_student(i.first);
                    i.first->professor->flag_professor(i.first);
                }
                return;
            }

        }
    }
    // std::cout<<"there"<<std::endl;

}

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    // auto now = std::chrono::system_clock::now();
    // std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    // std::cout << "Current time: " << std::ctime(&now_c);
    timestamp[__submission]=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    database[__submission]=tokenizer_t(__submission->codefile).get_tokens();
    // std::cout<<tokens.size()<<std::endl;

    std::mutex db_mutex;
    std::lock_guard<std::mutex> lock(db_mutex);

    std::thread(&check_plagiarism,database,timestamp,__submission).detach();
    // std::cout<<"here"<<std::endl;
    // std::cout<<"detached at "<<std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())<<std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(3));

}




// End TODO