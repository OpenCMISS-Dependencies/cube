/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2016                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  Copyright (c) 2009-2015                                                **
**  German Research School for Simulation Sciences GmbH,                   **
**  Laboratory for Parallel Programming                                    **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/



#ifndef __CUBE_INFO_CALLS_H
#define __CUBE_INFO_CALLS_H



AggrCube*
openCubeFile( const char* filename )
{
    if ( check_file( filename ) != 0 )
    {
        throw Error( string( "File " ) + filename + " failed check!" );
    }

/*
   #ifndef CUBE_COMPRESSED
   ifstream in(filename);
   #else
   gzifstream in(filename, ios_base::in|ios_base::binary);
 #endif*/

    AggrCube* input = new AggrCube();
    input->openCubeReport( filename );


/*  if (!in)
    throw Error(string("Could not create stream for file ") + filename + "!");

   in >> *input;*/
    return input;
}


#endif
