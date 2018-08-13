#include "Paint.h"

Paint::Paint(const Paint &pt):text(pt.text), start(pt.start), end(pt.end)
{
}

Paint& Paint::operator=(const Paint &pt)
{

	text = pt.text;
	start = pt.start;
	end = pt.end;

	return *this;
}

string Paint::paint(const string &msg)
{
	string str(start+msg+end);
	return str;
}

bool Paint::operator==(const Paint &pt)
{
	if(text != pt.text)
		return false;

	if(start != pt.start)
		return false;

	if(end != pt.end)
		return false;

	return true;
}

bool Paint::operator!=(const Paint &pt)
{
	return !(*this == pt);
}
