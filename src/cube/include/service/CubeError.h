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


/**
 * \file CubeError.h
 * \brief Provides a hierarchy of errors compatible with C++ thwrow/catch mechanism
 */
#ifndef CUBE_ERROR_H
#define CUBE_ERROR_H

/*
   *----------------------------------------------------------------------------
 *
 * class Error
 *
 *******************************----------------------------------------------------------------------------
 */
#include <sstream>
#include <string>
#include <iostream>
#include <exception>

namespace cube
{
/**
 * General error: gets a message.
 */
class Error : public std::exception
{
public:

    Error( std::string _message ) : message( _message )
    {
    }
    virtual
    std::string
    get_msg() const
    {
        return "Error: " + message;
    }
    ~Error() throw( )
    {
    }


protected:

    std::string message;
};
/**
 * General error: gets a message.
 */
class RuntimeError :  public Error
{
public:

    RuntimeError( std::string _message ) : Error( _message )
    {
        message = "Runtime Error: " + _message;
    }
};
/**
 * General error: gets a fatal message.
 */
class FatalError : public Error
{
public:

    FatalError( std::string _message ) : Error( _message )
    {
        message = "Fatal Error: " + _message;
    }
};

/**
 * General error: gets a message.
 */
class AnchorSyntaxError :  public RuntimeError
{
public:

    AnchorSyntaxError( std::string _message ) : RuntimeError( _message )
    {
        message = "Syntax Error: " + _message;
    }
};



/**
 * Not supported version of cube error
 */
class NotSupportedVersionError : public RuntimeError
{
public:

    NotSupportedVersionError() : RuntimeError( "" )                           /*  the type of error indicated everything. Used CubeData.cpp*/
    {
    }
    NotSupportedVersionError( std::string _message ) : RuntimeError( "Version of cube syntax " + _message +  " is not supported.  Please try newer version of Cube.\n" ) /*  the type of error indicated everything. Used CubeData.cpp*/
    {
    }
};

/**
 * Special error: missing value in the data stream. .
 */
class MissingValueError : public RuntimeError
{
public:

    MissingValueError() : RuntimeError( "" )                           /*  the type of error indicated everything. Used CubeData.cpp*/
    {
    }
    MissingValueError( std::string _message ) : RuntimeError( _message ) /*  the type of error indicated everything. Used CubeData.cpp*/
    {
    }
    virtual std::string
    get_msg() const
    {
        return message;
    }
};


/**
 * Special error: Try to save the value outside of the available buffer in the data stream. .
 */
class WrongMarkerInFileError : public RuntimeError
{
public:

    WrongMarkerInFileError() : RuntimeError( "" )                           /*  the type of error indicated everything. Used CubeData.cpp*/
    {
    }
    WrongMarkerInFileError( std::string _message ) : RuntimeError( _message ) /*  the type of error indicated everything. Used CubeData.cpp*/
    {
    }
    virtual std::string
    get_msg() const
    {
        return message;
    }
};




class PlaceOutOfBufferError : public RuntimeError
{
protected:
    int size;
    int index;
public:


    PlaceOutOfBufferError( int _size,
                           int _index ) : RuntimeError( "" )            /*  the type of error indicated everything. Used CubeData.cpp*/
    {
        size  = _size;
        index = _index;
    }

    virtual std::string
    get_msg() const
    {
        std::stringstream sstr;
        std::string       str1, str2;
        sstr << size;
        sstr >> str1;
        sstr <<  index;
        sstr >> str2;
        return "Write data outside (" + str2 + ") of the buffer of size " + str1;
    }
};




class UnknownResourceError : public RuntimeError
{
public:
    UnknownResourceError( std::string _message ) : RuntimeError( _message )
    {
        message = "Unknown resource: " + _message;
    };
};


class IOError : public RuntimeError
{
public:
    IOError( std::string _message ) : RuntimeError( _message )
    {
        message = "IO Error " + _message;
    };
};



class NoFileError : public IOError
{
public:
    NoFileError( std::string _message ) : IOError( _message )
    {
        message = "Missing file " + _message;
    };
};



class NoFileInTarError : public NoFileError
{
public:
    NoFileInTarError( std::string _message ) : NoFileError( _message )
    {
        message = "Missing file in TAR file : " + _message;
    };
};




class NoIndexFileError : public NoFileError
{
public:
    NoIndexFileError( std::string _message ) : NoFileError( _message )
    {
        message = " Error with index file : " + _message;
    };
};

/**
 * finalization (gathering files into tar archive) error
 */
class FinalizeFilesError : public RuntimeError
{
public:

