
#ifndef DecisionTreeNode_h


#include <iostream>


class DecisionTreeNode {     // a node in the tree
private:
    DecisionTreeNode();
    DecisionTreeNode( int featureIndex, int classDecision, bool isLeaf, DecisionTreeNode *leftChildPtr, DecisionTreeNode *rightChildPtr );
    
    
    DecisionTreeNode *leftChildPtr;    // pointers to children
    DecisionTreeNode *rightChildPtr;
    bool isLeaf;
    int featureIndex; //if it is a non-leaf node
    int classDecision; //if it is a leaf node
    void setAsLeaf( int classNo );
    
    // friend class - can access private parts
    friend class DecisionTree;
    
};

#define DecisionTreeNode_h


#endif /* DecisionTreeNode_h */
