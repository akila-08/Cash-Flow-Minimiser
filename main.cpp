#include <bits/stdc++.h>
#include <stdlib.h>
using namespace std;

class bank{
    public:
    string name;
    int netAmount;
    set<string> types;
};

int getMinIndex(bank listOfNetAmounts[],int numBanks){
    int min=INT_MAX, minIndex=-1;
    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount == 0) continue;

        if(listOfNetAmounts[i].netAmount < min){
            minIndex = i;
            min = listOfNetAmounts[i].netAmount;
        }
    }
    return minIndex;
}

int getSimpleMaxIndex(bank listOfNetAmounts[],int numBanks){
    int max=INT_MIN, maxIndex=-1;
    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount == 0) continue;

        if(listOfNetAmounts[i].netAmount > max){
            maxIndex = i;
            max = listOfNetAmounts[i].netAmount;
        }
    }
    return maxIndex;
}

pair<int,string> getMaxIndex(bank listOfNetAmounts[],int numBanks,int minIndex,bank input[],int maxNumTypes){
    int max=INT_MIN;
    int maxIndex=-1;
    string matchingType;

    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount == 0) continue;

        if(listOfNetAmounts[i].netAmount < 0) continue;

        //TODO
        //see complexity of intersection

        vector<string> v(maxNumTypes);
        vector<string>::iterator ls=set_intersection(listOfNetAmounts[minIndex].types.begin(),listOfNetAmounts[minIndex].types.end(), listOfNetAmounts[i].types.begin(),listOfNetAmounts[i].types.end(), v.begin());

        if((ls-v.begin())!=0 && max<listOfNetAmounts[i].netAmount ){
            max=listOfNetAmounts[i].netAmount;
            maxIndex=i;
            matchingType = *(v.begin());
        }
    }

    //if there is NO such max which has a common type with any remaining banks then maxIndex has -1
    // also return the common payment type
    return make_pair(maxIndex,matchingType);
}

void printAns(vector<vector<pair<int,string>>> ansGraph, int numBanks,bank input[]){

    cout<<"\n\t\t\t\t\tThe transactions for minimum cash flow are as follows : \n\n";
    for(int i=0;i<numBanks;i++){
        for(int j=0;j<numBanks;j++){

            if(i==j) continue;

            if(ansGraph[i][j].first != 0 && ansGraph[j][i].first != 0){

                if(ansGraph[i][j].first == ansGraph[j][i].first){
                    ansGraph[i][j].first=0;
                    ansGraph[j][i].first=0;
                }
                else if(ansGraph[i][j].first > ansGraph[j][i].first){
                    ansGraph[i][j].first -= ansGraph[j][i].first;
                    ansGraph[j][i].first =0;

                    cout<<"\n\t\t\t\t\t\t"<<input[i].name<<" pays Rs" << ansGraph[i][j].first<< "to "<<input[j].name<<" via "<<ansGraph[i][j].second<<endl;
                }
                else{
                    ansGraph[j][i].first -= ansGraph[i][j].first;
                    ansGraph[i][j].first = 0;

                    cout<<"\n\t\t\t\t\t\t"<<input[j].name<<" pays Rs "<< ansGraph[j][i].first<<" to "<<input[i].name<<" via "<<ansGraph[j][i].second<<endl;

                }
            }
            else if(ansGraph[i][j].first != 0){
                cout<<"\n\t\t\t\t\t\t"<<input[i].name<<" pays Rs "<<ansGraph[i][j].first<<" to "<<input[j].name<<" via "<<ansGraph[i][j].second<<endl;

            }
            else if(ansGraph[j][i].first != 0){
                cout<<"\n\t\t\t\t\t\t"<<input[j].name<<" pays Rs "<<ansGraph[j][i].first<<" to "<<input[i].name<<" via "<<ansGraph[j][i].second<<endl;

            }

            ansGraph[i][j].first = 0;
            ansGraph[j][i].first = 0;
        }
    }
    cout<<"\n";
}

