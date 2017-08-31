#include <getopt.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <stack>
#include <vector>

using namespace std;

//ebx - base address register
//esi - address offset register
//a* - value register
//di - memory width

struct programConfig_t {
	int memoryLength;
	int memoryWidth;
	bool useProcedures;
	bool zeroOnReturn;
	string functionName;
	string registerName;
	ifstream inFile;
	ofstream outFile;
	bool verbose;
} programConfig;
static const char *options = "m:w:o:n:zpvh?";
string outFilename;

void writePreamble(ofstream& outFile);
void writePostamble(ofstream& outFile);
void printHelp();
string deriveFilename(string source);

int main(int argc, char* argv[]){
	string inFile;
	
	programConfig.memoryLength = 30000;
	programConfig.memoryWidth = 1;
	programConfig.useProcedures = false;
	programConfig.zeroOnReturn = false;
	programConfig.functionName = "main";
	programConfig.registerName = "al";
	programConfig.verbose = false;
	
	if(argc==1) {
		char confirm;
		struct stat statBuffer;
		cout<<"Source file: ";
		getline(cin, inFile);
		if((stat(inFile.c_str(),&statBuffer))!=0){
			cerr<<"Error: File doesn't exist";
			return 0;
		}
		
		cout<<"Output file: ";
		getline(cin, outFilename);
		if((stat(outFilename.c_str(),&statBuffer))==0){
			cerr<<"Warning: File exists. Continue? (y/n): ";
			cin>>confirm;
			if(confirm!='y'){
				return 0;
			}
		}	
	}
	else {
		int opt = getopt(argc, argv, options);
		int temp;
		while(opt != -1) {
			switch(opt)
			{
				case 'z':
					programConfig.zeroOnReturn = true;
					break;
				case 'p':
					programConfig.useProcedures = true;
					break;
				case 'm':
					temp = strtol(optarg, NULL, 0);
					if(temp<1) {
						cerr<<"Error: Memory length can't be less than 1";
						return 0;
					}
					if(temp<10) {
						cerr<<"Warning: Memory length of "<<temp<<" is probably too low";
					}
					programConfig.memoryLength = temp;
					break;
				case 'w':
					temp = strtol(optarg, NULL, 0);
					switch(temp)
					{
						case 1:
							programConfig.registerName = "al";
							break;
						case 2:
							programConfig.registerName = "ax";
							break;
						case 4:
							programConfig.registerName = "eax";
							break;
						case 8:
							programConfig.registerName = "rax";
							break;
						default:
							cerr<<"Error: Memory width must be one of the following: 1, 2, 4, 8";
							return 0;
					}
					programConfig.memoryWidth = temp;
					break;
				case 'n':
					programConfig.functionName = optarg;
					break;
				case 'o':
					outFilename = optarg;
					break;
				case 'v':
					programConfig.verbose = true;
					break;
				case '?':
				case 'h':
					printHelp();
					return 0;
					break;
				default:
					;
			}
			opt = getopt(argc, argv, options);
		}
	}
	
	if(argc==optind) {
		cerr<<"Error: No input file specified";
		return 0;
	}
	
	inFile = argv[optind];
	if(outFilename.empty()) {
		outFilename = deriveFilename(inFile);
	}
	
	if(programConfig.verbose) {
		cout<<"Input file: "<<inFile<<"\nOutput file: "<<outFilename<<endl;
		cout<<"Function name: \""<<programConfig.functionName<<'"'<<endl;
		cout<<"Memory: "<<programConfig.memoryLength<<"(memory length) x "<<programConfig.memoryWidth<<"(memory width) = "
			<<programConfig.memoryLength*programConfig.memoryWidth<<" bytes"<<endl;
		cout<<"Program returns "<< (programConfig.zeroOnReturn?"zero":"current value") <<endl;
		cout<<"Procedures syntax "<< (programConfig.useProcedures?"en":"dis") <<"abled"<<endl;
	}
	
	programConfig.inFile.open(inFile.c_str());
	if(!programConfig.inFile.good()) {
		cerr<<"Error: Can't open file \""<<inFile<<"\" for reading\n";
		return 0;
	}
	programConfig.outFile.open(outFilename.c_str(), ios_base::out|ios_base::trunc);
	if(!programConfig.outFile.good()){
		cerr<<"Error: Can't open file \""<<inFile<<"\" for writing\n";
		programConfig.inFile.close();
		return 0;
	}
	
	writePreamble(programConfig.outFile);
	
	char op;
	int line=1, column=0;
	int loopIndex = 1;
	stack<int,vector<int> > loopStack;
	int memoryIndex = 0, memoryIndexMax = programConfig.memoryLength-1;
	string incrementString, decrementString;
	if(programConfig.memoryWidth==1) {
		incrementString = "inc esi\n";
		decrementString = "dec esi\n";
	}
	else {
		incrementString = "add ";
		switch(programConfig.memoryWidth)
		{
			case 2:
			case 4:
				incrementString+= "esi, edi";
				break;
			case 8:
				incrementString+= "rsi, rdi";
				break;
		}			
		incrementString+= '\n';
		decrementString = incrementString;
		decrementString.replace(0, 3, "sub");
	}
	
	programConfig.inFile.get(op);
	while(!programConfig.inFile.eof()){
		column++;
		switch(op) 
		{
			case '>':
				programConfig.outFile<<"mov [ebx+esi], "<<programConfig.registerName<<'\n';
				if(++memoryIndex > memoryIndexMax){
					memoryIndex = 0;
					if(programConfig.memoryWidth<=4) {
						programConfig.outFile<<"xor esi, esi\n";
					}
					else {
						programConfig.outFile<<"xor rsi, rsi\n";
					}
				}
				else {
					programConfig.outFile<<incrementString;
				}
				programConfig.outFile<<"mov "<<programConfig.registerName<<", [ebx+esi]\n";
				break;
				
			case '<':
				programConfig.outFile<<"mov [ebx+esi], "<<programConfig.registerName<<'\n';
				if(--memoryIndex < 0){
					memoryIndex = memoryIndexMax;
					if(programConfig.memoryWidth<=4) {
						programConfig.outFile<<"mov esi, "<<memoryIndexMax<<'\n';
					}
					else {
						programConfig.outFile<<"mov rsi, "<<memoryIndexMax<<'\n';
					}
				}
				else {
					programConfig.outFile<<decrementString;
				}
				programConfig.outFile<<"mov "<<programConfig.registerName<<", [ebx+esi]\n";
				break;
				
			case '+':
				programConfig.outFile<<"inc "<<programConfig.registerName<<'\n';
				break;
			case '-':
				programConfig.outFile<<"dec "<<programConfig.registerName<<'\n';
				break;
				
			case '.':
				programConfig.outFile<<"mov [esp], "<<programConfig.registerName<<'\n';
				programConfig.outFile<<"call _putchar\n";
				programConfig.outFile<<"mov "<<programConfig.registerName<<", [esp]\n"; //TODO: proper hanadling of 64
				break;
			case ',':
				programConfig.outFile<<"call _getchar\n";
				break;
				
			case '[':
				loopStack.push(loopIndex);
				programConfig.outFile<<"jmp LoopCondition"<<loopIndex<<'\n'<<"LoopBody"<<loopIndex<<":\n";
				loopIndex++;
				break;
			case ']':
				if(loopStack.empty()) {
					cerr<<"Error: Extra ']' at line "<<line<<", column "<<column<<endl;
					return 0;
				}
				programConfig.outFile<<"LoopCondition"<<loopStack.top()<<":\n"<<"test "<<programConfig.registerName<<','<<programConfig.registerName<<"\njnz LoopBody"<<loopStack.top()<<'\n';
				loopStack.pop();
				break;
				
			case '\n':
				line++;
				column = 0;
				programConfig.outFile<<"\n";
				break;
			case ' ':
				break;
			default:
				cout<<"Error: Invalid character at line "<<line<<", column "<<column<<endl;
				return 0;
		}
		programConfig.inFile.get(op);
	}
	if(!loopStack.empty()) {
		cout<<"Error: Extra '[', "<<loopStack.size()<<" loop(s) not closed";
		return 0;
	}
	
	writePostamble(programConfig.outFile);

	return 0;
}

