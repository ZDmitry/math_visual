////////////////////////////////////////////////////////
//Additional functions for string                     //
////////////////////////////////////////////////////////
#include "str_pp.h"

bool StringAnalizer::isStrNumeric(const char* str, int n) //numeric string?
{
	bool b=1, fp=0, fm=0;
	
	if (str=="") return 0;
	for(int i=0; i<(n-1); i++)
	{
		if(str[i]=='\0') return b;    //if end of string - return answer
		if(((int)str[i])==(-52)) b=0; //string consist trash, not numeric
		if(isdigit(str[i])==0) b=0;   //if any char is not numeric, then whole string is not numeric too
		if(str[i]=='x') return 0;     //reached õ - not numeric
		if(str[i]=='.' && fp==0)      //dot - may be double
		{
			b=1;
			fp=1;
		}
		if(str[0]=='-' && fm==0)      //minus - may be still numeric
		{
			b=1;
			fm=1;
		}
	}

	return b; 
}

bool StringAnalizer::consistSign(const char* str, int n, char c)
{
	bool b=false;
	int non_sens=0;

	for(int i=0; i<n; i++)
	{	
		//do not check when reached '(' or ')' 
		if(str[i]=='(') non_sens++; 
		if(str[i]==')') non_sens--;

		//consist some sign
		if(non_sens==0 && str[i]==c) b=true; 
	}

	return b;
}

bool StringAnalizer::isMltpl(const char* str, int n) // Multiplication?
{
	return consistSign(str,n,'*');
}

bool StringAnalizer::isPlus(const char* str, int n) // Addidion?
{
	return consistSign(str,n,'+');
}

bool StringAnalizer::isMinus(const char* str, int n) // Substraction?
{
	return consistSign(str,n,'-');
}

bool StringAnalizer::isDiv(const char* str, int n)  // Division?
{
	return consistSign(str,n,'/');
}

bool StringAnalizer::isPow(const char* str, int n) // Powering?
{
	return consistSign(str,n,'^');
}

bool StringAnalizer::isSin(const char* str, int n) // Sin?
{
    if (n==3)
	{
		// reached "sin" - trigonometric func
		if (str[0]=='s' && str[1]=='i' && str[2]=='n') return true;
	}

	return false;
}

bool StringAnalizer::isCos(const char* str, int n) // Cos?
{
    if (n==3)
	{
		// reached "cos" - trigonometric func
		if (str[0]=='c' && str[1]=='o' && str[2]=='s') return true;
	}

	return false;
}

bool StringAnalizer::isTan(const char* str, int n) // Tan?
{
    if (n==3)
	{
		// reached "tan" - trigonometric func
		if (str[0]=='t' && str[1]=='a' && str[2]=='n') return true;
	}

	return false;
}
 
char* ftoa(double d, int count) // double to string (format 0.00)
{
	int dec, sign, adec;
	char* buff, *res=new char[60];
	char  lbf[30],rbf[30];

	// double to string (stage 1)
	buff=_fcvt(d,count,&dec,&sign); 
	adec=(count-dec);
 
	// find dot position
    if (dec<(-20)) return "0\0"; 
    if (dec>=0) 
    {
	    if (dec==0) strcpy(lbf,"0\0");
	    else
	    {
		    for (int i=0; i<dec; i++)
		        lbf[i]=buff[i];
  	            lbf[dec]='\0';
	    }

	    for (int i=0; i<adec; i++)
		    rbf[i]=buff[dec+i];
	    
		rbf[adec]='\0';
	
		// has minus sign - put first
		if (!sign) strcpy(res,lbf);
		else 
		{
			strcpy(res,"-");
			strcat(res,lbf);
		}
		
		strcat(res,".");
		strcat(res,rbf);
	}
	else
	{
		for(int i=0; i<((-1)*dec); i++)
			lbf[i]='0';
  		
		lbf[(-1)*dec]='\0';

		for(int i=0; i<((-1)*dec); i++)
			rbf[i]=buff[i];
  		
		rbf[(-1)*dec]='\0';

		// has minus sign - put first
		if (!sign) strcpy(res,"0");
		else 
		{
			strcpy(res,"-");
			strcat(res,"0");
		}
		
		strcat(res,".");
		strcat(res,lbf);
		strcat(res,rbf);
	}	

	// return string
	return res;
}