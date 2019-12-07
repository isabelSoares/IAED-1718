/* Project about sparse matrix's management
part 1

Isabel Soares 89466 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXELS 10000
#define MAXNAME 80
#define MAXINPUT 1000


typedef struct {
	unsigned long int line, column;
	double value;
} Element;

typedef struct {
	Element elements[MAXELS];
	int nelements;
} Matrix;

void addElements(unsigned long int line, unsigned long int column, double value);
void listElements();
void limits();
void density();
void listLimitsDensity();
void limits();
void listLine(unsigned long int line);
void listColumn(unsigned long int column);
int cmpfunc1 (const void * a, const void * b);
int cmpfunc2 (const void * a, const void * b);
void orderLineColumn(int mode);
void defineElzero(double v);
void saveElements();
void put_values(unsigned long int line, double line_values[], int offset, double value[], unsigned long int index[]);
unsigned long int check_dens_line(int densities[]);
void compressmatrix();


/*global variables*/
Matrix matrix;
unsigned long int mini, minj, maxi, maxj;
double zero;
char fileName[MAXNAME + 1];
float dens;
unsigned long int size;

int main(int argc, char const *argv[])
{
	unsigned long int line, column;
	double value;
	char string[MAXINPUT];
	matrix.nelements = 0;
	mini = 0;
	minj = 0;
	maxi = 0;
	maxj = 0;
	zero = 0;

	if (argc == 2) {
		/*testing if function have 2 arguments.If it has, read file*/
		strcpy(fileName, argv[1]);
		FILE *fileElements;
		fileElements = fopen(fileName, "r");
		unsigned long int lin = 0;
		unsigned long int col = 0;
		double val = 0;

		while (fscanf(fileElements, "%lu %lu %lf\n",
		              &lin, &col, &val) == 3) {
			/*while file has 3 components add this elements*/
			addElements(lin, col, val);
		}
		fclose(fileElements);
	}

	do {
		fgets(string, MAXINPUT, stdin);

		switch (string[0]) {
			case'a':
				sscanf(string, "a %lu %lu %lf", &line,  &column, &value);
				/*check line, column and value in string*/
				addElements(line, column, value);
				break;
			case 'p':
				listElements();
				break;
			case'i':
				listLimitsDensity();
				break;
			case 'l':
				sscanf(string, "l %lu", &line);
				/*check line in string*/
				listLine(line);
				break;
			case 'c':
				sscanf(string, "c %lu", &column);
				/*check column in string*/
				listColumn(column);
				break;
			case 'o':
				if (strlen(string) == 2) {
					/*if the chosen command is 'o' */
					orderLineColumn(0);
				}
				else {
					orderLineColumn(1);
					/*or 'o column'*/
				}
				break;
			case 'z':
				sscanf(string, "z %lf", &value);
				/*check value in string*/
				defineElzero(value);
				break;
			case 's':
				compressmatrix();
				break;
			case 'w':
				if (string[2] != '\0') {
					/*if input just is 'w'*/
					sscanf(string, "w %s", fileName);
				}
				saveElements();
				break;
			}
	} while (strcmp(string, "q\n"));

	return 0;
}

void addElements(unsigned long int line, unsigned long int column, double value ) {

	int i;

	int find_zero = 0;
	int find_lineCol = 1;


	if ( value != zero) {
		for (i = 0; i < matrix.nelements; i++) {
			/*if there is an element in this line
			and this column,replace it by new value*/
			if (matrix.elements[i].line == line &&
			        matrix.elements[i].column == column) {
				matrix.elements[i].value = value;
				find_lineCol = 0;
			}
		}
		if (find_lineCol == 1) {
			/*if there isn't element in this line and column*/
			matrix.elements[matrix.nelements].line = line;
			matrix.elements[matrix.nelements].column = column;
			matrix.elements[matrix.nelements].value = value;
			matrix.nelements++;
		}
	}
	else {
		for (i = 0; i < matrix.nelements; i++) {
			if (matrix.elements[i].line == line &&
			        matrix.elements[i].column == column) {
				/*if this line and this column, value is zero*/
				find_zero = 1;
			}
			else if (find_zero == 1) {
				matrix.elements[i - 1] = matrix.elements[i];
				/*make a shift left to eliminate this element that is zero*/
			}
		}
		if (find_zero == 1) {
			matrix.nelements--;
		}
	}
}


