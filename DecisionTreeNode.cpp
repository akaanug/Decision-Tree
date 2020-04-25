
#include "DecisionTreeNode.h"


DecisionTreeNode::DecisionTreeNode(int featureIndex, int classDecision, bool isLeaf, DecisionTreeNode *leftChildPtr, DecisionTreeNode *rightChildPtr ) {
    
    this->featureIndex = featureIndex;
    this->isLeaf = isLeaf;
    this->leftChildPtr = leftChildPtr;
    this->rightChildPtr = rightChildPtr;
    this->classDecision = classDecision;
    
}

void DecisionTreeNode::setAsLeaf( int classNo ) {
    isLeaf = true;
    classDecision = classNo;
}