void writePreamble(ofstream& out)
{
	out<<"global "<<'_'<<programConfig.functionName<<'\n';
	out<<"extern "<<'_'<<"putchar\n";
	out<<"extern "<<'_'<<"getchar\n";
	out<<endl;
	out<<"section .bss\n";
	out<<"memory:\n";
	out<<"res";
	switch(programConfig.memoryWidth)
	{
		case 1:
			out<<'b';
			break;
		case 2:
			out<<'w';
			break;
		case 4:
			out<<'d';
			break;
		case 8:
			out<<'q';
			break;
	}
	out<<' '<<programConfig.memoryLength<<'\n';
	out<<endl;
	out<<"section .text\n";
	out<<'_'<<programConfig.functionName<<":\n";
	if(programConfig.memoryWidth<=4) {
		out<<"push ebp\n";
		out<<"mov ebp, esp\n";
		out<<"push ebx\n";
		out<<"push esi\n";
		out<<"push edi\n";
		out<<"sub esp, 4\n";
		out<<"mov ebx, memory\n";
		out<<"xor esi, esi\n";
		out<<"xor eax, eax\n";
		if(programConfig.memoryWidth>1)
			out<<"mov edi, "<<programConfig.memoryWidth<<'\n';
	}
	else {
		out<<"push rbp\n";
		out<<"mov rbp, rsp\n";
		out<<"push rbx\n";
		out<<"push rsi\n";
		out<<"push rdi\n";
		out<<"sub rsp, 8\n";
		out<<"mov rbx, memory\n";
		out<<"xor rsi, rsi\n";
		out<<"xor rax, rax\n";
		out<<"mov rdi, "<<programConfig.memoryWidth<<'\n';
	}
	out<<endl;
}

