#include <iostream>
#include <string>
#include <cmath>
#include <limits>
using namespace std;

#define MAX_OPERATORS 16

void throwException(const char* message)
{
	cerr << message << endl;
	terminate();
}

int strlen(const char* str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		i++;
	}
	return i;
}

class Operation
{
protected:
	string name;
	string symbol;

	void assertValidity()
	{
		if(name.empty()) throwException("Invalid operation name!");
		if(symbol.empty()) throwException("Invalid operation symbol!");
	}

	void copyFrom(string name, string symbol)
	{
		this->name = name;
		this->symbol = symbol;
		assertValidity();
	}

public:
	Operation(string name, string symbol)
	{
		copyFrom(name, symbol);
	}

	Operation(const Operation &other)
	{
		copyFrom(other.name, other.symbol);
	}

	Operation& operator=(const Operation &other)
	{
		if(this != &other)
		{
			copyFrom(other.name, other.symbol);
		}
		return *this;
	}

	virtual Operation* createNew() const = 0;

	virtual double execute(const double n1, const double n2) = 0;

	string getName() const
	{
		return name;
	}

	string getSymbol() const
	{
		return symbol;
	}

	void setName(const string name)
	{
		this->name = name;
		assertValidity();
	}

	void setSymbol(const string symbol)
	{
		this->symbol = symbol;
		assertValidity();
	}
};

class AddOperation: public Operation
{
public:
	AddOperation(): Operation("Add", "+") {};

	AddOperation(const AddOperation &other): Operation(other) {};

	AddOperation& operator=(const AddOperation &other)
	{
		Operation::operator=(other);
		return *this;
	}

	AddOperation* createNew() const override
	{
		return new AddOperation(*this);
	}

	double execute(const double n1, const double n2) override
	{
		return n1 + n2;
	}
};

class SubtractOperation: public Operation
{
public:
	SubtractOperation(): Operation("Subtract", "-") {};

	SubtractOperation(const SubtractOperation &other): Operation(other) {};

	SubtractOperation& operator=(const SubtractOperation &other)
	{
		Operation::operator=(other);
		return *this;
	}

	SubtractOperation* createNew() const override
	{
		return new SubtractOperation(*this);
	}
	
	double execute(const double n1, const double n2) override
	{
		return n1 - n2;
	}
};

class MultiplyOperation: public Operation
{
public:
	MultiplyOperation(): Operation("Multiply", "*") {};

	MultiplyOperation(const MultiplyOperation &other): Operation(other) {};

	MultiplyOperation& operator=(const MultiplyOperation &other)
	{
		Operation::operator=(other);
		return *this;
	}

	MultiplyOperation* createNew() const override
	{
		return new MultiplyOperation(*this);
	}

	double execute(const double n1, const double n2) override
	{
		return n1 * n2;
	}
};

class DivideOperation: public Operation
{
public:
	DivideOperation(): Operation("Divide", "/") {};

	DivideOperation(const DivideOperation &other): Operation(other) {};

	DivideOperation& operator=(const DivideOperation &other)
	{
		Operation::operator=(other);
		return *this;
	}

	DivideOperation* createNew() const override
	{
		return new DivideOperation(*this);
	}

	double execute(const double n1, const double n2) override
	{
		if(n2 == 0) throwException("Cannot divide by zero!");
		return n1 / n2;
	}
};

class PowerOperation: public Operation
{
public:
	PowerOperation(): Operation("Power", "**") {};

	PowerOperation(const PowerOperation &other): Operation(other) {};

	PowerOperation& operator=(const PowerOperation &other)
	{
		Operation::operator=(other);
		return *this;
	}

	PowerOperation* createNew() const override
	{
		return new PowerOperation(*this);
	}

	double execute(const double n1, const double n2) override
	{
		if(n1 == 0 && n2 == 0) throwException("Cannot raise 0 to the power of 0!");
		return pow(n1, n2);
	}
};

class RootOperation: public Operation
{
public:
	RootOperation(): Operation("Root", "V") {};

	RootOperation(const RootOperation &other): Operation(other) {};

	RootOperation& operator=(const RootOperation &other)
	{
		Operation::operator=(other);
		return *this;
	}

	RootOperation* createNew() const override
	{
		return new RootOperation(*this);
	}

	double execute(const double n1, const double n2) override
	{
		if(n1 < 0 && n2 < 0) throwException("Cannot take negative root of negative number!");
		if(n1 < 0 && (int)n2 != n2) throwException("Cannot take fractional root of negative number");
		return pow(n1, 1 / n2);
	}
};

class Calculator
{
protected:
	const char* name;
	size_t numberOfSupportedOperations;
	size_t capacityForOperations;
	Operation **operations;
	static double numberOfSuccessfulCalculations;

	void assertValidity()
	{
		if(name == nullptr || strlen(name) == 0) throwException("Invalid calculator name!");
		if(capacityForOperations == 0) throwException("Capacity for operations cannot be zero!");
	}

	void copyFrom(const char* name, size_t numberOfSupportedOperations, size_t capacityForOperations, Operation **operations)
	{
		this->name = name;
		this->numberOfSupportedOperations = numberOfSupportedOperations;
		this->capacityForOperations = capacityForOperations;
		this->operations = new Operation*[capacityForOperations];
		for(size_t i = 0; i < numberOfSupportedOperations; i++)
		{
			this->operations[i] = operations[i];
		}
		assertValidity();
	}

