#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------
// #include<map>
#include<algorithm>
#define CHUNK_SIZE 10
#include<unordered_set>
typedef long long ll;
typedef __int128_t vl;
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here
void hashing(std::vector<int> & submission,std::vector<ll> &hash,int chunk){
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
    // std::cout<<std::endl;
    return;
}
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
int total_exact_matched_length(std::vector<int> & submission1,std::vector<int> & submission2){
    int l1=submission1.size();
    int l2=submission2.size();
    std::cout<<l1<<" "<<l2<<std::endl;
    std::vector<ll> hash_1,hash_2;
    int chunk=10;
    hashing(submission1,hash_1,chunk);
    hashing(submission2,hash_2,chunk);

    std::unordered_set<ll> hash;
    for(ll x : hash_1) hash.insert(x);
    
    int result=0;
    int l=0;
    // std::unordered_set<ll> patterns_matched;
    std::unordered_set<ll> visited_hashes;
    // int start=0;
    // std::vector<int> pattern;
    std::vector<ll> visited_hash;
    for(int i=0;i<hash_2.size();i++){
        if(hash.contains(hash_2[i]) && (!(visited_hashes.contains(hash_2[i])))){
            // std::cout<<"surya"<<std::endl;
            l++;
            // if(start==0){
            //     start=1;
            //     for(int j=0;j<10;j++) pattern.push_back(submission2[i+j]);
            // }
            // else pattern.push_back(submission2[i+9]);
            visited_hash.push_back(hash_2[i]);

        }
        else if(l!=0){
            // ll hv=new_hashing(pattern);
            result+=l+chunk-1;
            for(int x : visited_hash) visited_hashes.insert(x);
            // std::cout<<pattern.size()<<std::endl;
            // patterns_matched.insert(hv);
            l=0;
            // start=0;
            // pattern.clear();
            visited_hash.clear();
        }
    }
    if(l!=0){
        // ll hv=new_hashing(pattern);
        // if(!patterns_matched.contains(hv)) result+=l+chunk-1;
        result+=l+chunk-1;
        // patterns_matched.insert(hv);
        l=0;
        // start=0;
        // pattern.clear();
    }
    // for(ll x : patterns_matched) std::cout<<x<<" ";
    // std::cout<<std::endl;
    std::cout<<"result "<<result<<std::endl;    
    return result;
    // std::vector<int> counted_index(l2,0);
    // int result=0;
    // for(int i=0;i<l1;i++){
    //     // position and max matched length for this index i in the second vector
    //     int pos_j=-1,max_length=0; 
    //     for(int j=0;j<l2;j++){
    //         // vars to store initial values
    //         int pi=i,pj=j; 
    //         int length=0;
    //         while(i<l1 && j<l2 && (submission1[i]==submission2[j])){
    //             length++;
    //             i++;
    //             j++;
    //         }
    //         if(length>max_length){
    //             pos_j=pj;
    //             max_length=length;
    //         }
    //         i=pi;
    //     }
    //     if(max_length>=10){
    //         for(int a=i,b=pos_j;a<i+max_length && b<pos_j+max_length;a++,b++){
    //             if(counted_index[b]==0){
    //                 result++;
    //                 counted_index[b]=1;
    //             }
    //         }
    //         // we are not visiting the indices of first vector which are already counted above
    //         i=i+max_length-1;
    //     }
    // }
    // return result;
}

