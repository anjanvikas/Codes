#include<iostream>
#include<string>
#include<cmath>
using namespace std;

string shift_left(string a, int m){
	while(m--){
		a = a + (char)(48);
	}
	return a;
}

string unpad_extras(string a){
	int i = 0;
	string zero("0");
	while(a[i] == '0' and (i+1)<a.length()){
		a = a.substr(i+1);
		i++;
	}
	if(a.length() == 0){
		return zero;
	}
	return a;
}

string pad(string a, int l){
	while(l--){
		a = '0' + a;
	}
	
	return a;
}


/*Ths function subtracts two given strings of numbers. This function runs only for positive integers and assumes that a > b*/
string sub(string a, string b){
	int a_length = a.length(), b_length = b.length();
	if(b == "0"){
		return a;
	}
	string c = "";	//The result is appended into this string
	
	int borrow_from, i = a_length -1, j; // borrow_from is the index from which I am borrowing a number is necessary
	
	for(j = b_length -1; j >= 0; j--){ //Since we have assumed that b is always the smaller number, the loop conditon only checks for the number of digits in b
		if(b[j] <= a[i]) c = (char) (a[i] - b[j] + 48) + c;	//This is the case where we dont have to borrow from the next digits
		
		else{
			borrow_from = i - 1;	//In this case we have to borrow
			
			while(a[borrow_from] == '0') borrow_from--;//This loop gives us the first index from where we can borrow aa digit
			
			a[borrow_from++]--;	//Decrease the value of the digit from where we borrow by one and change index towards the next digit to the right
			while(borrow_from < i) a[borrow_from++] = 57;	//All numbers which could not lend a digit were 0s so now they change to 9s
			
			c = char(a[i] - b[j] + 58) + c; // After succesfully borrowing, we can now subtract the numbers
		}
		i--;
	}
	
	if(i >= 0){			//In case the string a has more digits than b, then the remaining digits are appended in this loop
		while(i>=0){
			c = a[i] + c;
			
			i--;
		}
	}
	
	j = 0;
	while(c[j] == '0') c = c.substr(j+1);	//All valueless 0s are removed from the beginning eg. 00001 becomes 1
	
	if(c.length() == 0) c = '0';	//In case the previous loop removes all digits, it means the number was 0. This is fixed here
	
	return unpad_extras(c);
}


/*This function adds the two numbers a and b using grade school addition method*/
string add(string a, string b){	
	int a_length = a.length(), b_length = b.length();
	if(a == "0")
		return b;
	else if(b == "0")
		return a;
	
	string c = "";		//This is the new string where the result will be stored
	
	int digit1, digit2, total = 0, carry = 0, j = 0, i = a_length -1;		//digit1 and digit2 are the digits of a and b respectively
	
	for(j = b_length -1; j >= 0 && i >= 0; j--){	//The loop runs till either of the numbers a or b run out of digits to compute upon
	
		digit1 = a[i] - 48;
		//The character is converted from its ASCII value to an integer value, computed upon and then converted back to ASCII
		digit2 = b[j] - 48;
		
		total = digit1 + digit2 + carry;
		carry = total / 10;
		
		if(carry != 0) total -= 10;	
		//The total should always be a single digit, so if a carry is present, it means that the number is greater than 10
		
		c = char(total + 48) + c;
		
		i--;
	}
	if(i >= 0){				//This handles the case where a has more digits than b
		while(i>=0){
		
			digit1 = a[i] - 48;
			
			total = digit1 + carry;
			carry = total/10;
			
			if(carry!=0) total -= 10;
			
			
			c = (char)(total + 48) + c;
			
			i--;
		}
	}
	if(j >= 0){							//This handles the case where b has more digits than a
		while(j>=0){
			digit2 = b[j] - 48;	
			total = digit2 + carry;
			carry = total/10;	
			if(carry!=0) total -= 10;		
			c = (char)(total + 48) + c;
			
			j--;
		}
	}
	
	if (carry != 0) c = char(carry + 48) + c;		//This takes care of the final carry over if any
	
	j = 0;
	while(c[j] == '0') c = c.substr(j+1);			//This takes care of any useless 0s in the beginning of the string
	
	if(c.length() == 0) c = '0';					//If the number has no digits at all, it should be a zero

	//cout<<"a, b, c "<<a<<" "<<b<<" "<<c<<endl;
	return unpad_extras(c);
}


