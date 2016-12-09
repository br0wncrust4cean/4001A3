# 4001A3




CODE FOR USE FOR TOMORROW 
char line[256]; //string used to keep track of each line
		FILE* file = fopen(fileName, "r");
		char* collection = malloc(20*sizeof(char)); //array each value in the line
		char* beginning = collection; //pointer to beginning of the array
		while (fgets(line, sizeof(line), file)){ 
			char* p = line; //pointer to the first line in the file
			while(*p){
				if (isdigit(*p)){ //stores value in the array if the value in the line is a digit
					//long val = strtol(p, &p, 10);
					*collection = *p;
					collection++; 
				} else if(*p == ','){ //goes to next value if character is a comma
					p++;
				}
			}
		}