void approx_match_length(std::array<int,5> &result,std::vector<int> &submission1,
                                        std::vector<int> &submission2,bool exchange){
    
    int l1=submission1.size();
    int l2=submission2.size();
    std::vector<std::vector<int>> matching=std::vector<std::vector<int>> 
                                                (l1,std::vector<int> (l2,-1));
    // used code of lcs from https://www.geeksforgeeks.org/longest-common-subsequence-dp-4/
    for(int i=0;i<l1;i++){
        for(int j=0;j<l2;j++){
            if(submission1[i]==submission2[j]){
                if(i==0 || j==0) matching[i][j]=1;
                else matching[i][j]=matching[i-1][j-1]+1;
            }
            else{
                if(i==0 && j==0)  matching[i][j]=0;
                else if(i==0) matching[i][j]=matching[i][j-1];
                else if(j==0) matching[i][j]=matching[i-1][j];
                else matching[i][j]=std::max(matching[i-1][j],matching[i][j-1]);
            }
        }
    }
    // v1 and v2 are the vectors of the indices that match between the given vectors
    // these are the indices that contribute to the matched length at i,j i.e. in matching[i][j]
    std::vector<int> v1,v2;
    int a=l1-1,b=l2-1;
    while(a>=0 && b>=0){
        if(submission1[a]==submission2[b]){
            v1.push_back(a);
            v2.push_back(b);
            a--;
            b--;
        }
        else{
            if((b==0) || (a>0 && matching[a-1][b]>matching[a][b-1])) a--;
            else b--;
        }
    }
    std::reverse(v1.begin(),v1.end());
    std::reverse(v2.begin(),v2.end());

    int matches=v1.size();
    // v3 and v4 are the vectors that store the index of the greatest value in v1 and v2 
    // that are less than the considered index in given vectors
    std::vector<int> v3,v4;
    int pos1=0,pos2=0;
    for(int i=0;i<l1;i++){
        if(pos1<matches && i<v1[pos1]) v3.push_back(pos1-1);
        else{
            if(pos1<matches) pos1++;
            v3.push_back(pos1-1);
        }
    }
    for(int i=0;i<l2;i++){
        if(pos2<matches && i<v2[pos2]) v4.push_back(pos2-1);
        else{
            if(pos2<matches) pos2++;
            v4.push_back(pos2-1);
        }
    }
    // the maximum approximately matched pattern length and it's start indices in given vectors
    int max_length=0;
    int si=-1,sj=-1;
    for(int i=0;i<matches;i++){
        // consider pattern starts at k in submission1 such that first matched
        //token is the ith token in v1
        for(int k=(i==0)?0:v1[i-1]+1;k<=v1[i];k++){

            // v5 is the vector that stores the greatest index upto which a approximately matched
            // pattern length can be formed starting from k in first vector submission1,
            // assuming that all matchable tokens (i.e. indices in v1) are matched.
            std::vector<int> v5;
            int curr_match_length=0;
            int end_index=k-1;
            for(int j=k;j<l1;j++){
                if(v3[j]>=0 && v1[v3[j]]==j){
                    curr_match_length++;
                }
                if(float(curr_match_length)>=0.8*float(j-k+1)){
                    v5.push_back(j);
                    end_index=j;
                }
                else v5.push_back(end_index);
            }
            // pattern contains v2[i] since the first matched token is ith token in v2 
            // consider the pattern ends at j
            for(int j=l2-1;j>=v2[i] && j<=v2[i]+(l1-k)-1;j--){
                // the max pattern's end point index in submission1 possible 
                // for these k and j values
                int the_max=v5[std::min(k-1+j+1,
                                    (v4[j]!=matches-1)?(v1[v4[j]+1]-1):(l1-1))-(k)];
                // the_max should not go above l1 and 
                // the length of the pattern can't be more than j+1
                if(v4[j]!=matches-1 && the_max==(v1[v4[j]+1]-1)){
                    int match_length=v4[j]-i+1;
                    int associated_max_length=int(1.25*float(match_length));
                    the_max=std::max(the_max,std::min(l1-1,
                                    std::min(associated_max_length+k-1,k+j)));
                }
                // if this condition holds then the pattern does not contain the 
                // v2[i] index in submission2
                if(the_max-k+1<j-v2[i]+1){
                    continue;
                }
                if(the_max-k+1>max_length){
                    max_length=the_max-k+1;
                    si=k;
                    sj=j-max_length+1;
                }
            }   
        }
    }
    // exchange refers to the order of vectors passed to this function from the 
    // match_submissions function
    if(max_length>result[2] && max_length>=30){
        result[2]=max_length;
        result[3]=(exchange)?sj:si;
        result[4]=(exchange)?si:sj;
    }
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0 , 0};

    int l1=submission1.size();
    int l2=submission2.size();
    
    result[1]=std::max(total_exact_matched_length(submission1,submission2)
                        ,total_exact_matched_length(submission2,submission1));

    approx_match_length(result,submission2,submission1,true);
    approx_match_length(result,submission1,submission2,false);
    // std::cout<<l1<<" "<<l2<<std::endl;
    // bench mark to flag it to be plagiarized
    if((float(result[1])/float(std::min(l1,l2)))>0.25 || float(result[2])/float(std::min(l1,l2))>0.3 ) result[0]=1;

    return result;
    // End TODO
}