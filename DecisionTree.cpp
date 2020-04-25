
#include "DecisionTree.h"
#include <cmath>
#include <fstream>
#include <sstream>

DecisionTree::DecisionTree() : root( NULL ){
}

void DecisionTree::addLastlr(int *leftChildClassCount, int *rightChildClassCount, int numFeatures) {
    
    this->leftChildClassCount = new int[numFeatures];
    this->rightChildClassCount = new int[numFeatures];
    
    for( int i = 0; i < numFeatures; i++ ) {
        
        this->leftChildClassCount[i] = leftChildClassCount[i];
        this->rightChildClassCount[i] = rightChildClassCount[i];
        
    }
    
    
}

void DecisionTree::print(DecisionTreeNode* curr, int level) {
    if (curr == NULL) {
        return;
    }
    
    for (int i = 0; i < level; i++) {
        cout << '\t';
    }
    
    if (curr->isLeaf) {
        cout << "class=";
        cout << curr->classDecision << endl;
    } else {
        cout << curr->featureIndex << endl;
    }
    
    
    print(curr->leftChildPtr, level + 1);
    print(curr->rightChildPtr, level + 1);
}

bool allSamplesUsed( const bool * leftUsedSamples, const bool * rightUsedSamples, const bool numSamples ) {
    
    for( int i = 0; i < numSamples; i++ ) {
        if( !leftUsedSamples[i] || !rightUsedSamples[i] ) {
            return false;
        }
        
    }
    return true;
    
}


bool DecisionTree::isEmpty() const {
    return root == NULL;
}


double calculateEntropy(const int* classCounts, const int numClasses) {
    
    double sum = 0;
    double result = 0;
    for( int i = 0; i < numClasses; i++ ) {
        sum += classCounts[i];
        
    }
    
    double num;
    for( int j = 0; j < numClasses; j++ ) {
        if( classCounts[j] == 0 ) {
            continue;
        }
        num = ( classCounts[j] / sum );
        result += num * log2( num );
    }
    
    result = result * -1.0;
    return result;
    
}

double DecisionTree::calculateInformationGain(const bool** data, const int* labels,
                                const int numSamples, const int numFeatures, const bool* usedSamples, const int featureId) {
    
    int * leftClassCount = new int[numFeatures];  //index 0: class1 | index 1: class2 | index 2: class3
    int * rightClassCount = new int[numFeatures]; //index 0: class1 | index 1: class2 | index 2: class3
    int * parentClassCount = new int[numFeatures];
    
    for( int i = 0; i < numFeatures; i++ ) {
        leftClassCount[i] = 0;
        rightClassCount[i] = 0;
        parentClassCount[i] = 0;
    }
    
    
    
    int rightSampleSize = 0, leftSampleSize = 0;
    int totalSampleSize;
    
    for( int i = 0; i < numSamples; i++ ) {
        if( usedSamples[i] ) { //if sample is used, skip
            continue;
        } else {
            for( int j = 1; j < numFeatures + 1; j++ ) {
                if( labels[i] == j ) {
                    
                    parentClassCount[j - 1]++;
                    
                    if(data[i][featureId]) {
                        rightSampleSize++;
                        rightClassCount[j - 1]++;
                    } else {
                        leftSampleSize++;
                        leftClassCount[j - 1]++;
                    }
                }
            }
        }
    }
    
    
    totalSampleSize = rightSampleSize + leftSampleSize;
    
    //call calculate entropy for left child
    double entropyLeft = calculateEntropy(leftClassCount, numFeatures);
    
    //call calculate entropy for right child
    double entropyRight = calculateEntropy(rightClassCount, numFeatures);
    
    //call calculate entropy for parent
    double entropyParent = calculateEntropy(parentClassCount, numFeatures);
    
    //info gain = entropy of parent - weighted sum of the entropy of the child nodes
    double wSum = entropyLeft * ((double)leftSampleSize / totalSampleSize) +
    entropyRight * ((double)rightSampleSize / totalSampleSize);
    
    double infoGain = entropyParent - wSum;
    
    delete [] leftClassCount;
    delete [] rightClassCount;
    delete [] parentClassCount;
    
    return infoGain;
}


