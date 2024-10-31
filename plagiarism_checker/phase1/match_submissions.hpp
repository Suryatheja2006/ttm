#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------
#include<map>
#define CHUNK_SIZE 10
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here
void hashing(std::vector<int> &v,std::vector<int> &h){

    int l=v.size();

    int p=31;
    int M=1e9+7;

    int curr=0;
    int pow=1;
    for(int i=CHUNK_SIZE-1;i>=0;i--){
        curr=(curr+((long)v[i]*(long)pow)%M)%M;
        if(i>0) pow=((long)pow*(long)p)%M;
    }
    h[0]=curr;
    for(int i=1;i<=l-CHUNK_SIZE;i++){
        h[i]=h[i-1];
        h[i]=(h[i]-((long)v[i-1]*(long)pow)%M+M)%M;
        if(h[i]<0 && i==1) std::cout<<"here"<<std::endl;
        h[i]=((long)pow*(long)h[i])%M;
        h[i]=(h[i]+v[i+CHUNK_SIZE-1])%M;
    }
    return;
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0 , 0};

    // int l=std::min(submission1.size(),submission2.size());
    // for(int i=0;i<l;i++){
    //     if(submission1[i]==submission2[i]) result[1]++;
    // }
    // // std::cout<<"submission1 : "<<"size : "<<submission1.size()<<" and ";
    // // for(int x : submission1) std::cout<<x<<" ";
    // // std::cout<<std::endl;

    // // std::cout<<"submission2 : "<<"size : "<<submission2.size()<<" and ";
    // // for(int x : submission2) std::cout<<x<<" ";
    // // std::cout<<std::endl;



    // int l1=submission1.size();
    // int l2=submission2.size();

    // std::vector<int> h1(l1-CHUNK_SIZE+1);
    // hashing(submission1,h1);

    // // for(int x : h1) std::cout<<x<<" ";
    // // std::cout<<std::endl;

    // std::map<int,int> hash1;
    // for(int x : h1) hash1[x]++;
    // // std::cout<<"hash1 size : "<<hash1.size()<<std::endl;

    // std::vector<int> h2(l2-CHUNK_SIZE+1);
    // hashing(submission2,h2);
    // l=0;
    // for(int x : h2){
    //     if(hash1.find(x)!=hash1.end()){
    //         // std::cout<<"there"<<std::endl;
    //         l++;
    //     }
    //     else{
    //         // std::cout<<"here"<<std::endl;
    //         if(l!=0){
    //             result[1]+=CHUNK_SIZE+l-1;
    //             // std::cout<<"added"<<std::endl;
    //             l=0;
    //         }
    //     }
    // }
    // // std::cout<<l<<std::endl;
    // if(l!=0){
    //     result[1]+=CHUNK_SIZE+l-1;
    //     l=0;
    // }
    // std::cout<<result[1]<<std::endl;


    //2D grid idea

    // int l1=30;
    // int l2=30;

//     submission1 = std::vector<int> {
//     1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
//     21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 
//     41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
//     61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
//     81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100
// };
//     submission2 = std::vector<int>{
//     1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
//     21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 90, 91, 92, 93,
//     41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
//     81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
//     91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120
// };
    int l1=submission1.size();
    int l2=submission2.size();

    
    // std::vector<std::vector<int>> grid(l1,std::vector<int> (l2,0));

    // for(int i=0;i<l1;i++){
    //     for(int j=0;j<l2;j++){
    //         if(submission1[i]==submission2[j]) grid[i][j]=1;
    //     }
    // }

    // for(std::vector<int> x : grid){
    //     for(int y : x){
    //         std::cout<<y<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
    // std::cout<<"  "<<std::endl;

    // std::vector<int> visited_row(l1,0),visited_column(l2,0);

    for(int i=0;i<l1;i++){
        int pos_j=-1,max_length=0;
        for(int j=0;j<l2;j++){
            if(visited_column[j]==1) continue;
            int pi=i,pj=j;
            int length=0;
            while(i<l1 && j<l2 && grid[i][j]==1){
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
                if(visited_row[a]==0 && visited_column[b]==0){
                    result[1]++;
                    visited_row[a]=1;
                    visited_column[b]=1;
                }
            }
            i=i+max_length-1;
            // break;
        }
        // else{
        //     i=pi;
        //     j=pj;
        // }
    }

    // std::vector<std::vector<int> > dp(l1,std::vector<int>(l2,-1))>;
    // int max_length=0;
    // for(int i=0;i<l1;i++){
    //     for(int j=0;j<l2;j++){
    //         if(i==0 || j==0) dp[i][j]=grid[i][j];
    //         else if(dp[i][j]==1) dp[i][j]=dp[i-1][j-1]+1;
            
            
    //     }
    // }


    std::cout<<result[1]<<std::endl;
    return result; // dummy return
    // End TODO
}
