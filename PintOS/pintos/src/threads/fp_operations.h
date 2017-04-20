/* Reference: http://www.scs.stanford.edu/17wi-cs140/pintos/pintos_7.html#SEC131
			  B.6 Fixed-point real arithmetic

Let x and y be fixed-point numbers (17 integer bits, 14 decimal bits) and n an integer:

Operations needed for recent_cpu and load_avg calculations in MLFQS. (Real-numbers simulation)

Convert n to fixed point:	n * f
Convert x to integer (rounding toward zero):	x / f
Convert x to integer (rounding to nearest):	(x + f / 2) / f if x >= 0, 
(x - f / 2) / f if x <= 0.
Add x and y:	x + y
Subtract y from x:	x - y
Add x and n:	x + n * f
Subtract n from x:	x - n * f
Multiply x by y:	((int64_t) x) * y / f
Multiply x by n:	x * n
Divide x by y:	((int64_t) x) * f / y
Divide x by n:	x / n */

#define F (1 << 14)
#define MAX_INT ((1 << 31)-1)
#define MAX_INT ((1 << 31)-1)

int int_to_fp(int n);
int fp_to_int_round_zero(int x);
int fp_to_int_round_nearest(int x);
int add_fp(int x, int y);
int sub_fp(int x, int y);
int add_fp_int(int x, int n);
int sub_fp_int(int x, int n);
int mult_fp(int x, int y);
int mult_fp_int(int x, int n);
int div_fp(int x, int y);
int div_fp_int(int x, int n);

int int_to_fp(int n){
	return n * F;
}

int fp_to_int_round_zero(int x){
	return x / F;
}

int fp_to_int_round_nearest(int x){
	if(x >= 0){
		return (x + F / 2) / F;
	}else{
		return (x - F / 2) / F;
	}
}

int add_fp(int x, int y){
	return x + y;
}

int sub_fp(int x, int y){
	return x - y;
}

int add_fp_int(int x, int n){
	return x + int_to_fp(n);
}

int sub_fp_int(int x, int n){
	return x - int_to_fp(n);	
}

int mult_fp(int x, int y){
	return ((int64_t) x) * y / F;
}

int mult_fp_int(int x, int n){
	return x * n;
}

int div_fp(int x, int y){
	return ((int64_t) x) * F / y;
}

int div_fp_int(int x, int n){
	return x / n;	
}