bool stringToBool(string s) {
    
    if( s == "1" ) {
        return true;
    } else {
        return false;
    }
    
}

int stringToInt(string s) {
    
    stringstream si(s);
    int a = 0;
    si >> a;
    return a;
}


void DecisionTree::parseFile(string file, bool **& data, int *& labels ) {
    
    ifstream inFile;
    inFile.open(file.c_str());
    
    
    string line;
    int line_count = 0;
    
    // Loop to get each line in file
    while(getline(inFile, line)) {
        stringstream ss(line);
        string value;
        int word_count = 0;
        
        // Loop to get each word in line
        while(getline(ss, value, ' ') ) {
            if(word_count == numFeatures) {
                //Getting labels
                labels[line_count] = stringToInt(value);
                word_count++;
                break;
                
            } else {
                data[line_count][word_count] = stringToBool(value);
                word_count++;
            }
        }
        line_count++;
    }
    
}

int DecisionTree::getBestFeature(const bool **data, const int *labels,
                                 const int numSamples, const int numFeatures, const bool * usedSamples, const bool * usedFeatures,
                                 int *& leftChildClassCount, int *& rightChildClassCount, bool & allZero ) {
    allZero = false;
    
    int * classCounts = new int[numFeatures];
    
    int zeros = 0;
    int ones = 0;
    
    
    for( int i = 0; i < numSamples; i++ ) {
        if( !usedSamples[i] ) { //if sample is used, skip
            for( int j = 1; j < numFeatures + 1; j++ ) {
                if( labels[i] == j ) {
                    classCounts[j - 1]++;
                }
            }
        }
    }
    
    int bestFeature = 0;
    //select which feature we want to start at the current node
    double biggestInfoGain = 0;
    for( int j = 0; j < numFeatures; j++ ) {
        if( !usedFeatures[j] ) {
            double infoGain = calculateInformationGain( (const bool**)data,
                                                       labels, numSamples, numFeatures, usedSamples, j);
            if( infoGain == 0 ) {
                zeros++;
            } else {
                ones++;
            }
            
            if( infoGain > biggestInfoGain ) {
                biggestInfoGain = infoGain;
                bestFeature = j; //we want to split at this featureId
                
            }
        }
    }
    
    if( ( (double)ones / zeros ) < 5 ) {
        allZero = true;
    }
    
    for( int i = 0; i < numSamples; i++ ) {
        if( usedSamples[i] ) { //if sample is used, skip
            continue;
        } else {
            for( int j = 1; j < numFeatures + 1; j++ ) {
                if( labels[i] == j ) {
                    if(data[i][bestFeature]) {
                        rightChildClassCount[j - 1]++;
                    } else {
                        leftChildClassCount[j - 1]++;
                    }
                }
            }
        }
    }
    
    delete [] classCounts;
    return bestFeature;
}


int DecisionTree::getHighestClassAmount( int * cc ) {
    
    int biggest = cc[0];
    int cno = 1;
    
    for( int i = 1; i < numFeatures; i++ ) {
        if(cc[i] > biggest) {
            biggest = cc[i];
            cno = i+1;
        }
    }
    
    return cno;
    
}


//Returns the purity of both left child and right child
void isPure( const bool **data, const int *labels, const int featureId,
            const int numSamples, const bool * leftUsedSamples, const bool * rightUsedSamples, bool & rightPure,
            bool & leftPure, int & classOfRightChild, int & classOfLeftChild, int numFeatures) {
    
    int * ccRight = new int[numFeatures];  //class counts for right child
    int * ccLeft = new int[numFeatures]; //class counts for left child
    
    for( int p = 0; p < numFeatures; p++) {
        ccRight[p] = 0;
        ccLeft[p] = 0;
    }
    
    for( int i = 0; i < numSamples; i++ ) {
        for( int j = 1; j < numFeatures+ 1; j++ ) {
            
            if( rightUsedSamples[i] ) {
                if( labels[i] == j ) {
                    ccRight[j-1]++;
                }
            }
            //else if???????
            else if( leftUsedSamples[i] ) {
                if( labels[i] == j ) {
                    ccLeft[j-1]++;
                }
            }
            
            
        }
        
    }
    
    //Get class names for leaf nodes
    for( int k = 0; k < numFeatures; k++ ) {
        if (ccRight[classOfRightChild-1] < ccRight[k]) {
            classOfRightChild = k+1;
        }
        
        if (ccRight[classOfLeftChild-1] < ccLeft[k]) {
            classOfLeftChild = k+1;
        }
        
    }
    
    
    if( calculateEntropy(ccRight, numFeatures) == 0 ) { //if the entropy of right child is zero
        rightPure = true;
    } else if( calculateEntropy(ccLeft, numFeatures) == 0 ) { //if the entropy of left child is zero
        leftPure = true;
    } else if((calculateEntropy(ccLeft, numFeatures) == 0) && (calculateEntropy(ccRight, numFeatures) == 0)){
        rightPure = true;
        leftPure = true;
    } else {
        rightPure = false;
        leftPure = false;
        classOfLeftChild = -1;
        classOfRightChild = -1;
    }
    
    delete [] ccLeft;
    delete [] ccRight;
}



