//
// Copyright (C) 2013-2016 University of Amsterdam
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "columns.h"

#include <boost/foreach.hpp>

#include "sharedmemory.h"

using namespace std;
using boost::interprocess::offset_ptr;

Columns::Columns(boost::interprocess::managed_shared_memory *mem) :
	_columnStore(mem->get_segment_manager())
{
	_mem = mem;
}

Column &Columns::get(string name)
{	
	BOOST_FOREACH(Column &column, *this)
	{
		if (column.name() == name)
			return column;
	}

	throw exception();
}

void Columns::setRowCount(int rowCount)
{
	BOOST_FOREACH(Column &column, *this)
		column.setRowCount(rowCount);
}

void Columns::setColumnCount(int columnCount)
{
	_columnStore.reserve(columnCount);
	for (int i = _columnStore.size(); i < columnCount; i++)
		_columnStore.push_back(Column(_mem));
}

Column &Columns::at(int index)
{
	return _columnStore.at(index);
}

Columns::iterator Columns::begin()
{
	return _columnStore.begin();
}

Columns::iterator Columns::end()
{
	return _columnStore.end();
}

void Columns::setSharedMemory(boost::interprocess::managed_shared_memory *mem)
{
	_mem = mem;

	BOOST_FOREACH(Column &column, *this)
		column.setSharedMemory(mem);
}