void writePostamble(ofstream& out)
{
	out<<endl;
	if(programConfig.zeroOnReturn) {
		out<<"xor "<<programConfig.registerName<<", "<<programConfig.registerName<<'\n';
	}
	if(programConfig.memoryWidth<=4) {
		out<<"add esp,4\n";
		out<<"pop edi\n";
		out<<"pop esi\n";
		out<<"pop ebx\n";
		out<<"pop ebp\n";
	}
	else {
		out<<"add rsp,8\n";
		out<<"pop rdi\n";
		out<<"pop rsi\n";
		out<<"pop rbx\n";
		out<<"pop rbp\n";
	}
	out<<"ret\n";
}

void printHelp()
{
	cout<<"Usage: bfcompile source [options]"<<endl;
	cout<<"Options:"<<endl;
	cout<<"  -m <length>\t\tSet memory length in cells. Default: 30000\n";
	cout<<"  -w <width>\t\tSet memory cell width in bytes. Allowed values: 1, 2, 4, 8. Default: 1\n";
	cout<<"\t\t\tValue of 8 generates 64-bit code. Untested.\n";
	cout<<"  -n <name>\t\tSet main function name. Default: \"main\"\n";
	cout<<"  -o <filename>\t\tSet output file. If not specified, filename will be derived from source\n";
	cout<<"  -z\t\t\tAlways return zero. Otherwise, return current value\n";
	//cout<<"  -p\t\t\tAllow brainfuck procedures syntax\n";
	cout<<"  -v\t\t\tEnable verbose mode\n";
}

string deriveFilename(string source)
{
	string outFile = source;
	size_t pos = outFile.find_last_of('.');
	if(pos!=string::npos) {
		outFile.erase(pos);
	}
	outFile.append(".asm");
	return outFile;
}