/*
*****Be warned! Dont use this for very large numbers as it will take significantly longer computational time.This function only works for non negative integers. It will give incorrect results for negative or non integral numbers*****
The grade school multiplication algorithm for 2 numbers. This is of order n*m and is just a helper function
for the Karatsuba algorithm.
*/
string naive_multiply(string a, string b){
	string c = "0", temp = "", zeroes = "";	// each time a single digit from 'a' is multiplied to b, it is stored in temp
	// 'c' is the final string that is returned
	int a_length = a.length(), b_length = b.length(), i = a_length - 1, j, digit1, digit2, total, carry = 0;
	while(i >= 0){
		carry = 0;
		j =  b_length - 1;
		digit1 = a[i] - 48;
		temp = "";
		while( j >= 0){
			digit2 = b[j] - 48;
			total = digit1 * digit2 + carry;
			carry = total/10;
			total = total % 10;
			temp = (char) (total + 48) + temp;
			j--;
		}
		if(carry != 0)
			temp = (char)(carry + 48) + temp;	//Adding the overflowed carry if any
		
		temp = temp + zeroes;//Padding the extra zeroes in the end corresponding to the stage of the multiplication
		
		zeroes = zeroes + (char)(48); //For the next stage one more zero will be required
		
		c = add(c, temp);	//Adding the new result to the total result
		
		i--;
	}
	
	return unpad_extras(c);
}


/*
*****Be warned!This function only works for non negative integers. It will give incorrect results for negative or non integral numbers*****
This functions multiplies two numbers that are stored as strings using the
Karatsuba fast multiplication algorithm.Since these functions are to be used for
extremely large numbers, Karatsuba algorithm should provide significantly faster
computations than the generic grade-school algorithm which involves n*m single digit 
mulitplications. The complexity is n^{\log_23}
 */
string multiply(string a, string b){
	//cout<<endl;
	//cout<<"a, b "<<a<<" "<<b<<endl;
	string c;
	float a_length = a.length(), b_length = b.length();

	/*If the either of the two strings represents a single digit number, then we apply the grade school algorithm.
	  This is the terminating case for recursion*/
	if(a_length < 2 or b_length < 2)
		return naive_multiply(a, b);
		
	if(a_length < b_length)
		a = pad(a, b_length - a_length);
	else if(a_length> b_length)
		b = pad(b, a_length - b_length);
	
	int m = max(a_length, b_length);
	
	if( m % 2 == 1){
		a = pad(a,1);
		b = pad(b,1);
		m++;
	}
		
	//int m = max(/ceil(a_length/2), ceil(b_length/2)) * 2;
	string low1 = a.substr(m/2), low2 = b.substr(m/2) , high1 = a.substr(0, m/2), high2 = b.substr(0, m/2);
	//cout<<high1<<" "<<low1<<" "<<high2<<" "<<low2<<endl;

	low1  = unpad_extras(low1);
	low2  = unpad_extras(low2);
	high1 = unpad_extras(high1);
	high2 = unpad_extras(high2);

	//cout<<high1<<" "<<low1<<" "<<high2<<" "<<low2<<endl;
	string z0 = multiply(low1, low2);
	string z2 = multiply(high1, high2);
	string z1 = sub( sub( multiply( add(low1, high1), add(low2,high2) ), z2), z0);
	
	//cout<<z0<<" z0 "<<z1<<" z1 "<<z2<<" z2 "<<m<<" m"<<endl;
	
	c = add( add( shift_left(z2, m), shift_left(z1, m/2) ), z0);
	return unpad_extras(c);
}



int main(){
	string a,b;
	cin>>a>>b;
	cout<<multiply(a, b)<<endl;
	cout<<naive_multiply(a, b)<<endl;
	return 0;
}