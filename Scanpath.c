#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

    BERKANT TUGBERK DEMIRTAS
            2315232

*/

struct graphHead{
	int count; //Number of
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



int main(){

    char nameOfFile[20]; // Stores file name
    struct graphHead *head; // Graph head

    printf("Enter a file name >");
    gets(nameOfFile);

    head=createVertices(nameOfFile);
    createEdges(head,nameOfFile);
    printSimilarScanpaths(head);

    return 0;
}



// This function creates vertices according to the scanpaths
struct graphHead * createGraph(){
	struct graphHead *head = (struct graphHead *)malloc(sizeof(struct graphHead));
        head->count = 0;
        head->first = NULL;

	return head;
}


// This function inserts the vertex
void insertVertex(struct graphHead * head, char dataIn){
	struct graphVertex *vertex = (struct graphVertex *)malloc(sizeof(struct graphVertex));
	vertex->next = NULL;
	vertex->data = dataIn;
	vertex->inDegree = 0;
	vertex->outDegree = 0;
	vertex->firstArc = NULL;
	head->count++;
	if(head->first == NULL){
		head->first = vertex;
	}
	else{
		struct graphVertex *temp = head->first;
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = vertex;
	}
}

// This function inserts the edges
int insertArc(struct graphHead *head, int fromKey, int toKey, float weight){
	struct graphArc *arc = (struct graphArc *) malloc(sizeof(struct graphArc));
	struct graphVertex *fromVertex = NULL, *toVertex = NULL;

	struct graphArc *tempArc = NULL;
	fromVertex = head->first;
	while(fromVertex != NULL && fromVertex->data != fromKey)
		fromVertex = fromVertex->next;

	if (fromVertex ==NULL)
		return -2; //fromKey Not found

	toVertex = head->first;
	while(toVertex != NULL && toVertex->data != toKey)
		toVertex = toVertex->next;

	if (toVertex ==NULL)
		return -3; //toKey Not found

	fromVertex->outDegree++;
	toVertex->inDegree++;

	arc->destination = toVertex;

	tempArc = fromVertex->firstArc;
	if (tempArc == NULL){
		fromVertex->firstArc = arc;
		fromVertex->firstArc->weight=weight; // stores the similarity
		fromVertex->firstArc->from = fromKey; //stores the from node
		fromVertex->firstArc->to = toKey; // stores to to node
		arc->next = NULL;
		return 1; //Success as the first arc
	}
	else{
		while(tempArc->next != NULL){
			tempArc = tempArc->next;
		}
		tempArc->next = arc;
		tempArc->weight=weight; // stores the similarity
		tempArc->from = fromKey; //stores the from node
		tempArc->to = toKey; // stores to to node
		arc->next= NULL;
		return 1; //Success as the last arc
	}
}


// This function creates vertices according to the scanpaths

struct graphHead * createVertices(char nameOfFile[20]){

    char ch; // Variable for reading file char by char
    int count=0; //Counts the number of scanpath
    FILE *infile;
    infile=fopen(nameOfFile,"r"); //OPENS FILE AND CHECKS IF IT'S EXIT


    // This loop below counts the number of scanpaths. When there is a new line, that means that it is a new scanpath.
    while((ch = fgetc(infile)) != EOF){
        if(ch=='\n')
            count+=1;
    }


    struct graphHead *head = createGraph();

    fclose(infile);

    //This loop insert vertices
    for(int i=0;i<count;i++)
        insertVertex(head,i+1);

