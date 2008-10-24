/*
---------------------------------------------------------------------------
Open Asset Import Library (ASSIMP)
---------------------------------------------------------------------------

Copyright (c) 2006-2008, ASSIMP Development Team

All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the following 
conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the ASSIMP team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the ASSIMP Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

/** @file Implementation of BaseImporter */

#include "AssimpPCH.h"
#include "BaseImporter.h"


using namespace Assimp;


// ------------------------------------------------------------------------------------------------
// Constructor to be privately used by Importer
BaseImporter::BaseImporter()
{
	// nothing to do here
}

// ------------------------------------------------------------------------------------------------
// Destructor, private as well
BaseImporter::~BaseImporter()
{
	// nothing to do here
}

// ------------------------------------------------------------------------------------------------
// Imports the given file and returns the imported data.
aiScene* BaseImporter::ReadFile( const std::string& pFile, IOSystem* pIOHandler)
{
	// create a scene object to hold the data
	aiScene* scene = new aiScene();

	// dispatch importing
	try
	{
		InternReadFile( pFile, scene, pIOHandler);
	} catch( ImportErrorException* exception)
	{
		// extract error description
		mErrorText = exception->GetErrorText();

		DefaultLogger::get()->error(mErrorText);

		delete exception;

		// and kill the partially imported data
		delete scene;
		scene = NULL;
	}

	// return what we gathered from the import. 
	return scene;
}

// ------------------------------------------------------------------------------------------------
void BaseImporter::SetupProperties(const Importer* pImp)
{
	// the default implementation does nothing
}

// ------------------------------------------------------------------------------------------------
bool BaseImporter::SearchFileHeaderForToken(IOSystem* pIOHandler,
	const std::string&	pFile,
	const char**		tokens, 
	unsigned int		numTokens,
	unsigned int		searchBytes /* = 200 */)
{
	ai_assert(NULL != tokens && 0 != numTokens && NULL != pIOHandler && 0 != searchBytes);

	boost::scoped_ptr<IOStream> pStream (pIOHandler->Open(pFile));
	if (pStream.get() )
	{
		// read 200 characters from the file
		boost::scoped_ptr<char> _buffer (new char[searchBytes]);
		char* buffer = _buffer.get();

		unsigned int read = (unsigned int)pStream->Read(buffer,1,searchBytes);
		if (!read)return false;

		for (unsigned int i = 0; i < read;++i)
			buffer[i] = ::tolower(buffer[i]);

		// It is not a proper handling of unicode files here ...
		// ehm ... but it works in most cases.
		char* cur = buffer,*cur2 = buffer,*end = &buffer[read];
		while (cur != end)
		{
			if (*cur)*cur2++ = *cur;
			++cur;
		}
		*cur2 = '\0';

		for (unsigned int i = 0; i < numTokens;++i)
		{
			ai_assert(NULL != tokens[i]);
			if (::strstr(buffer,tokens[i]))return true;
		}
	}
	return false;
}



