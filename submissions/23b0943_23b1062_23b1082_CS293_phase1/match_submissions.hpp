#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here

int total_exact_matched_length(std::vector<int> & submission1,std::vector<int> & submission2){
    int l1=submission1.size();
    int l2=submission2.size();

    std::vector<int> counted_index(l2,0);
    int result=0;
    for(int i=0;i<l1;i++){
        // position and max matched length for this index i in the second vector
        int pos_j=-1,max_length=0; 
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
            i=pi;
        }
        if(max_length>=10){
            for(int a=i,b=pos_j;a<i+max_length && b<pos_j+max_length;a++,b++){
                if(counted_index[b]==0){
                    result++;
                    counted_index[b]=1;
                }
            }
            // we are not visiting the indices of first vector which are already counted above
            i=i+max_length-1;
        }
    }
    return result;
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0 , 0};

    int l1=submission1.size();
    int l2=submission2.size();

    result[1]=std::max(total_exact_matched_length(submission1,submission2)
                        ,total_exact_matched_length(submission2,submission1));

    // matrices storing the maximum approximately matched pattern length ending
    // at i in v1 and j in v2 and the corresponding matched subsequence length in it
    // using dynamic programming to fill these
    // match[i][j]>=0.8*dp[i][j]
    std::vector<std::vector<int> > dp(l1,std::vector<int>(l2,-1)); 
    std::vector<std::vector<int>> match(l1,std::vector<int> (l2,-1));
    // max approximately matched pattern length found and its endpoints in both the vectors
    int max_length=0;
    int pi=-1,pj=-1;
    for(int i=0;i<l1;i++){
        for(int j=0;j<l2;j++){
            if(i==0 || j==0){
                dp[i][j]=(submission1[i]==submission2[j]);
                match[i][j]=(submission1[i]==submission2[j]);
            }
            else if(submission1[i]==submission2[j]){
                // we can extend the sequence ending at i-1 and j-1
                dp[i][j]=dp[i-1][j-1]+1;
                match[i][j]=match[i-1][j-1]+1;
            }
            else{
                // we can extend if match length condition is satisfied
                if(float(match[i-1][j-1])>=0.8*float(dp[i-1][j-1]+1)){
                    dp[i][j]=dp[i-1][j-1]+1;
                    match[i][j]=match[i-1][j-1];
                }
                else{
                    dp[i][j]=0;
                    match[i][j]=0;
                }
            }
            if(dp[i][j]>max_length){
                max_length=dp[i][j];
                pi=i;
                pj=j;
            }
        }
    }
    // doing the same thing as above but for sequences starting at i in v1 and j in v2
    for(int i=l1-1;i>=0;i--){
        for(int j=l2-1;j>=0;j--){
            if(i==l1-1 || j==l2-1){
                dp[i][j]=(submission1[i]==submission2[j]);
                match[i][j]=(submission1[i]==submission2[j]);
            }
            else if(submission1[i]==submission2[j]){
                dp[i][j]=dp[i+1][j+1]+1;
                match[i][j]=match[i+1][j+1]+1;
            }
            else{
                if(float(match[i+1][j+1])>=0.8*float(dp[i+1][j+1]+1)){
                    dp[i][j]=dp[i+1][j+1]+1;
                    match[i][j]=match[i+1][j+1];
                }
                else{
                    dp[i][j]=0;
                    match[i][j]=0;
                }
            }
            if(dp[i][j]>max_length){
                max_length=dp[i][j];
                pi=i+max_length-1;
                pj=j+max_length-1;
            }
        }
    }

    if(max_length>=30){
        result[2]=max_length;
        result[3]=pi-max_length+1;
        result[4]=pj-max_length+1;
    }
    // bench mark to flag it to be plagiarized
    if((float(result[1])/float(std::min(l1,l2)))>0.2 || float(result[2])/float(std::min(l1,l2))>0.2 ) result[0]=1;

    return result;
    // End TODO
}