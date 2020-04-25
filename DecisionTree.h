
#ifndef DecisionTree_h
#define DecisionTree_h

#include "DecisionTreeNode.h"
#include <string>

using namespace std;

class DecisionTree {
public:
    DecisionTreeNode * root;
    
    DecisionTree(); //OK
    
    bool isEmpty() const; //OK
    
    void train(const bool** data, const int* labels, const int numSamples, const int numFeatures);
    void train(const string path, const int numSamples, const int numFeatures);
    int predict(const bool*);
    double test(const bool** data, const int* labels, const int numSamples);
    double test(const string path, const int numSamples);
    static void print(DecisionTreeNode* curr, int level);
    
    void split( const bool ** data, const int featureId, const int numSamples, const int numFeatures, DecisionTreeNode * root,
               const int * labels,
               bool *& leftUsedSamples, bool *& rightUsedSamples, bool *& usedFeatures );
    int getBestFeature(const bool **data, const int *labels, const int numSamples, const int numFeatures,
                       const bool * usedSamples, const bool * usedFeatures, int *& leftChildClassCount, int *& rightChildClassCount, bool & allZero);
    
    void addLastlr( int * leftChildClassCount, int * rightChildClassCount, int numFeatures );
    int getHighestClassAmount( int * cc );
    void parseFile(string file, bool **& data, int *& labels );
    double calculateInformationGain(const bool** data, const int* labels,
                                    const int numSamples, const int numFeatures, const bool* usedSamples, const int featureId);
    
    int * leftChildClassCount;
    int * rightChildClassCount;
    bool allZero;
    
    int numFeatures;
    int numSamples;
    
protected:
    
};

#endif /* DecisionTree_h */
