
#include <iostream>
#include "DecisionTree.h"

using namespace std;
 
int main(int argc, const char * argv[]) {
    
    DecisionTree * d = new DecisionTree;
    d->train( "/test_data.txt" ,473, 21);
    
    
    d->print(d->root, 0);
    cout << endl << "Test Result: " << d->test( "/train_data.txt", 498);

    return 0;
}

