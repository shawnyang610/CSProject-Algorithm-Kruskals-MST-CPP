//CSCI700-34 PROJECT7A KRUSKAL'S MST
//SHAWN YANG

#include <iostream>
#include <fstream>
using namespace std;

class graphNode
{
public:
    int nodeID;
    graphNode *next;
    graphNode(int inID)
    {
        nodeID = inID;
        next = NULL;
    }
    ~graphNode()
    {
        cout << "a graphNode Object is deleted" << endl;
    }
    
};

class undirectedEdge
{
public:
    int ni;
    int nj;
    int edgeCost;
    undirectedEdge *next;
    undirectedEdge (int inNi, int inNj, int inEdgeCost)
    {
        ni = inNi;
        nj = inNj;
        edgeCost = inEdgeCost;
        next = NULL;
    }
    ~undirectedEdge() { cout << "an undirectedEdge object is deleted" << endl; };
    void printEdge(ofstream *outfile1)
    {
        *outfile1 << ni << " " << nj << " " << edgeCost << endl;
    }
};

class KruskalMST
{
public:
    int numNodes;
    int * inWhichSet;
    int numSets;
    int totalMSTCost;
    ifstream *infile;
    ofstream *outfile1;
    ofstream *outfile2;
    undirectedEdge *MSTofG;
    undirectedEdge *edgeListHead;
    undirectedEdge *nextEdge;
    KruskalMST(int inNumNodes, ifstream* inInfile, ofstream * inOutfile1, ofstream * inOutfile2)
    {
        numNodes = inNumNodes;
        infile = inInfile;
        outfile1 = inOutfile1;
        outfile2 = inOutfile2;
        //initialize to self
        inWhichSet = new int[numNodes + 1];
        for (int i=1; i<numNodes+1; i++)
        {
            inWhichSet[i] = i;
        }
        numSets = numNodes;
        totalMSTCost = 0;
        MSTofG = new undirectedEdge(0, 0, 0);
        edgeListHead = new undirectedEdge(0, 0, 0);
        nextEdge=NULL;
    }
    void insertEdge (undirectedEdge *inEdge, undirectedEdge *inEdgeListhead)
    {
        //insert by using insertion sort by cost
        undirectedEdge *walker = inEdgeListhead;
        if (inEdgeListhead->next==NULL)
        {
            inEdgeListhead->next = inEdge;
        }
        else
        {
            while (walker->next!=NULL && inEdge->edgeCost>walker->next->edgeCost)
            {
                walker = walker->next;
            }
            inEdge->next = walker->next;
            walker->next = inEdge;
        }
   
    }
    
    static undirectedEdge* removeEdge (undirectedEdge * inEdgeListHead)
    {
        undirectedEdge *temp = inEdgeListHead->next;
        if (inEdgeListHead->next!=NULL){
            inEdgeListHead->next = inEdgeListHead->next->next;
        }
        return temp;
    }
    static void pushEdge (undirectedEdge *inEdge, undirectedEdge *inMSTofG)
    {
        inEdge->next=inMSTofG->next;
        inMSTofG->next=inEdge;
    }
    void merge2Sets (int inNode1, int inNode2)
    {
        int set4Node1 = inWhichSet[inNode1];
        int set4Node2 = inWhichSet[inNode2];
        if (set4Node1<set4Node2){
            for (int i=1; i<numNodes+1;i++){
                if (inWhichSet[i]==set4Node2){
                    inWhichSet[i]=set4Node1;
                }
            }
        }
        else {
            for (int i=1; i<numNodes+1; i++){
                if (inWhichSet[i]==set4Node1){
                    inWhichSet[i]=set4Node2;
                }
            }
        }
           
    }
    void printSet (int * inInWhichSet)
    {
        *outfile2<<"inWhichSet: "<<endl;
        for (int i=1; i<numNodes+1; i++){
            *outfile2<<"Node:"<<i<<" Set:"<<inWhichSet[i]<<" ";
        }
        *outfile2<<endl;
    }
    static void printList (undirectedEdge *inUndirectedEdgeList, ofstream *inOutfile)
    {
        undirectedEdge *walker = inUndirectedEdgeList;
        *inOutfile << "Listhead: -> ";
        while (walker->next != NULL)
        {
            *inOutfile << "<" << walker->ni << "," << walker->nj << "," << walker->edgeCost << "> -> ";
            walker = walker->next;
        }
        *inOutfile << "<" << walker->ni << "," << walker->nj << "," << walker->edgeCost << "> -> NULL"<<endl;
        
    }
    void sortToEdgeList ()
    {
        int i, j, cost;
        while ((*infile)>>i&& *infile>>j && *infile>>cost)
        {
            //step1
            insertEdge(new undirectedEdge(i, j, cost), edgeListHead);
            //step2
            printList(edgeListHead, outfile2);
        }   //step3
    }
    void printMST (ofstream * outfile){
        undirectedEdge *walker=MSTofG;
        while (walker->next != NULL){
            walker=walker->next;
            *outfile<< walker->ni<< " "<<walker->nj<<" "<<walker->edgeCost<<endl;
        }
    }
    void start ()
    {
        //step1,2,3
        sortToEdgeList();
        while (numSets>1){
            //step4
            nextEdge=removeEdge(edgeListHead);
            //step5 repeat 4 until ni and nj are in different sets
            while (nextEdge!=NULL && inWhichSet[nextEdge->ni]==inWhichSet[nextEdge->nj]){
                nextEdge = removeEdge(edgeListHead);
                if (nextEdge==NULL){
                    cout<<"edgeListHead is out of bound, empty"<<endl;
                    return;
                }
            }
            //step 6
            pushEdge(nextEdge, MSTofG);
            totalMSTCost+=(nextEdge->edgeCost);
            merge2Sets(nextEdge->ni, nextEdge->nj);
            numSets--;
            printSet(inWhichSet);
            //step 7
            *outfile2<<"MSTofG: ";
            printList(MSTofG, outfile2);
            *outfile2<<endl;
        }//step 8, repeat 4~7 until numSets==1
        //step9
        *outfile1<<"numNodes: "<<numNodes<<endl;
        *outfile1<<"MSTofG: "<<endl;
        printMST(outfile1);
        *outfile1<< "totalMSTCost: "<<totalMSTCost<<endl;
    }
};

int main(int argc, const char * argv[]) {
    ifstream infile;
    ofstream outfile1;
    ofstream outfile2;
    int numNodes;
    KruskalMST *kruskalMST;
    if (argc<4)
    {
        cout << "Program terminited, Please try again and supply required 1 input and 2 output files." << endl;
        return 0;
    }
    
    try
    {
        infile.open(argv[1]);
        outfile1.open(argv[2]);
        outfile2.open(argv[3]);
        cout<< "Files open successful"<<endl;
        cout<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<endl;
    }
    catch (const exception *e)
    {
        cout << "Problems with IO files.";
        return(0);
    }

    infile >> numNodes;
    cout<<"numNodes = "<<numNodes<<endl;
    kruskalMST = new KruskalMST(numNodes,&infile, &outfile1, &outfile2);
    cout<< "kruskalMST start"<<endl;
    kruskalMST->start();
    
    infile.close();
    outfile1.close();
    outfile2.close();
    return 0;
}
