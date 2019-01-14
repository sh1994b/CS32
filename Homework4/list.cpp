void listAll(const Class* c, string path)  // two-parameter overload
{
	path += c->name();
	cout << path << endl;
	for (unsigned int i = 0; i < (c->subclasses()).size(); i++)
	{
		listAll((c->subclasses())[i], path + "=>");
	}

}