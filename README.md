# Decision-Tree

**(a)** [5 points] Implement a function for calculating the entropy of a node by using the set of samples at that node. You only need the number of observations that belong to each class. The function must return the calculated entropy. The prototype of the function must be the following:
• double calculateEntropy(const int* classCounts, const int numClasses);

**(b)** [10 points] Implement a function for calculating the information gain for a particular split at a node. This function should call the calculateEntropy function you imple- mented in the previous step. The prototype of the function must be the following:
• double calculateInformationGain(const bool** data, const int* labels, const int numSamples, const int numFeatures, const bool* usedSamples, const int featureId);
data is a boolean array with numSamples rows and numFeatures columns, labels is a vector of length numSamples, usedSamples is also a vector of length numSamples, and featureId is the column id (between 0 and numFeatures-1) of the considered feature in the data array. usedSamples indicates, for each sample, whether that sample reached that node or not. In other words, the samples whose values are true are in the subset that is used for the decision at that particular node.

**(c)** Define two classes named DecisionTree and DecisionTreeNode. Name the files as DecisionTreeNode.h, DecisionTreeNode.cpp, DecisionTree.h, and DecisionTree.cpp. DecisionTreeNode class must keep track of whether or not the node is a leaf node,
the feature index for its decision (split) if it is a non-leaf node, its class decision if it
is a leaf node, and pointers to its children. The data members and functions you will implement for this purpose is up to you.
DecisionTree class must keep a pointer to the root DecisionTreeNode, and has the public functions listed below. You can define any number of private data members and private class functions. However, you cannot modify the prototypes for the functions listed below.
• void DecisionTree::train(const bool**, const int*, const int, const int);
• void DecisionTree::train(const string, const int, const int);
• int DecisionTree::predict(const bool*);
• double DecisionTree::test(const bool**, const int*, const int);
• double DecisionTree::test(const string, const int);
• void DecisionTree::print();

**(d)** [25 points] Implement the train functions.
• void DecisionTree::train(const bool** data, const int* labels, const int numSamples, const int numFeatures);
data is a boolean array with numSamples rows and numFeatures columns, labels is a vector of length numSamples. Together, they define the training data set. Using this data set, the function must build the decision tree. The calculateInformationGain function you implemented earlier should be used here.
• void DecisionTree::train(const string fileName, const int numSamples, const int numFeatures);
Opens and parses the text file fileName that contains numSamples lines, each of which represents a sample (observation). Each observation is given by numFeatures+1 space-separated numbers. The first numFeatures numbers are binary (0 or 1) numbers indicating the feature values. The last number is an integer correspond-
ing to the class label for that observation. The class numbers are between 1 and N where N is the number of classes (the largest number in this column). After parsing the text input, the above train function must be called for the actual training.

**(e)** [15 points] Implement the predict function.
• int DecisionTree::predict(const bool* data):
This function can only be called after train is called. data is a boolean array of size numFeatures (numFeatures is already known by this point), and it rep- resents a single observation. The function must return the integer class label assigned to the input observation by the decision tree.

**(f)** [5 points] Implement the test functions.
• double DecisionTree::test(const bool** data, const int* labels, const int numSamples);
This function can only be called after train is called. data is a boolean array with numSamples rows and numFeatures columns, labels is a vector of length numSamples. Together they define a test data set for measuring the accuracy of the decision tree classifier. Note that the training data and test data can have dif- ferent number of samples (numSamples). This function must utilize the predict function to predict the class of each observation in the test data set, compare the predictions with the true classes of the observations, and return the accuracy (proportion of true predictions among the total number of observations).
• double DecisionTree::test(const string fileName, const int numSamples); Opens and parses a text file fileName. The specification of the text file is the
same with the one described in the train function. After parsing the text input,
the above test function must be called for actual testing.

**(g)** [10 points] Implement the print function. The function prints the nodes of the tree using preorder traversal. Each node should be printed on a separate line. In the beginning of each line, put a number of tabs according to the level of the node being printed (e.g., the root node has no tabs and a node at level 5 has 4 tabs). The indentation will help the user understand the printed tree structure. If the node is a leaf node, the integer corresponding to the selected class is printed as “class=x” (e.g., for the second class, “class=2” is printed). For non-leaf nodes, the integer feature index used for the split at that node is printed. Note that the feature indices are zero-based but the class numbers start from 1.
