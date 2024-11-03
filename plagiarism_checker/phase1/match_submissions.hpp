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

int mfind(std::vector<std::vector<int>> & matching,int i,int j){
    if(i>=0 && j>=0 && i<matching.size() && j<matching[0].size()) return matching[i][j];
    return 0;
}

int match_length(std::vector<std::vector<int>> & matching,std::pair<int,int> left,std::pair<int,int> right){
    return mfind(matching,right.first,right.second)-mfind(matching,right.first,left.second-1)-mfind(matching,left.first-1,right.second)+mfind(matching,left.first-1,left.second-1);
    // return mfind(matching,right.first,right.second)-mfind(matching,left.first-1,left.second-1);

}

// int movement(std::pair<int,int> &pos,int l1,int l2){
//     if(pos.first==pos.second)
// }

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

    // submission1=std::vector<int> {};
    // submission2=std::vector<int> {};

    // for(int i=1;i<=200;i++){
    //     submission1.push_back(i);
    //     if(i<=80 || i>120) submission2.push_back(i);
    // }

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

    std::vector<int> visited_row(l1,0),visited_column(l2,0);
    int value_1=0;
    for(int i=0;i<l1;i++){
        int pos_j=-1,max_length=0;
        for(int j=0;j<l2;j++){
            // if(visited_column[j]==1) continue;
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
                if(visited_column[b]==0){
                    value_1++;
                    // visited_row[a]=1;
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
    visited_column=std::vector<int>(l1,0);
    int value_2=0;
    for(int i=0;i<l2;i++){
        int pos_j=-1,max_length=0;
        for(int j=0;j<l1;j++){
            // if(visited_column[j]==1) continue;
            int pi=i,pj=j;
            int length=0;
            while(i<l2 && j<l1 && (submission2[i]==submission1[j])){
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
                if(visited_column[b]==0){
                    value_2++;
                    // visited_row[a]=1;
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
    result[1]=std::max(value_1,value_2);


    std::vector<std::vector<int> > dp(l1,std::vector<int>(l2,-1));
    std::vector<std::vector<int>> match(l1,std::vector<int> (l2,-1));
    int max_length=0;
    int pi=-1,pj=-1;
    for(int i=0;i<l1;i++){
        for(int j=0;j<l2;j++){
            if(i==0 || j==0){
                dp[i][j]=(submission1[i]==submission2[j]);
                match[i][j]=(submission1[i]==submission2[j]);
            }
            else if(submission1[i]==submission2[j]){
                dp[i][j]=dp[i-1][j-1]+1;
                match[i][j]=match[i-1][j-1]+1;

                // if(i-2-dp[i-1][j-1]>=0 && j-2-dp[i-1][j-1]>=0 && float(match[i][j]+match[i-2-dp[i-1][j-1]][j-2-dp[i-1][j-1]])>=0.8*float(dp[i][j]+1+dp[i-2-dp[i-1][j-1]][j-2-dp[i-1][j-1]])){
                //     dp[i][j]+=1+dp[i-2-dp[i-1][j-1]][j-2-dp[i-1][j-1]];
                //     match[i][j]+=match[i-2-dp[i-1][j-1]][j-2-dp[i-1][j-1]];
                // }

                // std::vector<int> matched (std::min(i-dp[i-1][j-1],j-dp[i-1][j-1]));
                // int b=matched.size();
                // for(int a=0;a<b;a++){
                //     matched[b-a-1]=int(submission1[i-dp[i-1][j-1]-a-1]==submission2[j-dp[i-1][j-1]-a-1])+(a==0?0:matched[b-a]);
                // }

                // for(int a=0;a<b;a++){
                //     if(0.8*float(b-a+dp[i][j])<=float(matched[a]+match[i][j])){
                //         dp[i][j]+=b-a;
                //         match[i][j]+=matched[a];
                //         break;
                //     }
                // }


            }
            else{
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
    for(int i=l1-1;i>=0;i--){
        for(int j=l2-1;j>=0;j--){
            if(i==l1-1 || j==l2-1){
                dp[i][j]=(submission1[i]==submission2[j]);
                match[i][j]=(submission1[i]==submission2[j]);
            }
            else if(submission1[i]==submission2[j]){
                dp[i][j]=dp[i+1][j+1]+1;
                match[i][j]=match[i+1][j+1]+1;

                // if(i-2-dp[i-1][j-1]>=0 && j-2-dp[i-1][j-1]>=0 && float(match[i][j]+match[i-2-dp[i-1][j-1]][j-2-dp[i-1][j-1]])>=0.8*float(dp[i][j]+1+dp[i-2-dp[i-1][j-1]][j-2-dp[i-1][j-1]])){
                //     dp[i][j]+=1+dp[i-2-dp[i-1][j-1]][j-2-dp[i-1][j-1]];
                //     match[i][j]+=match[i-2-dp[i-1][j-1]][j-2-dp[i-1][j-1]];
                // }

                // std::vector<int> matched (std::min(i-dp[i-1][j-1],j-dp[i-1][j-1]));
                // int b=matched.size();
                // for(int a=0;a<b;a++){
                //     matched[b-a-1]=int(submission1[i-dp[i-1][j-1]-a-1]==submission2[j-dp[i-1][j-1]-a-1])+(a==0?0:matched[b-a]);
                // }

                // for(int a=0;a<b;a++){
                //     if(0.8*float(b-a+dp[i][j])<=float(matched[a]+match[i][j])){
                //         dp[i][j]+=b-a;
                //         match[i][j]+=matched[a];
                //         break;
                //     }
                // }


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
    result[2]=max_length;
    result[3]=pi-max_length+1;
    result[4]=pj-max_length+1;

    std::cout<<l1<<" "<<l2<<std::endl;

    std::vector<std::vector<int>> matching(l1,std::vector<int> (l2,-1));
    // std::vector<int> row(l1,0),column(l2,0);
    for(int i=0;i<l1;i++){
        for(int j=0;j<l2;j++){
            if(submission1[i]==submission2[j]){
                if(i==0 || j==0) matching[i][j]=1;
                else matching[i][j]=matching[i-1][j-1]+1;
            }
            else{
                if(i==0 && j==0) matching[i][j]=0;
                else if(i==0) matching[i][j]=matching[i][j-1];
                else if(j==0) matching[i][j]=matching[i-1][j];
                else matching[i][j]=std::max(matching[i-1][j],matching[i][j-1]);
            }
        }
    }



    // int max_length=0;
    // int pi=-1,pj=-1;
    // for(int i=0;i<l1;i++){
    //     for(int j=0;j<l2;j++){
    //         for(int l=std::min(i+1,j+1);l>=max_length+1;l--){
    //             // if(i==90+l-1 && j==5+l-1 && l==548) std::cout<<match_length(matching,std::make_pair(i-l+1,j-l+1),std::make_pair(i,j))<<" and "<<l<<std::endl;
    //             if(float(match_length(matching,std::make_pair(i-l+1,j-l+1),std::make_pair(i,j)))>=(0.8*float(l))){
    //                 max_length=l;
    //                 pi=i;
    //                 pj=j;
    //                 break;
    //             }
    //         }
    //     }
    // }

    // result[2]=max_length;
    // result[3]=pi-max_length+1;
    // result[4]=pj-max_length+1;
    // std::cout<<match_length(matching,std::make_pair(result[3],result[4]),std::make_pair(result[3]+max_length-1,result[4]+max_length-1));
    // std::vector<std::vector<int>> dummy=std::vector<std::vector<int>>(max_length,std::vector<int>(max_length,-1));

    // for(int i=0;i<max_length;i++){
    //     for(int j=0;j<max_length;j++){
    //         if(submission1[i+result[3]]==submission2[j+result[4]]){
    //             if(i==0 || j==0) dummy[i][j]=1;
    //             else dummy[i][j]=dummy[i-1][j-1]+1;
    //         }
    //         else{
    //             if(i==0 && j==0) dummy[i][j]=0;
    //             else if(i==0) dummy[i][j]=dummy[i][j-1];
    //             else if(j==0) dummy[i][j]=dummy[i-1][j];
    //             else dummy[i][j]=std::max(dummy[i-1][j],dummy[i][j-1]);
    //         }
    //     }
    // }
    // std::cout<<" actual matching : "<<dummy[max_length-1][max_length-1]<<std::endl;
    // for(std::vector<int> x : matching){
    //     for(int y : x){
    //         std::cout<<y<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
    // int max_length=0;
    // for(int i=0;i<l1;i++){
    //     for(int j=0;j<l2;j++){
    //         for(int k=i+29;k<l1;k++){
    //             for(int l=j+29;l<l2;l++){
    //                 if(float(matching[k][l]-matching[i][j])>=0.8*float(std::max(k-i+1,l-j+1)) && std::max(k-i+1,l-j+1)>max_length) max_length=std::max(k-i+1,l-j+1);
    //             }
    //         }
    //     }
    // }

    // std::pair<int,int> left={0,0};
    // std::pair<int,int> right={l1-1,l2-1};
    // while(true){
    //     // std::cout<<left.first<<","<<left.second<<" and "<<right.first<<","<<right.second<<std::endl;
    //     if(float(match_length(matching,left,right))>=0.8*float(std::max(right.first-left.first+1,right.second-left.second+1))){
    //         result[2]=std::max(right.first-left.first+1,right.second-left.second+1);
    //         std::cout<<"giving the value "<<result[2]<<std::endl;
    //         std::cout<<left.first<<" "<<left.second<<" "<<right.first<<" "<<right.second<<std::endl;
    //         std::cout<<matching[left.first][left.second]<<" "<<matching[right.first][right.second]<<std::endl;
    //         break;
    //     }
    //     int curr_match_length=match_length(matching,left,right);
    //     if(left.first+29<right.first && curr_match_length<=match_length(matching,std::make_pair(left.first+1,left.second),right)) left.first++;
    //     else if(left.second+29<right.second && curr_match_length<=match_length(matching,std::make_pair(left.first,left.second+1),right)) left.second++;
    //     else if(right.first>left.first+29 && curr_match_length<=match_length(matching,left,std::make_pair(right.first-1,right.second))) right.first--;
    //     else if(right.second>left.second+29 && curr_match_length<=match_length(matching,left,std::make_pair(right.first,right.second-1))) right.second--;
        
    //     else if(((right.second-left.second)>(right.first-left.first)) && ((right.first-left.first)>29)) left.second++;
    //     else if(((right.first-left.first)>(right.second-left.second)) && ((right.second-left.second)>29)) left.first++;
        
    //     else if(left.first+29<right.first) left.first++;
    //     else if(left.second+29<right.second) left.second++;
    //     else if(right.first>left.first+29) right.first--;
    //     else if(right.second>left.second+29) right.second--;

    //     if(left.first+29==right.first && left.second+29==right.second) break;
    // }

    // std::cout<<match_length(matching,std::make_pair(280,160),std::make_pair(559,439))<<std::endl;

    // std::pair<int,int> end={l1-1,l2-1};
    // std::pair<int,int> the_max={-1,-1};
    // for(int i=l1-1;i>=0;i--){
    //     for(int j=l2-1;j>=0;j--){
    //         end=std::make_pair(i,j);
    //         if(float(mfind(matching,end.first,end.second))>=0.8*(std::max(end.first+1,end.second+1)) && std::max(end.first+1,end.second+1)>std::max(the_max.first+1,the_max.second+1)) the_max=end;
    //     }
    // }
    // result[2]=std::max(the_max.first+1,the_max.second+1);


    // std::cout<<"result1 : "<<result[1]<<std::endl;
    // std::cout<<"result2 : "<<result[2]<<std::endl;
    // std::cout<<"result3 : "<<result[3]<<std::endl;
    // std::cout<<"result4 : "<<result[4]<<std::endl;


    // const int MATCH_SCORE = 10;
    // const int MISMATCH_PENALTY = -2;
    // const int GAP_PENALTY = -4;


    // std::vector<std::vector<int>> dp(l1 + 1, std::vector<int>(l2 + 1, 0));
    // int max_score = 0;
    // int end_i = 0, end_j = 0;

    // for (int i = 1; i <= l1; ++i) {
    //     for (int j = 1; j <= l2; ++j) {
    //         int match = (submission1[i - 1] == submission2[j - 1]) ? MATCH_SCORE : MISMATCH_PENALTY;
    //         int score_diag = dp[i - 1][j - 1] + match;
    //         int score_up = dp[i - 1][j] + GAP_PENALTY;
    //         int score_left = dp[i][j - 1] + GAP_PENALTY;

    //         dp[i][j] = std::max(0, std::max(score_diag, std::max(score_up, score_left)));

    //         if (dp[i][j] > max_score) {
    //             max_score = dp[i][j];
    //             end_i = i;
    //             end_j = j;
    //         }
    //     }
    // }

    // // Traceback
    // int length = 0;
    // int i = end_i, j = end_j;
    // while (i > 0 && j > 0 && dp[i][j] > 0) {
    //     if (dp[i][j] == dp[i - 1][j - 1] + MATCH_SCORE && submission1[i - 1] == submission2[j - 1]) {
    //         // result[1]++;  // Number of matches
    //         i--;
    //         j--;
    //         length++;
    //     } else if (dp[i][j] == dp[i - 1][j] + GAP_PENALTY) {
    //         i--;
    //     } else {
    //         j--;
    //     }
    // }

    // // Fill the results with details
    // result[0] = 1;                // Indicating that a match was found
    // result[2] = length;         // Max score
    // result[3] = end_i - length;    // Start position in submission1
    // result[4] = end_j - length;    // Start position in submission2

    result[0]=1;
    return result; // dummy return
    // End TODO
}