void listElements() {
	int i;
	if (matrix.nelements == 0) {
		printf("empty matrix\n");
	}

	for (i = 0; i < matrix.nelements; i++) {
		printf ("[%lu;%lu]=%.3lf\n", matrix.elements[i].line,
		        matrix.elements[i].column, matrix.elements[i].value);
	}
}


void limits() {
	int i;
	mini = matrix.elements[0].line;
	minj = matrix.elements[0].column;
	maxi = matrix.elements[0].line;
	maxj = matrix.elements[0].column;

	for (i = 0; i < matrix.nelements; i++) {
		if (matrix.elements[i].line < mini) {
			mini = matrix.elements[i].line;
		}
		if (matrix.elements[i].column < minj) {
			minj = matrix.elements[i].column;
		}
		if (matrix.elements[i].line > maxi) {
			maxi = matrix.elements[i].line;
		}
		if (matrix.elements[i].column > maxj) {
			maxj = matrix.elements[i].column;
		}
	}

}

void density() {
	limits();
	size = (maxi - mini + 1) * (maxj - minj + 1);
	dens = (matrix.nelements / ((float) size)) * 100;
}

void listLimitsDensity() {
	if (matrix.nelements == 0) {
		printf("empty matrix\n");
		return;
	}
	density();

	printf("[%lu %lu] [%lu %lu] %d / %lu = %.3lf%%\n",
	       mini, minj, maxi, maxj, matrix.nelements, size, dens);

}

void listLine(unsigned long int line) {
	int i;
	limits();
	double values[maxj - minj + 1];

	for (i = 0; i < (maxj - minj + 1); i++) {
		values[i] = zero;
	}

	int empty = 1;
	if (line >= mini && line <= maxi) {
		for (i = 0; i < matrix.nelements; i++) {
			if (matrix.elements[i].line == line) {
				values[matrix.elements[i].column - minj] = matrix.elements[i].value; 
				empty = 0;
			}
		}
		if (empty) {
			printf("empty line\n");
		}
		else {
			for (i = 0; i < (maxj - minj + 1); i++) {
				printf(" %.3lf", values[i]);
			}
			printf("\n");
		}

	}
	else {
		printf("empty line\n");
	}
}

void listColumn(unsigned long int column) {
	int i;
	limits();
	double values[maxi - mini + 1];
	for (i = 0; i < (maxi - mini + 1); i++) {
		values[i] = zero;
	}
	int empty = 1;
	if (column >= minj && column <= maxj) {

		for (i = 0; i < matrix.nelements; i++) {
			if (matrix.elements[i].column == column) {
				values[matrix.elements[i].line - mini] = matrix.elements[i].value;
				empty = 0;
			}
		}
		if (empty) {
			printf("empty column\n");
		}
		else {
			for (i = 0; i < (maxi - mini + 1); i++) {
				printf("[%lu;%lu]=%.3lf\n", i + mini, column, values[i]);
			}
		}

	}
	else {
		printf("empty column\n");
	}
}

int cmpfunc1 (const void * a, const void * b) {
	/*A function to compare the element in line of matrix */
	return ( (*(Element*)a).line - (*(Element*)b).line);
}

int cmpfunc2 (const void * a, const void * b) {
	/*A function to compare the element in colune of matrix */
	return ( (*(Element*)a).column - (*(Element*)b).column);
}


void orderLineColumn(int mode) {
	if (mode == 0) {
		/*when I order line and inside line I order column*/
		qsort(matrix.elements, matrix.nelements, sizeof(Element), cmpfunc2);
		qsort(matrix.elements, matrix.nelements, sizeof(Element), cmpfunc1);
	}
	else if (mode == 1) {
		/*when firstly I order column and after line*/
		qsort(matrix.elements, matrix.nelements, sizeof(Element), cmpfunc1);
		qsort(matrix.elements, matrix.nelements, sizeof(Element), cmpfunc2);
	}
}


