# SimpleScanpathAnalyser

Eye tracking does not only allow us to understand which areas of interest (AOIs) of visual stimuli take the most attention, but also allows us to understand in which order these AOIs are used. Figure 1 shows an example of an eye movement sequence (i.e. scanpath) of a particular person on the home page of the Babylon website.


![Figure 1: ](https://github.com/berkanttubi/SimpleScanpathAnalyser/blob/main/Figure%201.png "Figure 1")




The process model of this program is as follows:

![Figure 2: ](https://github.com/berkanttubi/SimpleScanpathAnalyser/blob/main/process.png "Path")

**1) Input : User enters the name of the file.**
**2) Internal Processing : Graph is created according to the scanpaths similarities in the given scanpath txt file.**
**3) Output : Prints the similiar scanpath in ASC order.**


##### Structs of the program is given below:

``` C

struct graphHead{
	int count; //Number of node
	struct graphVertex *first;
};

struct graphVertex{
	struct graphVertex *next;
	int data ; //Data to save in a node/vertex
	int inDegree;
	int outDegree;
	struct graphArc *firstArc;
};

struct graphArc{
	struct graphVertex *destination;
	struct graphArc *next;
	float weight; // Stores the weight of edge
	int from; // Stores the node from
	int to;// Stores the node to
};

// This struct is using when file reading operation happens. Stores the scanpaths data here
struct data{
    char c[50]; // Stores the each scanpath
    int sizeOfText; // Stores the size of scanpath
};


```
##### Funcitons of the program is given below:

``` c

struct graphHead * createGraph(); // Initialize the head of graph
void displayGraph(struct graphHead *);
void insertVertex(struct graphHead *, char);
int insertArc(struct graphHead *,int,int,float);
struct graphHead* createVertices(char *); // This function creates vertices according to the scanpaths
void createEdges(struct graphHead *,char *); // This function create Edges according to the similarity of scanpaths
void initializeTempScanpath(char*); // The purpose of this function initializing temporary arrays in createEdges() funtion
int min (int,int,int); // Finds the minimum of three
int maks (int,int); // Find maksimum of two
float * sortTheList (float *,int); // Finds the maksimum in float list. Used for sorting the similarities of scanpahts
int editDist(char *, char *, int , int ); // Function given in assignment
void printSimilarScanpaths(struct graphHead *); //Prints the similar scanpaths by the order


```

