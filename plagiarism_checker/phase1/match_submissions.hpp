#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------
// #include<map>
#include<algorithm>
#define CHUNK_SIZE 10
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

void approx_match_length(std::array<int,5> &result,std::vector<int> &submission1,std::vector<int> &submission2,bool exchange){
    
    int l1=submission1.size();
    int l2=submission2.size();
    std::vector<std::vector<int>> matching=std::vector<std::vector<int>> (l1,std::vector<int> (l2,-1));
    
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

    // for(int x : v1) std::cout<<x<<" ";
    // std::cout<<std::endl;
    // for(int x : v2) std::cout<<x<<" ";
    // std::cout<<std::endl;

    int matches=v1.size();
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

    // for(int x : v3) std::cout<<x<<" ";
    // std::cout<<std::endl;
    // for(int x : v4) std::cout<<x<<" ";
    // std::cout<<std::endl;
    int max_length=0;
    int si=-1,sj=-1;
    for(int i=0;i<matches;i++){
        for(int k=(i==0)?0:v1[i-1]+1;k<=v1[i];k++){
            // int left=v1[i];
            // if(i>0) left-=v1[i-1]+1;

            std::vector<int> v5;
            int curr=0;
            int last=k-1;
            for(int j=k;j<l1;j++){
                if(v3[j]>=0 && v1[v3[j]]==j){
                    // std::cout<<"here"<<" ";
                    curr++;
                }
                if(float(curr)>=0.8*float(j-k+1)){
                    // std::cout<<"there"<<" ";
                    v5.push_back(j);
                    last=j;
                }
                else v5.push_back(last);
            }
            // std::cout<<std::endl;
            // std::cout<<"k="<<k<<std::endl;
            // for(int x : v5) std::cout<<x<<" ";
            // std::cout<<std::endl;
            
            for(int j=l2-1;j>=v2[i] && j<=v2[i]+(l1-k)-1;j--){
                // int  right_length=j-v2[i]+1;
                // int max_match_length=v4[j]-i+1;
                // int min_match_length=int(0.8*(float(right_length)))+int(float(int(0.8*(float(right_length))))!=0.8*(float(right_length)));
                // if(min_match_length>max_match_length) continue;
                // int x=0;
                // while(x<v2[i]){

                // }
                int the_max=v5[std::min(k-1+j+1,(v4[j]!=matches-1)?(v1[v4[j]+1]-1):(l1-1))-(k)];
                // if(the_max-k+1<(j-v2[i]+1)) continue;
                // if(the_max-k+1==435 && the_max-k+1>=j-v2[i]+1 && max_length!=435){
                //     std::cout<<"i="<<i<<std::endl;
                //     std::cout<<"j="<<j<<std::endl;
                //     std::cout<<"v2[i]="<<v2[i]<<std::endl;
                //     std::cout<<"v1[i]="<<v1[i]<<std::endl;
                //     std::cout<<"k="<<k<<std::endl;
                //     std::cout<<"v4[j]="<<v4[j]<<std::endl;
                //     std::cout<<"v3[the_max]="<<v3[the_max]<<std::endl;
                //     for(int a=i;a<=389;a++){
                //         std::cout<<"count="<<a-i+1<<" indices: "<<v1[a]<<" "<<v2[a]<<" values: "<<submission1[v1[a]]<<" "<<submission2[v2[a]]<<std::endl;
                //         if(submission1[v1[a]]!=submission2[v2[a]]) std::cout<<"......................................................................................................."<<std::endl;
                //     }
                //     std::cout<<"hello"<<std::endl;
                // }
                if(v4[j]!=matches-1 && the_max==(v1[v4[j]+1]-1)){
                    int match_length=v4[j]-i+1;
                    int associated_max_length=int(1.25*float(match_length));
                    the_max=std::max(the_max,std::min(l1-1,std::min(associated_max_length+k-1,k+j)));
                    // if(the_max-k+1<(j-v2[i]+1)) continue;
                }
                // if(the_max-k+1>2) std::cout<<"hello"<<std::endl;
                if(the_max-k+1<j-v2[i]+1){
                    // std::cout<<"bye"<<std::endl;
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
    if(max_length>result[2]){
        result[2]=max_length;
        result[3]=(exchange)?sj:si;
        result[4]=(exchange)?si:sj;
    }
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0 , 0};

    // std::vector<int> v=submission2;
    // submission2=submission1;
    // submission1=v;

    int l1=submission1.size();
    int l2=submission2.size();

    result[1]=std::max(total_exact_matched_length(submission1,submission2)
                        ,total_exact_matched_length(submission2,submission1));

    // // matrices storing the maximum approximately matched pattern length ending
    // // at i in v1 and j in v2 and the corresponding matched subsequence length in it
    // // using dynamic programming to fill these
    // // match[i][j]>=0.8*dp[i][j]
    // std::vector<std::vector<int> > dp(l1,std::vector<int>(l2,-1)); 
    // std::vector<std::vector<int>> match(l1,std::vector<int> (l2,-1));
    // // max approximately matched pattern length found and its endpoints in both the vectors
    // int max_length=0;
    // int pi=-1,pj=-1;
    // for(int i=0;i<l1;i++){
    //     for(int j=0;j<l2;j++){
    //         if(i==0 || j==0){
    //             dp[i][j]=(submission1[i]==submission2[j]);
    //             match[i][j]=(submission1[i]==submission2[j]);
    //         }
    //         else if(submission1[i]==submission2[j]){
    //             // we can extend the sequence ending at i-1 and j-1
    //             dp[i][j]=dp[i-1][j-1]+1;
    //             match[i][j]=match[i-1][j-1]+1;
    //         }
    //         else{
    //             // we can extend if match length condition is satisfied
    //             if(float(match[i-1][j-1])>=0.8*float(dp[i-1][j-1]+1)){
    //                 dp[i][j]=dp[i-1][j-1]+1;
    //                 match[i][j]=match[i-1][j-1];
    //             }
    //             else{
    //                 dp[i][j]=0;
    //                 match[i][j]=0;
    //             }
    //         }
    //         if(dp[i][j]>max_length){
    //             max_length=dp[i][j];
    //             pi=i;
    //             pj=j;
    //         }
    //     }
    // }
    // // doing the same thing as above but for sequences starting at i in v1 and j in v2
    // for(int i=l1-1;i>=0;i--){
    //     for(int j=l2-1;j>=0;j--){
    //         if(i==l1-1 || j==l2-1){
    //             dp[i][j]=(submission1[i]==submission2[j]);
    //             match[i][j]=(submission1[i]==submission2[j]);
    //         }
    //         else if(submission1[i]==submission2[j]){
    //             dp[i][j]=dp[i+1][j+1]+1;
    //             match[i][j]=match[i+1][j+1]+1;
    //         }
    //         else{
    //             if(float(match[i+1][j+1])>=0.8*float(dp[i+1][j+1]+1)){
    //                 dp[i][j]=dp[i+1][j+1]+1;
    //                 match[i][j]=match[i+1][j+1];
    //             }
    //             else{
    //                 dp[i][j]=0;
    //                 match[i][j]=0;
    //             }
    //         }
    //         if(dp[i][j]>max_length){
    //             max_length=dp[i][j];
    //             pi=i+max_length-1;
    //             pj=j+max_length-1;
    //         }
    //     }
    // }

    // if(max_length>=30){
    //     result[2]=max_length;
    //     result[3]=pi-max_length+1;
    //     result[4]=pj-max_length+1;
    // }

    approx_match_length(result,submission2,submission1,true);

    std::vector<std::vector<int>> dummy=std::vector<std::vector<int>> (result[2],std::vector<int> (result[2],-1));
    for(int i=0;i<result[2];i++){
        for(int j=0;j<result[2];j++){
            if(submission1[i+result[3]]==submission2[j+result[4]]){
                if(i==0 || j==0) dummy[i][j]=1;
                else dummy[i][j]=dummy[i-1][j-1]+1;
            }
            else{
                if(i==0 && j==0)  dummy[i][j]=0;
                else if(i==0) dummy[i][j]=dummy[i][j-1];
                else if(j==0) dummy[i][j]=dummy[i-1][j];
                else dummy[i][j]=std::max(dummy[i-1][j],dummy[i][j-1]);
            }
        }
    }
    
    std::cout<<"actual matched length: "<<dummy[result[2]-1][result[2]-1]<<std::endl;

    // bench mark to flag it to be plagiarized
    if((float(result[1])/float(std::min(l1,l2)))>0.2 || float(result[2])/float(std::min(l1,l2))>0.2 ) result[0]=1;

    return result;
    // End TODO
}