void defineElzero(double v) {
	int i, j;
	zero = v;
	for (i = 0; i < matrix.nelements; ) {
		if (zero == matrix.elements[i].value) {
			for (j = i + 1; j < matrix.nelements; j++) {
				matrix.elements[j - 1] = matrix.elements[j];
				/*the element is eliminated of matrix, because now it's a zero */
			}
			matrix.nelements--;
		}
		else {
			i++;
		}
	}
}

void saveElements() {
	int i;
	FILE *fileElements;
	fileElements = fopen(fileName, "w");

	for (i = 0; i < matrix.nelements; i++) {
		fprintf(fileElements, "%lu %lu %lf\n",
		        matrix.elements[i].line, matrix.elements[i].column, matrix.elements[i].value);
	}
	fclose(fileElements);
}

void put_values(unsigned long int line, double line_values[], int offset, double value[], unsigned long int index[]) {
	/*function that put matrix's values in vector 'value' sorted by descending lines' density and put index in vector 'index'*/
	int i;
	for (i = 0; i < maxi - mini + 1; i++) {
		if (line_values[i] != zero) {
			value[i + offset] = line_values[i];
			index[i + offset] = line;
		}
	}

}

unsigned long int check_dens_line(int densities[]) {
	int i;
	unsigned long int highest_line_dens = 0;
	for (i = 0; i <= maxi; i++) {
		if (densities[i] > densities[highest_line_dens]) {
			highest_line_dens = i;
			/*check the line with more density*/
		}
		else if (densities[i] == densities[highest_line_dens]) {
			/*if there are more lines with same density*/
			if (i < highest_line_dens) {
				highest_line_dens = i;
			}
		}
	}
	return highest_line_dens;
}

void compressmatrix() {
	int i;
	density();
	int densities[maxi + 1];
	unsigned long int biggest_line;
	unsigned long int line_els = 0;
	double value[2 * matrix.nelements];
	unsigned long int index[2 * matrix.nelements];
	int offset[maxj - minj + 1];
	double save_biggest_line[maxi - mini + 1];
	int found;
	int index_limit = 0;

	if (dens > 50) {
		printf("dense matrix\n");
		return;
	}
	for (i = 0; i < 2 * matrix.nelements; i++) {
		value[i] = zero;
		index[i] = 0;
	}

	for (i = 0; i < maxi - mini + 1; i++) {
		offset[i] = 0;
	}

	for (i = 0; i <= maxi; i++) {
		densities[i] = 0;
	}

	for (i = 0; i < matrix.nelements; i++) {
		/*if line has more than one element*/
		densities[matrix.elements[i].line]++;
	}
	for (i = 0; i <= maxi; i++) {
		if (densities[i] != 0) {
			line_els++;
		}
	}

	while (line_els != 0) {
		biggest_line = check_dens_line(densities); 
		/*see the number of line*/
		densities[biggest_line] = 0; 
		/*mark line not to back there*/
		for (i = 0; i < maxi - mini + 1; i++) {
			save_biggest_line[i] = zero; 
			/*save values of biggest_line*/
		}

		for (i = 0; i < matrix.nelements; i++){
			if (matrix.elements[i].line == biggest_line) {
				save_biggest_line[matrix.elements[i].column - minj] = matrix.elements[i].value; 
			}
		}

		offset[biggest_line - minj] = -1;
		found = 0;
		while (!found) {
			/*find offset*/
			offset[biggest_line - minj]++;
			found = 1;
			for (i = 0; i < maxi - mini + 1; i++) {
				if (save_biggest_line[i] != zero && value[i + offset[biggest_line - minj]]!=zero) {
				/*if these elements can be put in this position with this offset*/
					found =0;
				}
			}
		}

		put_values(biggest_line, save_biggest_line, offset[biggest_line - minj], value, index);
		line_els--;

		index_limit = (index_limit > i+offset[biggest_line - minj]) ? index_limit: i + offset[biggest_line - minj];
		/*for panding zero*/
	}

	printf("value =");
	for (i=0;i < index_limit;i++){
		printf(" %.3lf",value[i]);
	}
	printf("\nindex =");
	for (i=0;i < index_limit;i++){
		printf(" %lu",index[i]);
	}
	printf("\noffset =");
	for(i=0;i<maxi - mini + 1;i++){
		printf(" %d",offset[i]);
	}
	printf("\n");
}