    return head;

}

// This function create Edges according to the similarity of scanpaths

void createEdges(struct graphHead * head, char nameOfFile[20]){

    char ch; //character for reading the file char by char
    int count=0; // for counting how many paths we have
    FILE *infile; // file stream
    infile=fopen(nameOfFile,"r"); //OPENS FILE AND CHECKS IF IT'S EXIT


    // Read the files and find the number of paths
    while((ch = fgetc(infile)) != EOF){
        if(ch=='\n') // Each line we have a path. If we count how many new lines, we can find the number
            count+=1;
    }

    fclose(infile);
    infile=fopen(nameOfFile,"r"); //OPENS FILE AND CHECKS IF IT'S EXIT

    char lineData[50]; // Temporary array for storing the line data
    int i,j; // variables for loops


    struct data *d; // Struct for storing all the path details
    d = (struct data *) malloc(sizeof(struct data)*count);

   initializeTempScanpath(lineData); // Initialize the array


   // The loop below reads the file char by char till new line and stores it into struct for creating to store path details
    for(i=0; i<count;i++){

        for(j=0; (ch = fgetc(infile)) != '\n' ; j++){
            lineData[j]=ch;
        }

        strcpy(d[i].c,lineData); //store the path into struct
        d[i].sizeOfText=j; // store the length of path
        initializeTempScanpath(lineData); //initialise the array again to avoid mistakes
    }


    int dist[(count*(count-1))/2]; // Array that includes calculated distance
    int control=0; // control variable
    float similarity; // Variable for calculating the similarity
    int n; // Variable for maksimum length of path


    // The loop before calculates the distance , and similarity. Then insert the edges

   for (i=0;i<count-1;i++){

    for(j=i;j<count;j++){

        if(i<j){ // Condition for not repeating the pairs like in handshake problem ( (1,0)=(0,1) )

            dist[control]=editDist(d[i].c,d[j].c,d[i].sizeOfText,d[j].sizeOfText); // call the function with the scanpaths in data struct d
            n = maks(d[i].sizeOfText,d[j].sizeOfText); // get the maksimum length of paths in order to calculare similarity
            similarity = (100 *(1.0-((float)dist[control]/(float)n))); //Calculate the similarity

            //If it is more than 50, insert it as edge

            if(similarity>=50.0){

                insertArc(head, i+1, j+1, similarity);
            }

            control++;


        }

    }


   }



}


// Displays the graph

void displayGraph(struct graphHead * head){
	struct graphVertex *tempVertex = NULL;
	struct graphArc * tempArc = NULL;

	tempVertex = head->first;
	while(tempVertex != NULL){
		printf("Vertex (Data: %d, inDegree: %d, outDegree: %d)\n", tempVertex->data, tempVertex->inDegree, tempVertex->outDegree);
		tempArc = tempVertex->firstArc;
		printf("Arcs:\n");
		while(tempArc != NULL){
			printf("Destination (Data: %d, inDegree: %d, outDegree: %d, Weight: %f)\n", tempArc->destination->data, tempArc->destination->inDegree, tempArc->destination->outDegree, tempArc->weight);
			tempArc = tempArc->next;
		}
		printf("\n");
		tempVertex = tempVertex->next;
	}


}

// Initialize the temporary arrays in createEdges fuction for doing easier operations

void initializeTempScanpath(char * scanpath){

    for(int i=0;i<50;i++){
        scanpath[i]='-';
    }

}

// Find the maksimum
int maks(int number1,int number2){

    if(number1>number2)
        return number1;
    return number2;


}

// Sort the list in ascending order

float * sortTheList(float *a,int n){

    int i,j;
    float temp;

     for(i=0;i<n;i++)
{
    for(j=0;j<n-i-1;j++)
    {
        if(a[j]<a[j+1])
        {
            temp=a[j+1];//swaping element
            a[j+1]=a[j];
            a[j]=temp;
        }
    }
}

}

// Find the minimum number

int min(int num1, int num2, int num3){

    if(num1 < num2 && num1 < num3)
	{
		return num1;
	}
	else if(num2 < num3)
	{
		return num2;
	}
	return num3;
}

// Find distance

int editDist(char s1[], char s2[], int m, int n) {
if (m == 0)
    return n;
if (n == 0)
    return m;
if (s1[m - 1] == s2[n - 1])
    return editDist (s1, s2, m - 1, n - 1);

return 1 + min(editDist(s1, s2, m, n - 1), editDist(s1, s2, m - 1, n), editDist(s1, s2, m - 1, n - 1));
}


// Prints the scanpaths by their similarities

void printSimilarScanpaths(struct graphHead *head){

    // Temporary structs for the operations
   struct graphVertex *tempVertex = NULL;
   struct graphArc * tempArc = NULL;
   struct graphHead * tempp =head;

   int i=0; // For the controlling loop

    tempVertex = head->first;

   float weight[head->count]; // Stores the weights

    //This loop traverse the graph and gets the edge weights into array
	while(tempVertex != NULL){

		tempArc = tempVertex->firstArc;
		while(tempArc != NULL){
            if(tempArc->weight>=50){

                weight[i]=tempArc->weight;
                tempArc = tempArc->next;
                i++;
            }

		}
		tempVertex = tempVertex->next;
	}

    sortTheList(weight,i); // Sorts the list

    // Initialize the structs again for traversing;

   tempVertex = tempp->first;
   tempArc = NULL;

    int control=0; // control variable for controlling loop



    // This loop prints the similart paths

    for(int j=0;tempVertex!=NULL;j++){
        tempArc = tempVertex->firstArc;

        while(tempArc != NULL){
            if(tempArc->weight==weight[control]){ // If weight of edge of the graph is equal the ordered list weights, print them
			printf("Scanpath: %d and Scanpath: %d - Similarity: %f\n", tempArc->from, tempArc->to,tempArc->weight);
			control+=1;
			tempVertex = tempp->first; // For the go beginning of the loop because we find the first value
			tempArc = tempVertex->firstArc;
            }
            else{

                tempArc = tempArc->next;
            }
		}

		tempVertex = tempVertex->next;

    }

}
