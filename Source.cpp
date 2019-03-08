#include <iostream>
#include <iomanip>
using namespace std;

void appendOne(int * signedNum, int max_index)
{
	int i = 0;

	while (signedNum[max_index - i] == 1) {
		signedNum[max_index - i] = 0;
		i++;
	}

	signedNum[max_index - i] = 1;
}

int * decimalToBinary(int n, int int_size)
{
	bool isNegative = false;
	int * signedNum = new int[int_size];

	int max_index = int_size - 1;

	if (n < 0) {
		isNegative = true;
		n = n * -1;
	}

	// Converts to binary and appends extra zeros
	for (int i = 0; i <= max_index; i++) {
		signedNum[max_index - i] = (n % 2);
		n = n / 2;
	}

	// Flips the bits and adds a one
	if (isNegative) {
		for (int i = 0; i <= max_index; i++) {
			// Treat the integer as a bool. If its 0 (false), make it a 1.
			// If its a 1, make it a 0.
			signedNum[i] = !signedNum[i];
		}
		appendOne(signedNum, max_index);
	}

	return signedNum;
}

void add(int * multiplicand, int * product, int int_size, bool addOne)
{
	int carry = 0;
	int upper = -1;
	int lower = -1;
	int result = -1;

	for (int j = 0; j < int_size; j++) {

		result = product[int_size - 1 - j] + multiplicand[int_size - j - 1] + carry;

		if (result == 3) {
			product[int_size - 1 - j] = 1;
			carry = 1;
		}
		if (result == 2) {
			product[int_size - 1 - j] = 0;
			carry = 1;
		}
		else if (result == 1) {
			product[int_size - 1 - j] = 1;
			carry = 0;
		}
		else if (result == 0) {
			product[int_size - 1 - j] = 0;
			carry = 0;
		}

	}

	if (addOne) {
		appendOne(product, int_size - 1);
	}
}

void shift(int * product, int product_size)
{
	int first = product[0];

	for (int i = product_size; i > 0; i--) {
		product[i] = product[i - 1];
	}

	product[0] = first;
}

int * boothsAlgorithm(int a, int b, int int_size)
{
	int product_size = 2 * int_size;

	// Multiplicand
	int * m = decimalToBinary(a, int_size);

	// Scalar
	int * s = decimalToBinary(b, int_size);

	// Product
	int * product = new int[product_size + 1];

	for (int i = 0; i < int_size; i++) {
		// Set the first half to all zeros, set the second half equal to the scalar
		product[i] = 0;
		product[i + int_size] = s[i];
	}

	// Per Booths Algorithm, borrow a zero on the end
	product[product_size] = 0;

	int first = -1;
	int second = -1;

	for (int i = 0; i < int_size; i++) {
		// Bits used to determine the action taken
		first = product[product_size - 1];
		second = product[product_size];

		cout << i << "\t|\t";

		cout << setw(30);

		// If end bits are 01, prod' = prod' + multiplicand
		if (first == 0 && second == 1) {
			add(m, product, int_size, false);

			cout << "prod' = prod' + multiplicand";
		}
		// If end bits are 10, prod' = prod' - multiplicand
		else if (first == 1 && second == 0) {
			int * temp_m = new int[int_size];

			// A - B is equal to A + !B + 1

			// Invert M
			for (int j = 0; j < int_size; j++) {
				temp_m[j] = !m[j];
			}

			add(temp_m, product, int_size, true);

			cout << "prod' = prod' - multiplicand";
		}
		else {
			cout << "no operation";
		}

		shift(product, product_size);

		cout << " + shift\t\t|\t";

		for (int j = 0; j <= product_size; j++) {
			cout << product[j];
		}

		cout << endl;
	}

	int * retArr = new int[product_size];

	// We don't need the borrowed last digit anymore
	for (int i = 0; i < product_size; i++) {
		retArr[i] = product[i];
	}

	return retArr;
}

int main()
{
	const int INT_SIZE = 8;

	int a;
	int b;

	cout << "Please enter the first number to multiply: ";
	cin >> a;

	cout << endl << "Please enter the second number to multiply: ";
	cin >> b; 
	
	int * r = boothsAlgorithm(a, b, INT_SIZE);

	cout << endl << endl << "Final Result: \t" << a * b << " = ";

	for (int i = 0; i < INT_SIZE * 2; i++) {
		cout << r[i];
	}

	cout << endl;

	delete[] r;
	r = nullptr;

	return 0;
}