bool noFeatureLeft( bool * usedFeatures, const int numFeatures ) {
    for( int i = 0; i < numFeatures; i++) {
        if( !usedFeatures[i] ) {
            return false;
        }
    }
    return true;
}

void DecisionTree::split( const bool ** data, const int featureId, const int numSamples,
                         const int numFeatures, DecisionTreeNode * root, const int * labels,
                         bool *& leftUsedSamples, bool *& rightUsedSamples, bool *& usedFeatures ) {
    
    int leftMajority, rightMajority;
    
    leftMajority = getHighestClassAmount(leftChildClassCount);
    rightMajority = getHighestClassAmount(rightChildClassCount);
    
    if( allZero || noFeatureLeft(usedFeatures, numFeatures) || allSamplesUsed(leftUsedSamples, rightUsedSamples, numSamples)  ) { //|| root->isLeaf
        
        bool rightPure, leftPure;
        int rightClass = 1, leftClass = 1;
        
        
        isPure( data, labels, featureId, numSamples, leftUsedSamples,
               rightUsedSamples, rightPure, leftPure, rightClass, leftClass, numFeatures );
        
        if( rightPure ) {
            root->rightChildPtr = new DecisionTreeNode( -1, rightClass, true, NULL, NULL ); //init right child that is leaf
            root->leftChildPtr = new DecisionTreeNode( -1, leftMajority, true, NULL, NULL ); //init right child that is leaf
            
        } else if( leftPure ) {
            root->leftChildPtr = new DecisionTreeNode( -1, leftClass, true, NULL, NULL ); //init left child that is leaf
            root->rightChildPtr = new DecisionTreeNode( -1, rightMajority, true, NULL, NULL ); //init right child that is leaf
            
        } else if( rightPure && leftPure ) {
            root->rightChildPtr = new DecisionTreeNode( -1, rightClass, true, NULL, NULL ); //init right child that is leaf
            root->leftChildPtr = new DecisionTreeNode( -1, leftClass, true, NULL, NULL ); //init left child that is leaf
            
        } else { //If none of them are pure, their class num is majority
            root->leftChildPtr = new DecisionTreeNode( -1, leftMajority, true, NULL, NULL ); //init right child that is leaf
            root->rightChildPtr = new DecisionTreeNode( -1, rightMajority, true, NULL, NULL ); //init right child that is leaf
        }
        
        return;
    }
    
    int * rightChildClassCount = new int[numFeatures];
    int * leftChildClassCount = new int[numFeatures];
    
    for( int i = 0; i < numFeatures; i++ ) {
        rightChildClassCount[i] = 0;
        leftChildClassCount[i] = 0;
    }
    
    int rightBestFeatureId;
    int leftBestFeatureId;
    
    bool leftPure = false; //Flag that represent purity of left child
    bool rightPure = false; //Flag that represent purity of right child
    
    int leftClass = 1; //class of the left child if pure
    int rightClass = 1; //class of the right child if pure
    
    
    //get which child is pure(if they are, return the class numbers of them)
    isPure( data, labels, featureId, numSamples, leftUsedSamples,
           rightUsedSamples, rightPure, leftPure, rightClass, leftClass, numFeatures );
    
    if( leftPure && rightPure ) { //Both left child and right child are pure
        
        root->leftChildPtr = new DecisionTreeNode( -1, leftClass, true, NULL, NULL ); //init left child that is leaf
        root->rightChildPtr = new DecisionTreeNode( -1, rightClass, true, NULL, NULL ); //init right child that is leaf
        
    } else if( rightPure ) {
        
        root->rightChildPtr = new DecisionTreeNode( -1, rightClass, true, NULL, NULL ); //init right child that is leaf
        
        //find best featureId for the left child
        leftBestFeatureId = getBestFeature(data, labels, numSamples, numFeatures, rightUsedSamples, usedFeatures, leftChildClassCount, rightChildClassCount, allZero);
        
        usedFeatures[leftBestFeatureId] = true;
        
        root->leftChildPtr = new DecisionTreeNode( leftBestFeatureId, -1, false, NULL, NULL ); //init right child that is non-leaf
        
        //update rightUsedSamples
        for( int p = 0; p < numSamples; p++ ) {
            if(data[p][leftBestFeatureId]) {
                leftUsedSamples[p] = true;
            }
        }
        
        addLastlr(leftChildClassCount, rightChildClassCount, numFeatures);
        
        
        //call recursively for left children
        split(data, root->leftChildPtr->featureIndex, numSamples, numFeatures,
              root->leftChildPtr, labels, leftUsedSamples, rightUsedSamples, usedFeatures);
        
        
    } else if( leftPure ) {
        
        //init left child that is leaf
        root->leftChildPtr = new DecisionTreeNode( -1, leftClass, true, NULL, NULL ); //init left child that is leaf
        
        //find best featureId for the right child
        rightBestFeatureId = getBestFeature(data, labels, numSamples, numFeatures, leftUsedSamples, usedFeatures, leftChildClassCount, rightChildClassCount, allZero);
        
        usedFeatures[rightBestFeatureId] = true;
        root->rightChildPtr = new DecisionTreeNode( rightBestFeatureId, -1, false, NULL, NULL ); //init right child that is non-leaf
        
        //update rightUsedSamples
        for( int p = 0; p < numSamples; p++ ) {
            if(data[p][rightBestFeatureId]) {
                rightUsedSamples[p] = true;
            }
        }
        
        addLastlr(leftChildClassCount, rightChildClassCount, numFeatures);
        
        //call recursively for right children
        split(data, root->rightChildPtr->featureIndex, numSamples, numFeatures,
              root->rightChildPtr, labels, leftUsedSamples, rightUsedSamples, usedFeatures);
        
    } else {
        
        //Case where both left and right children are non-leaf
        rightBestFeatureId = getBestFeature(data, labels, numSamples, numFeatures, rightUsedSamples, usedFeatures, leftChildClassCount, rightChildClassCount, allZero);
        leftBestFeatureId = getBestFeature(data, labels, numSamples, numFeatures, leftUsedSamples, usedFeatures, leftChildClassCount, rightChildClassCount, allZero);
        
        usedFeatures[rightBestFeatureId] = true;
        usedFeatures[leftBestFeatureId] = true;
        
        root->leftChildPtr = new DecisionTreeNode( leftBestFeatureId, -1, false, NULL, NULL ); //init right child that is non-leaf
        root->rightChildPtr = new DecisionTreeNode( rightBestFeatureId, -1, false, NULL, NULL ); //init right child that is non-leaf
        
        //update rightUsedSamples and leftUsedSamples
        for( int p = 0; p < numSamples; p++ ) {
            if(data[p][rightBestFeatureId]) {
                rightUsedSamples[p] = true;
            } else {
                leftUsedSamples[p] = true;
            }
        }
        
        addLastlr(leftChildClassCount, rightChildClassCount, numFeatures);
        //call recursively for right children
        split(data, root->rightChildPtr->featureIndex, numSamples, numFeatures,
              root->rightChildPtr, labels, leftUsedSamples, rightUsedSamples, usedFeatures);
        
        //call recursively for left children
        split(data, root->leftChildPtr->featureIndex, numSamples, numFeatures,
              root->leftChildPtr, labels, leftUsedSamples, rightUsedSamples, usedFeatures);
        
    }
    
    delete [] rightChildClassCount;
    delete [] leftChildClassCount;
    
}


