## About

This is a parser for a subset of the Pascal programming language. See ./Grammar for details of supported grammer.


## Usage

1. compile the source files in ./src:
    >g++ *.cpp -o pascal_parser
    
2. run pascalCompiler with a pascal source file as input to see the output parse tree:
    >pascal_parser source.pas
    
    An sample set of [input](https://raw.github.com/ceciliazhou/pascal_parser/master/test/sampleCode.pas) and 
[output](https://raw.github.com/ceciliazhou/pascal_parser/master/test/sampleOutput) is available under ./test.