    FinalizeFilesError( std::string _message ) : RuntimeError( _message )
    {
        message = "CUBE Finalization error: " + _message;
    }
};



class CreateFileError : public IOError
{
public:
    CreateFileError( std::string _message ) : IOError( _message )
    {
        message = " Error during file creation: " + _message;
    };
};




class OpenFileError : public IOError
{
public:
    OpenFileError( std::string _message ) : IOError( _message )
    {
        message = " Error during file opening: " + _message;
    };
};



class ReadFileError : public IOError
{
public:
    ReadFileError( std::string _message ) : IOError( _message )
    {
        message = " Error during file reading: " + _message;
    };
};




class WriteFileError : public IOError
{
public:
    WriteFileError( std::string _message ) : IOError( _message )
    {
        message = " Error during file writing: " + _message;
    };
};




class NotAllocatedMemoryForRow : public RuntimeError
{
public:
    NotAllocatedMemoryForRow( std::string _message ) : RuntimeError( _message )
    {
        message = "Error with memory: " + _message;
    };
};




/**
 * No left space error
 */
class NoLeftSpaceError : public RuntimeError
{
public:

    NoLeftSpaceError( std::string _message ) : RuntimeError( _message )
    {
        message = "CUBE writing data error: " + _message;
    }
};




class ZLibError : public RuntimeError
{
public:
    ZLibError() : RuntimeError( "" )
    {
        message = "ZLib compression error.";
    };
};



class ZNotEnoughMemoryError : public ZLibError
{
public:
    ZNotEnoughMemoryError() : ZLibError()
    {
        message = "Not enough memory to uncompress.";
    };
};




class ZNotEnoughBufferError : public ZLibError
{
public:
    ZNotEnoughBufferError() : ZLibError()
    {
        message = "Not enough room in the output buffer to uncompress.";
    };
};




class ZDataCorruptError : public ZLibError
{
public:
    ZDataCorruptError() : ZLibError()
    {
        message = "Compressed data is corrupt or incomplete.";
    };
};


/**
 * RowsSupplier selection error
 */
class CannotSelectRowSupplierError : public RuntimeError
{
public:

    CannotSelectRowSupplierError( std::string _message ) : RuntimeError( _message )
    {
        message = "Row supplier selection error: " + _message;
    }
};


// compilation errors -------------------
class CubePLError : public RuntimeError
{
public:
    CubePLError( std::string _message ) : RuntimeError( _message )
    {
        message = "CubePL Error: " + _message;
    };
};

class CubePLCompilationError : public CubePLError
{
public:
    CubePLCompilationError( std::string _message ) : CubePLError( _message )
    {
        message = "CubePL Compilation Error: " + _message;
    };
};



class CubePLMemoryLayoutError : public CubePLError
{
public:
    CubePLMemoryLayoutError( std::string _message ) : CubePLError( _message )
    {
        message = " CubePL Mempory Management Error: " + _message;
    };
};


class CubePLUnsupportedVersionError : public CubePLError
{
public:
    CubePLUnsupportedVersionError( std::string _message ) : CubePLError( _message )
    {
        message = " Version " + _message + " of CubePL engine  is not supported by this version of Cube. Please try newer version of Cube";
    };
};






class CubeClusteringLayoutError : public RuntimeError
{
public:
    CubeClusteringLayoutError( std::string _message ) : RuntimeError( _message )
    {
        message = "Cube clustering error " + _message;
    };
};



/**
 * Signalizes that the current system tree cannot be represented in cube3 format
 */
class Cube3SystemTreeMismatchError : public RuntimeError
{
public:

    Cube3SystemTreeMismatchError( std::string _message ) : RuntimeError( _message )
    {
        message = "System Tree in incompatible with cube3 model : " + _message;
    }
};





std::ostream&
operator<<( std::ostream& out,
            Error&        exception );                                           // /< Prints out a message about an error.
std::ostream&
operator<<( std::ostream& out,
            RuntimeError& exception );                                           // /< Prints out a message about a runtime error.
std::ostream&
operator<<( std::ostream& out,
            FatalError&   exception );                                           // /< Prints out a message about a fatal error.
}

#endif
