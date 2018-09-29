#include <bits/stdc++.h>
#define checkBit(n,p) (bool)(n&(1<<p))
#include <bitset>
#include <iostream>
#include <sstream>
#include<conio.h>
#include<cstring>
#include <string>
using namespace std;

string GetHexFromBin(string sBinary);
map<string,string> getMap(string path);
string decToBin(int num,int length);
string bitExtender(string sBinary);

/*
 * Function: getMap(String path )
 * ----------------------------
 *   Returns the map of the input file divided in key and corresponding value
 *
 *   path :  input file for handling into a map
 *
 *
 *   returns: the tokenized key with corresponding value , as in, the instructions with their corresponding op codes and
 *   register values with their corresponding binary names.
 */
map<string,string> getMap(string path)
{
    string word,value;
    map<string,string> m;
    ifstream file(path.c_str());
    while(file>>word>>value)
    {
        m[word] = value;
    }
    file.close();
    return m;
}

/*
 * Function: decToBin(int num,int length)
 * ----------------------------
 *   Returns the binary value of the input decimal number within the given 8 bit format
 *   num :  the immediate value used in I type format
 *   length : 3 bit (since we have assigned 3 bits in the immediate field
 *
 *   returns: it uses the #define preprocessor function like macro checkBit(n,p) (bool)(n&(1<<p))
 *   to recursively update the decimal value into binary code.
 */

string decToBin(int num,int length)
{
    string result = "";
    for(int i=0;i<length;i++)
    {

        if(checkBit(num,i))
        {
            result = "1" + result;

        }
        else
        {
            result = "0" + result;

        }
    }
    return result;
}


/*
 * Function:bitExtender(string sBinary)
 * ----------------------------
 *   Extends the already produced binary code in 4 bit format for easy conversion to hexadecimal code
 *   sBinary  :  the binary code from the decimal input
 *   returns: 4 bit binary output from 3 bit binary input.
 *
 */
string bitExtender(string sBinary)
{
    for(int i = 0; sBinary.length()!= 4; i++)
    {
        sBinary = "0" + sBinary;
    }
    return sBinary;

}





