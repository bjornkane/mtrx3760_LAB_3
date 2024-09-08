// This is a demonstration of using linux piping to read from files.
//
// To run it, first copy the following text into an appropriately named file, e.g. testCircuit.txt, being sure to
// remove the comment markers // and leading whitespace on each line:
//
//  # A 3-gate test circuit
//
//  component xor myXor0
//  component and myAnd0
//  component and myAnd1
//
//  wire myXor0 0 inwireA
//  wire myXor0 1 inwireB
//  wire myAnd0 1 inwireC
//
//  connect myXor0 myAnd0 0
//  connect inWireA myAnd1 0
//  connect inWireB myAnd1 0
//
//  testerOutput myAnd0 0
//  testerOutput myAnd1 0
//  testerInput inWireA
//  testerInput inWireB
//  testerInput inWireC
//
//  end
//
// Compile this program, then run it with a command line of the form
//
// a.out < testCircuit.txt
//
// to pipe the contents of testCircuit.txt into the program.
//
// Copyright (c) Donald Dansereau, 2023


#include <iostream>
#include <string>




int main()
{
 
  while( true )
  {
    std::string Request;
    std::cin >> Request;  // get the next word from the input stream
    std::cout << "Processing input token: " << Request << std::endl;
    
    if( Request[0] == '#' )
    {
      // a comment line
      // get the rest of the line and ignore it
      std::string DummyVar;
      getline( std::cin, DummyVar );
    }
    else if( Request.compare( "component" ) == 0 )
    {
      std::string GateType;
      std::string GateName;
      std::cin >> GateType;
      std::cin >> GateName;
      std::cout << "Adding gate of type " << GateType << " named " << GateName << std::endl;
      // TODO : add the gate
    }
    else if( Request.compare( "wire" ) == 0 )
    {
    
      // TODO: implement wire command
    
    }
    
    // TODO: add whatever other commands you need
    
    else if( Request.compare( "end" ) == 0 )
    {
      break; // end of file
    }
    else
    {
      std::cout << "Unrecognised command " << Request << std::endl;
      std::cout << "Continuing to next line" << std::endl;
      // get the rest of the line and ignore it
      std::string DummyVar;
      getline( std::cin, DummyVar );
    }
  }
  
  return 0;
}