void minimizeCashFlow(int numBanks,bank input[],unordered_map<string,int>& indexOf,int numTransactions,vector<vector<int>>& graph,int maxNumTypes){

    //Find net amount of each bank 
    bank listOfNetAmounts[numBanks];

    for(int b=0;b<numBanks;b++){
        listOfNetAmounts[b].name = input[b].name;
        listOfNetAmounts[b].types = input[b].types;

        int amount = 0;
        //incoming edges
        //column traverse
        for(int i=0;i<numBanks;i++){
            amount += (graph[i][b]);
        }

        //outgoing edges
        //row traverse
        for(int j=0;j<numBanks;j++){
            amount += ((-1) * graph[b][j]);
        }

        listOfNetAmounts[b].netAmount = amount;
    }

    vector<vector<pair<int,string>>> ansGraph(numBanks,vector<pair<int,string>>(numBanks,{0,""}));//adjacency matrix


    //find min and max net amount
    int numZeroNetAmounts=0;

    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount == 0) numZeroNetAmounts++;
    }
    while(numZeroNetAmounts!=numBanks){

        int minIndex=getMinIndex(listOfNetAmounts, numBanks);
        pair<int,string> maxAns = getMaxIndex(listOfNetAmounts, numBanks, minIndex,input,maxNumTypes);

        int maxIndex = maxAns.first;

        if(maxIndex == -1){

            (ansGraph[minIndex][0].first) += abs(listOfNetAmounts[minIndex].netAmount);
            (ansGraph[minIndex][0].second) = *(input[minIndex].types.begin());

            int simpleMaxIndex = getSimpleMaxIndex(listOfNetAmounts, numBanks);
            (ansGraph[0][simpleMaxIndex].first) += abs(listOfNetAmounts[minIndex].netAmount);
            (ansGraph[0][simpleMaxIndex].second) = *(input[simpleMaxIndex].types.begin());

            listOfNetAmounts[simpleMaxIndex].netAmount += listOfNetAmounts[minIndex].netAmount;
            listOfNetAmounts[minIndex].netAmount = 0;

            if(listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;

            if(listOfNetAmounts[simpleMaxIndex].netAmount == 0) numZeroNetAmounts++;

        }
        else{
            int transactionAmount = min(abs(listOfNetAmounts[minIndex].netAmount), listOfNetAmounts[maxIndex].netAmount);

            (ansGraph[minIndex][maxIndex].first) += (transactionAmount);
            (ansGraph[minIndex][maxIndex].second) = maxAns.second;

            listOfNetAmounts[minIndex].netAmount += transactionAmount;
            listOfNetAmounts[maxIndex].netAmount -= transactionAmount;

            if(listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;

            if(listOfNetAmounts[maxIndex].netAmount == 0) numZeroNetAmounts++;
        }

    }

    printAns(ansGraph,numBanks,input);
}


int main()
{
    cout<<"\n\t\t\t\t///////////////////// WELCOME TO PSG  CASH FLOW MINIMIZER SYSTEM \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n";
    cout<<"\n\t\t\tThis system minimizes the number of transactions among multiple banks in the different corners of the world \n\t\t            that use different modes of payment.There is one world bank (with all payment modes) to act as an \n\t\t\t\t\tintermediary between banks that have no common mode of payment \n\n";
    cout<<"\n\n\t\t\t\t\tEnter the number of banks participating in the transactions : ";
    int numBanks;cin>>numBanks;

    bank input[numBanks];
    unordered_map<string,int> indexOf;//stores index of a bank
    system("CLS");
    cout<<"\n\n\n\n\t\t\t\t\tEnter the details of the banks and transactions as stated:\n";
    cout<<"\n\t\t\t\t\tBank name ,number of payment modes it has and the payment modes.\n";
    cout<<"\n\t\t\t\t\tBank name and payment modes should not contain spaces\n";

    int maxNumTypes;
    for(int i=0; i<numBanks;i++){
        if(i==0){
            cout<<"\n\n\n\t\t\t\t\t\t\t\tWorld Bank : ";
        }
        else{
            cout<<"\n\n\n\t\t\t\t\t\t\t\tBank "<<i<<" : ";
        }
        cin>>input[i].name;
        indexOf[input[i].name] = i;
        int numTypes;
        cout<<"\n\n\t\t\t\t\t\t\tEnter number of payment modes : ";
        cin>>numTypes;

        if(i==0) maxNumTypes = numTypes;

        string type;
        cout<<"\n\n\t\t\t\t\t\t\tEnter the payment modes : ";
        while(numTypes--){
            cin>>type;

            input[i].types.insert(type);
        }

    }
    system("CLS");
    cout<<"\n\n\n\t\t\t\t\t\t\tEnter number of transactions : ";
    int numTransactions;
    cin>>numTransactions;

    vector<vector<int>> graph(numBanks,vector<int>(numBanks,0));//adjacency matrix

    cout<<"\n\n\n\n\t\t\t\t\tEnter the details of each transaction as stated:";
    cout<<"\n\t\t\t\t\tDebtor Bank , creditor Bank and amount\n";
    cout<<"\n\t\t\t\t\tThe transactions can be in any order\n";
    for(int i=0;i<numTransactions;i++){
        cout<<"\n\t\t\t\t\t\t"<<(i)<<" th transaction : ";
        string s1,s2;
        int amount;
        cin >> s1>>s2>>amount;

        graph[indexOf[s1]][indexOf[s2]] = amount;
    }
    system("CLS");
    minimizeCashFlow(numBanks,input,indexOf,numTransactions,graph,maxNumTypes);
    return 0;
}