int main()
{
    int val;
    string ins,op,r1,r2,cmnt,temp;
    int func;
    map<string,string> r_type, I_type, register_value;   //Obtaining static values from storage
    r_type = getMap("File/r_type.txt");
    I_type = getMap("File/I_type.txt");
    register_value = getMap("File/register_value.txt");
    ifstream inputFile("File/input.txt");   //ifstream inputFile("File/.txt");
    ofstream outputFile("File/output.txt"); //ofstream outputFile("File/.txt");



    while(getline(inputFile,ins))
    {
        stringstream ss;
        cout << ins << endl;
        //Checking if instruction is empty or not
        if(ins.size()==0)
        {
            cout << "Invalid: Instruction is empty\n" << endl;
            continue;
        }
        ss << ins;
        ss >> op;
        ss >> r1;


        //check step for different op codes
        if(op!="go")
        {
                 //assigns rd as zero for "j" instruction
                if(op == "j")
                {
                    temp = r1;
                    r1 = "$ACC,";
                    r1.erase(r1.end()-1);
                    goto level;
                }

        //error handling of input format of rd (destination register)

        if(r1[0]!='$')
        {
                cout << "Invalid: '$' sign is missing before rd\n" << endl;
                outputFile << "Invalid: '$' sign is missing before rd\n" << endl;
                continue;
            }


        else if(r1[r1.size()-1]!=',')
        {
                cout << "Invalid: ',' is missing after rd\n" << endl;
                outputFile << "Invalid: ',' is missing after rd\n" << endl;
                continue;
            }


        //erase the "," at the end of register name
        r1.erase(r1.end()-1);


        //handle the bne and sll instruction separately
        if( op == "bne" || op == "sll")
        {
                temp= r1;
                goto level;

            }

        // checks the functionality value of add and sub instruction
        if(op == "add")
        {
                func = 0;
            }
        else func = 1;



        }

        //If operation code is R Type then the program will go in here
        if(r_type.find(op)!=r_type.end())
        {
            val = 0;

            ss >> r2;

            //checks the validity of source register
            if(r2[0]!='$')
                {
                    cout << "Invalid: '$' sign is missing before rs\n" << endl;
                    outputFile << "Invalid: '$' sign is missing before rs\n" << endl;
                    continue;
                }

            print:

            //checking the validity of given register inputs
            if(register_value.find(r1)==register_value.end())
            {
                cout << "Invalid: rd not found\n" << endl;
                outputFile << "Invalid: rd not found\n" << endl;
            }
            else if(register_value.find(r2)==register_value.end())
            {
                cout << "Invalid: rs not found\n" << endl;
                outputFile << "Invalid: rs not found\n" << endl;
            }


            //Checking instruction validities
            else if(val<0)
            {
                cout << "Invalid: value can not be neg\n" << endl;
                outputFile << "Invalid: value can not be neg\n" << endl;
            }
            else if(val>7)
            {
                cout << "Invalid: value is too large\n" << endl;
                outputFile << "Invalid: value is too large\n" << endl;
            }
            else if(ss>>cmnt&&(cmnt[0]!='/'||cmnt[1]!='/'))
            {
            	cout << "Invalid: comment structure not maintained\n" << endl;
            	outputFile << "Invalid: comment structure not maintained\n" << endl;
            }

            //Printing machine code if instruction is valid
            else
            {
               cout << r_type[op] << " " << register_value[r1] << " " << register_value[r2] << " " << func <<  endl;
               cout << GetHexFromBin(r_type[op]) << "   " << GetHexFromBin(register_value[r1]) << "  " << GetHexFromBin(register_value[r2]) << "  " << func << "\n" << endl;
               outputFile << r_type[op] << " " << register_value[r1] << " " << register_value[r2] << " "  << func << endl;
               outputFile << GetHexFromBin(r_type[op]) << " " << GetHexFromBin(register_value[r1]) << " " << GetHexFromBin(register_value[r2]) << " " << func  << "\n" << endl;
            }
        }

         //If operation code is I Type then the program will go in here

     else if(I_type.find(op)!=I_type.end())
        {
            ss >> val;

            //separate handling of "j", "bne" & "sll"
            level : if (op == "j" || op == "bne"|| op == "sll"){

                    ss>> temp;

                    int jumpVal = stoi(temp);
                    if(jumpVal>3 ){          //2^3 = 8 //3 is imm bit

                        cout << "Invalid: value is too large \n" << endl;
                        outputFile << "Invalid: value is too large\n" << endl;

                    }
                    else if(jumpVal<-4){       //2^3 = 8 //3 is imm bit
                        cout << "Invalid: value is too small\n" << endl;
                        outputFile << "Invalid: value is too small\n" << endl;
                    }

                    else if (register_value.find(r1)==register_value.end())
                    {
                    cout << "Invalid: rd not found\n" << endl;
                    outputFile << "Invalid: rd not found\n" << endl;
                    }

                    else {
                        cout << I_type[op] << " " << register_value[r1] << " " << decToBin(jumpVal,3) << endl;
                        cout << GetHexFromBin(I_type[op]) << "   " << GetHexFromBin(register_value[r1]) << "  " << GetHexFromBin(decToBin(jumpVal,3)) << "\n" << endl;
                    }

                    }

                    //handling of "lw","sw","slt" ,"disp"

                    else if(val>7) //2^3 = 8 //3 is imm bit
                    {
                    cout << "Invalid: value is too large\n" << endl;
                    outputFile << "Invalid: value is too large\n" << endl;
                    }
                else if(val<0) //value can't be negative for memory access
                    {
                    cout << "Invalid: value is too small\n" << endl;
                    outputFile << "Invalid: value is too small\n" << endl;
                    }
                else if(ss>>cmnt&&(cmnt[0]!='/'||cmnt[1]!='/'))
                    {
                    cout << "Invalid: comment structure not maintained\n" << endl;
                    outputFile << "Invalid: comment structure not maintained\n" << endl;
                    }
                else if(register_value.find(r1)==register_value.end())
                    {
                    cout << "Invalid: rd not found\n" << endl;
                    outputFile << "Invalid: rd not found\n" << endl;
                    }

            //Printing machine code if instruction is valid
                else
                    {

                    cout << I_type[op] << " " << register_value[r1] << " " << decToBin(val,3) << endl;
                    cout << GetHexFromBin(I_type[op]) << "   " << GetHexFromBin(register_value[r1]) << "  " << GetHexFromBin(decToBin(val,3)) << "\n" << endl;
                    outputFile << I_type[op] << " " << register_value[r1] << " " << decToBin(val,3) << endl;
                    outputFile << GetHexFromBin(I_type[op]) << " " << GetHexFromBin(register_value[r1]) << " " << GetHexFromBin(decToBin(val,3)) << "\n"  << endl;

                }

        }
    else
            {
        	cout << "Invalid: Op code not found\n" << endl;
        	outputFile << "Invalid: Op code not found\n" << endl;
            }
    }



    inputFile.close();
}



/*
 * Function: GetHexFromBin(string sBinary)
 * ----------------------------
 *   Returns the hexadecimal value of the input binary number
 *   sBinary  :  the binary code from the decimal input
 *
 *   returns: hexadecimal output of the results in binary format
 */
string GetHexFromBin(string sBinary)
{
	string rest,tmp,chr = "";

	sBinary = bitExtender(sBinary);


	int len = sBinary.length()/4;
	chr = chr.substr(0,len);
	sBinary = chr+sBinary;
	for (int i=0;i<sBinary.length();i+=4)
	{
		tmp = sBinary.substr(i,4);
		if (!tmp.compare("0000"))
		{
			rest = rest + "0";
		}
		else if (!tmp.compare("0001"))
		{
			rest = rest + "1";
		}
		else if (!tmp.compare("0010"))
		{
			rest = rest + "2";
		}
		else if (!tmp.compare("0011"))
		{
			rest = rest + "3";
		}
		else if (!tmp.compare("0100"))
		{
			rest = rest + "4";
		}
		else if (!tmp.compare("0101"))
		{
			rest = rest + "5";
		}
		else if (!tmp.compare("0110"))
		{
			rest = rest + "6";
		}
		else if (!tmp.compare("0111"))
		{
			rest = rest + "7";
		}
		else if (!tmp.compare("1000"))
		{
			rest = rest + "8";
		}
		else if (!tmp.compare("1001"))
		{
			rest = rest + "9";
		}
		else if (!tmp.compare("1010"))
		{
			rest = rest + "A";
		}
		else if (!tmp.compare("1011"))
		{
			rest = rest + "B";
		}
		else if (!tmp.compare("1100"))
		{
			rest = rest + "C";
		}
		else if (!tmp.compare("1101"))
		{
			rest = rest + "D";
		}
		else if (!tmp.compare("1110"))
		{
			rest = rest + "E";
		}
		else if (!tmp.compare("1111"))
		{
			rest = rest + "F";
		}
		else
		{
			continue;
		}
	}
	return rest;
}