	double calculate(double n1, double n2, string op) const
	{
		for(int i = 0; i < numberOfSupportedOperations; i++)
		{
			if(operations[i]->getSymbol() == op) return operations[i]->execute(n1, n2);
		}
		return 0;
	}

public:
	Calculator()
	{
		char* buffer = new char[1];
		buffer[0] = '\0';
		name = buffer;
		operations = new Operation*[2];
		numberOfSupportedOperations = 0;
		capacityForOperations = 2;
		assertValidity();
	}

	Calculator(const char* name, size_t n, Operation** ops)
	{
		copyFrom(name, n, MAX_OPERATORS, ops);
	}

	Calculator(const Calculator& other)
	{
		copyFrom(other.name, other.numberOfSupportedOperations, other.capacityForOperations, other.operations);
	}

	const Calculator& operator=(const Calculator& other)
	{
		if(this != &other)
		{
			delete[] name;
			delete[] operations;
			copyFrom(other.name, other.numberOfSupportedOperations, other.capacityForOperations, other.operations);
		}
		return *this;
	}

	~Calculator()
	{
		delete[] name;
		delete[] operations;
	}

	void listSupportedOperations() const
	{
		for(size_t i = 0; i < numberOfSupportedOperations; i++)
		{
			cout << operations[i]->getSymbol() << " - " << operations[i]->getName() << endl;
		}
	}

	void listInputFormat() const
	{
		cout << "<num1> <symbol> <num2> <symbol> <num3> ... <numN> =" << endl;
		cout << "Please make sure to include spaces between each number and operator." << endl;
	}

	Calculator& addOperation(const Operation* op)
	{
		if(numberOfSupportedOperations == capacityForOperations) throwException("Capacity for operations exceeded!");
		operations[numberOfSupportedOperations++] = op->createNew();
		return *new Calculator();
	}

	void startCalculation() const
	{
		double result;
		double num2;
		string op;
		cin >> result;
		while(true)
		{
			cin >> op;
			if(op == "=") break;
			cin >> num2;
			result = calculate(result, num2, op);
		}
		numberOfSuccessfulCalculations++;
		cout << result << endl;
	}

	double getNumberOfSuccessfulCalculations() const
	{
		return numberOfSuccessfulCalculations;
	}
};

double Calculator::numberOfSuccessfulCalculations = 0;

Operation* createOperation(string operationSymbol)
{
	if(operationSymbol == "+") return new AddOperation();
	if(operationSymbol == "-") return new SubtractOperation();
	if(operationSymbol == "*") return new MultiplyOperation();
	if(operationSymbol == "/") return new DivideOperation();
	if(operationSymbol == "**") return new PowerOperation();
	if(operationSymbol == "V") return new RootOperation();
	throwException("Invalid operator!");
	return new AddOperation(); // So that the compiler doesn't complain...
}

int main()
{
	cout << "Enter calculator's name: ";
	char* calculatorName = new char[256];
	cin.getline(calculatorName, 256);
	size_t numberOfOperations;
	do
	{
		cout << "Enter number of operations: ";
		cin >> numberOfOperations;
		if(cin.fail())
		{
			cout << "Couldn't convert to number!" << endl;
			
		}
		else if(numberOfOperations > MAX_OPERATORS)
		{
			cout << "Exceeded operator capacity of " << MAX_OPERATORS << "!" << endl;
		}
		else break;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	} while(true);
	
	cout << "Enter operations: " << endl;
	cout << "+ - add" << endl;
	cout << "- - subtract" << endl;
	cout << "* - multiply" << endl;
	cout << "/ - divide" << endl;
	cout << "** - power" << endl;
	cout << "V - root" << endl;
	string* operationList = new string[MAX_OPERATORS];
	while(true)
	{
		string operationSymbol;
		bool valid = true;
		for(int i = 0; i < numberOfOperations; i++)
		{
			cin >> operationSymbol;
			if(operationSymbol == "+" || operationSymbol == "-" || operationSymbol == "*" || operationSymbol == "/" || operationSymbol == "**" || operationSymbol == "V")
			{
				operationList[i] = operationSymbol;
			}
			else
			{
				cout << "Invalid operator!" << endl;
				valid = false;
				break;
			}
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if(valid) break;
	}
	
	Operation** ops = new Operation*[MAX_OPERATORS];
	for(int i = 0; i < numberOfOperations; i++)
	{
		ops[i] = createOperation(operationList[i]);
	}
	Calculator* calc = new Calculator(calculatorName, numberOfOperations, ops);
	while(true)
	{
		cout << "1. List supported operations" << endl;
		cout << "2. List input format" << endl;
		cout << "3. Start calculation" << endl;
		cout << "4. Exit" << endl;
		short chosenOption;
		cin >> chosenOption;
		switch(chosenOption)
		{
		case 1:
			calc->listSupportedOperations();
			break;
		case 2:
			calc->listInputFormat();
			break;
		case 3:
			calc->startCalculation();
			break;
		case 4:
			break;
		default:
			cout << "Invalid option, try again." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		if(chosenOption == 4) break;
	}
	delete[] calculatorName;
	delete calc;
	for (int i = 0; i < numberOfOperations; ++i)
	{
    	delete ops[i];
	}
	delete[] ops;
	return 0;
}