void DecisionTree::train(const bool **data, const int *labels, const int numSamples, const int numFeatures) {
    
    //init used samples array
    bool * leftUsedSamples = new bool[numSamples];
    bool * rightUsedSamples = new bool[numSamples];
    bool * usedFeatures = new bool[numFeatures];
    
    bool allZero = false;
    
    for( int i = 0; i < numSamples; i++ ) {
        leftUsedSamples[i] = 0;
        rightUsedSamples[i] = 0;
    }
    
    for( int i = 0; i < numFeatures; i++ ) {
        
        usedFeatures[i] = false;
        
    }
    
    int featureId;
    int * rightChildClassCount = new int[numFeatures];
    int * leftChildClassCount = new int[numFeatures];
    
    //find featureId for the root node
    featureId = getBestFeature(data, labels, numSamples, numFeatures, leftUsedSamples,
                               usedFeatures, leftChildClassCount, rightChildClassCount, allZero);
    
    addLastlr(leftChildClassCount, rightChildClassCount, numFeatures);
    
    root = new DecisionTreeNode( featureId, -1, false, NULL, NULL ); //init root
    
    //update rightUsedSamples and leftUsedSamples
    for( int p = 0; p < numSamples; p++ ) {
        if(data[p][featureId]) {
            rightUsedSamples[p] = true;
        } else {
            leftUsedSamples[p] = true;
        }
    }
    
    usedFeatures[featureId] = true;
    
    //call recursive split function from the root node
    split( data, featureId, numSamples, numFeatures, root, labels, leftUsedSamples, rightUsedSamples, usedFeatures );
    
    
    delete [] leftUsedSamples;
    delete [] rightUsedSamples;
    delete [] usedFeatures;
    delete [] rightChildClassCount;
    delete [] leftChildClassCount;
    
}

