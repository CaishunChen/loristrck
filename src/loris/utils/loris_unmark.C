/*
 * This is the Loris C++ Class Library, implementing analysis, 
 * manipulation, and synthesis of digitized sounds using the Reassigned 
 * Bandwidth-Enhanced Additive Sound Model.
 *
 * Loris is Copyright (c) 1999-2016 by Kelly Fitz and Lippold Haken
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * loris_unmark.C
 *
 * main() function for a utility program to remove a
 * marker or markers from a AIFF, SPC, or SDIF file.
 *
 * Kelly Fitz, 20 Dec 2004
 * loris@cerlsoundgroup.org
 *
 * http://www.cerlsoundgroup.org/Loris/
 *
 */
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "AiffFile.h"
#include "Exception.h"
#include "Marker.h"
#include "SdifFile.h"
#include "SpcFile.h"

using std::cout;
using std::string;
using std::vector;
using namespace Loris;

static void findAndRemoveAll( std::vector< Marker > & markers,
							  const string & id )
{
	std::vector< Marker >::iterator it = markers.begin();
	while( it != markers.end() )
	{
		if ( it->name() == id )
		{
			it = markers.erase( it );
		}
		else
		{
			++it;
		}	
	}
}

int main( int argc, char* argv[] )
{
	//	check for all arguments
	if ( argc != 3 )
	{
		cout << "usage:\t" << argv[0] << " filename marker_name\n\n";
		return 1;
	}	
	
	//	get the filename and its suffix
	string filename( argv[1] );
	string suffix = filename.substr( filename.rfind('.')+1 );
	
	//cout << "filename is " << filename << "\n";
	//cout << "suffix is " << suffix << "\n";
	
	/*
	char * endptr;
	double time = strtod( argv[2], &endptr );
	if ( argv[2] == endptr )
	{
		cout << "Bad marker time: " << argv[2] << endl;
		cout << "usage:\t" << argv[0] << " filename marker_time marker_name\n\n";
		return 1;
	}
	*/
	string name( argv[2] );
	
	cout << "Removing Marker(s) named \"" << name << endl;
	cout << "from file \"" << filename << "\"" << endl;
	
	std::vector< Marker > markers;
	if ( suffix == "aiff" || suffix == "aif" )
	{
		try
		{
			AiffFile f( filename );
			findAndRemoveAll( f.markers(), name );
			markers = f.markers();
			f.write( filename );
		}
		catch( Exception & ex )
		{
			cout << "Error adding marker to AIFF file: " << filename << "\n";
			cout << ex.what() << "\n";
			return 1;
		}
	}
	else if ( suffix == "sdif" )
	{
		try
		{
			SdifFile f( filename );
			findAndRemoveAll( f.markers(), name );
			markers = f.markers();
			f.write( filename );
		}
		catch( Exception & ex )
		{
			cout << "Error adding marker to SDIF file: " << filename << "\n";
			cout << ex.what() << "\n";
			return 1;
		}
	}
	else if ( suffix == "spc" )
	{
		try
		{
			SpcFile f( filename );
			findAndRemoveAll( f.markers(), name );
			markers = f.markers();
			f.write( filename );
		}
		catch( Exception & ex )
		{
			cout << "Error adding marker to Spectrum file: " << filename << "\n";
			cout << ex.what() << "\n";
			return 1;
		}
	}
	else
	{
		cout << "Error -- unrecognized suffix: " << suffix << "\n";
		return 1;
	}
	
	if ( markers.empty() )
	{
		cout << "No more markers in " << filename << "\n";
	}
	else
	{
		//	print out the markers:
		cout << "Markers (time\tname)" << endl;
		std::vector< Marker >::iterator it;
		for ( it = markers.begin(); it != markers.end(); ++it )
		{
			cout << it->time() << "\t\"" << it->name() << "\"\n";
		}
	}
	
	return 0;
}

