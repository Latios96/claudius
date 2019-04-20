#include <maya/MSimple.h>
#include <maya/MIOStream.h>

DeclareSimpleCommand( helloWorld, "Jan Honsbrok", "2018");

MStatus helloWorld::doIt( const MArgList& )
{
  cout << "Hello World\n" << endl;
  return MS::kSuccess;
}