void DecisionTree::train(const string path, const int numSamples, const int numFeatures) {
    
    this->numFeatures = numFeatures;
    this->numSamples = numSamples;
    
    //init data array
    bool ** data = new bool * [ numSamples ];
    for(int i = 0; i < numSamples; i++) {
        data[i] = new bool[numFeatures];
    }
    
    int * labels = new int[numSamples];
    
    //DO NOT FORGET TO DELETE THESE AFTERWARDS !*!*!*!*!*
    
    //Start by parsing the data from the text file
    parseFile( path, data, labels );
    
    //Call the actual train method
    train( (const bool**)data, labels, numSamples, numFeatures);
    
    
    //Free up memory
    delete[] labels;
    //Free each sub-array
    for(int i = 0; i < numSamples; ++i) {
        delete[] data[i];
    }
    
    //Free the array of pointers
    delete[] data;
    
}


int DecisionTree::predict(const bool* data) {
    DecisionTreeNode* current = root;
    while ( current->leftChildPtr != NULL && current->rightChildPtr != NULL ) {
        
        int featureId = current->featureIndex;
        
        if (data[featureId] == 0) {
            current = current->leftChildPtr;
        } else {
            current = current->rightChildPtr;
        }
    }
    
    return current->classDecision;
}

double DecisionTree::test(const bool** data, const int* labels, const int numSamples) {
    int count = 0;
    for (int i = 0; i < numSamples; i++)
        if (labels[i] == predict(data[i]))
            count++;
    
    return (double)count / numSamples;
}

double DecisionTree::test(const string path, const int numSamples) {
    
    
    //init data array
    bool ** data = new bool * [ numSamples ];
    for(int i = 0; i < numSamples; i++) {
        data[i] = new bool[numFeatures];
    }
    
    int * labels = new int[numSamples];
    
    //Start by parsing the data from the text file
    parseFile( path, data, labels );
    
    //Call the actual test method
    double result = test( (const bool **)data, labels, numSamples );
    
    //Free up memory
    delete[] labels;
    //Free each sub-array
    for(int i = 0; i < numSamples; ++i) {
        delete[] data[i];
    }
    
    //Free the array of pointers
    delete[] data;
    
    return result;
    
}
