
class Investment
{
public:
	Investment(string name, int price);		//constructor
	string name() const;		//non-virtual since function does the exact same thing for all derived classes
	int purchasePrice() const;	//non-virtual since function does the exact same thing for all derived classes
	virtual bool fungible() const;		//non-pure virtual since only Stock is fungible and the rest are nonfungible
	virtual string description() const = 0;		//pure virtual since each type of investment outputs its own description
	virtual ~Investment();
private:
	string m_name;
	int m_purchasePrice;
};


class Painting : public Investment
{
public:
	Painting(string name, int price);
	virtual string description() const;
	virtual ~Painting();
};


class Stock :public Investment
{
public:
	Stock(string name, int price, string des);
	virtual string description() const;
	virtual bool fungible() const;
	~Stock();
private:
	string m_ticker;
};


class House :public Investment
{
public:
	House(string name, int price);
	virtual string description() const;
	~House();
};



Investment::Investment(string name, int price)
	:m_name(name), m_purchasePrice(price)
{}

string Investment::name() const
{
	return m_name;
}

int Investment::purchasePrice() const
{
	return m_purchasePrice;
}

bool Investment::fungible() const
{
	return false;
}

Investment::~Investment()
{}


Painting::Painting(string name, int price)
	:Investment(name, price)
{}

string Painting::description() const			//not sure if this is correct
{
	return "painting";
}

Painting::~Painting()
{
	//Destroying "name", a painting
	cout << "Destroying " << name() << ", a " << description() << endl;
}

Stock::Stock(string name, int price, string des)
	:Investment(name, price), m_ticker(des)
{}

string Stock::description() const
{
	return "stock trading as "+m_ticker;
}

bool Stock::fungible() const
{
	return true;
}

Stock::~Stock()
{
	//Destroying "name", a stock holding
	cout << "Destroying " << name() << ", a stock holding" << endl;
}

House::House(string name, int price)
	:Investment(name, price)
{}

string House::description() const
{
	return "house";
}

House::~House()
{
	//Destroying the house 4 Privet Drive
	cout << "Destroying the house " << name() << endl;
}

