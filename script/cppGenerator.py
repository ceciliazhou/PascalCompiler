'''
This script is used to generate cpp code which defines/initializes data structures storing productions/terms by parsing a given file containing productions.
USAGE: python cppGenerator.py productionFileName cppFile

'''

##PRODUCTION = 0
import sys

NOT_FOUND = -1

def parseProduction( index, prod, fSet ):
    '''
    return list: [ comment-production-formula, LH-side-NT, [ RH-item1, RH-item2, ... ], [ [FS-item1, prod-NDX], [FS-item2, prod-NDX], ... ] ]
    '''
    prodArray = [prod.strip()]
    temp = prod.strip().split()
    prodArray += [temp[0]]
    prodArray += [temp[2:]]
    
    tempSet = fSet.upper().strip().split()
    firSet = []
    for item in tempSet[ tempSet.index('{')+1: tempSet.index('}') ]:
        firSet.append( [item, index] )

    prodArray += [ firSet ]
    return [prodArray]
    
    
def parseTerminals( line ):
    '''
    return a list of terminals, each terminal is of following format:
	[ terminal_lexeme, terminal_token_type ]
    '''
    terms = line.strip()
    tempLst = terms.split('|')
    
    termLst = []
    for item in tempLst:
            newItem = item.split()
            termLst.append( [newItem[0].upper(), newItem[1]] )

    return termLst

    
def findTerminal( lexeme, termLst ):
    '''
    return the index of terminal if lexeme exists in termLst, otherwise NOT_FOUND returned.
    '''
    for ndx in range( len(termLst) ):
            if lexeme.upper() == termLst[ndx][0].upper():
                    return ndx
                
    return NOT_FOUND


def parseFile( fileName ):
    '''
    '''
    fObj = open( fileName, 'r' )

    terms = []
    count = int(fObj.readline())
    for i in range( count ):
        line = fObj.readline()
        terms += parseTerminals( line )
    #print terms
    prodArray = []
    count = int(fObj.readline())
    lines = fObj.readlines()
    for ndx in range(count):
        prodArray += parseProduction( ndx, lines[ndx], lines[ndx+count] )

    fObj.close()
    return terms, prodArray


def generateCPP( cppFile, terms, prodArray ):
    '''
    1. definition of a enum of non-terminal types
    2. definition of a string array to store the strings for printing non-terminal types 
    3. definition of a struct array to store all the productions
    4. definition of a 2-demension array to store the parse table
    '''
    fObj = open(cppFile, 'w')

    ntTypes = []
    include = "#include <string>\n#include \"scanner.h\"\n\n" 
    enum = "enum NTtype\n{\n\t"
    termStr = "const std::string NTstring[] = \n{\n\t"
    prodItem = "struct prod_item\n{\n\t%-10s%s\n\t%-10s%s\n};\n\n" % ("bool", "isTerm;", "int", "type;")
    prodArr = "const prod_item prod_array[] = \n{\n"
    prodAuxiItem = "struct prod_auxi_item\n{\n\t%-10s%s\n\t%-10s%s\n};\n\n" % ("int", "start;", "int", "len;")
    prodAuxiArray = "const prod_auxi_item prod_auxi_array[] = \n{\n\t"
    prsTbl = "const int parse_table[][%i] = \n{\n\t" % len(terms)
    cntNTtype = 0
    cntProdItems = 0
    lastProd = ""
    firstSet = []

    for n in range( len(prodArray) ):

        prod = prodArray[n]
        ## add the whole production as a comment in the production struct array
        if prodArr[-1:] != '\n':
            prodArr += '\n'
        prodArr += "\n\t/* %s */\n\t" %(prod[0])

        ## define non-terminal enum and non-terminal string array
        newNT, tmStr = processNTtype( prod[1], ntTypes )
        if(newNT != ""):
            enum += newNT
            termStr += tmStr
            cntNTtype += 1
            if( cntNTtype % 5 == 0):
                enum += "\n\t"
                termStr += "\n\t"  
		
	## define production arrays
        length, prodItemsStr = processProd( prod[2], terms )
        prodArr += prodItemsStr
        prodAuxiArray += "%-6s%-6s" % ( "{"+str(cntProdItems)+",", str(length)+"}," )
        cntProdItems += length
        if( (n+1) % 8 == 0 ):
            prodAuxiArray += "\n\t"

        ## define parse table
        ## first production of new nt type
        if(prod[1] != lastProd):
            prsTbl += "{"
            lastProd = prod[1]
            firstSet = []
        firstSet += prod[3]
        ## last production of new nt type

        if(n == len(prodArray)-1 or prod[1] != prodArray[n+1][1]):
            #print prod[1], " --- ", firstSet
            prsTbl += processFirstSet(firstSet, terms)+'},\n\t'
        
    enum = enum.strip(', \n\t') + "\n};\n\n"
    termStr = termStr.strip(', \n\t') + "\n};\n\n"
    prodArr = prodArr.strip(', \n\t') + "\n};\n\n"
    prodAuxiArray = prodAuxiArray.strip(', \n\t') + "\n};\n\n"
    prsTbl = prsTbl.strip(', \n\t') + "\n};\n\n"

    fObj.write(include) 
    fObj.write(enum)
    fObj.write(termStr)
    fObj.write(prodItem)
    fObj.write(prodArr)
    fObj.write(prodAuxiItem)
    fObj.write(prodAuxiArray)
    fObj.write(prsTbl)
    

def processFirstSet(fSet, terms):
    '''
    '''
    #print fSet
    prsTblItem = ""
    for item in terms:
        ndx = findTerminal(item[0], fSet)
        if ndx != NOT_FOUND:
            prsTblItem += "%5s" % ( str(fSet[ndx][1])+',' )
        else:
            prsTblItem += "%5s" % "-1," 

    return prsTblItem.strip(', \n\t')

        
def processNTtype(ntType, ntTypes):
    '''
    '''
    enum = ""
    termStr = ""
    
    if ntType not in ntTypes:
        ntTypes.append( ntType )
        enum = "%-28s" %( ntType + "," )
        termStr = "\"%-28s" %( ntType + "\"," )

    return enum, termStr
    

def processProd(prodItems, terms):
    '''
    '''
    cnt = 0
    prod = ""
    
    for item in prodItems:
        if item != "<nil>":
            ndx = findTerminal( item, terms )
            if ndx != NOT_FOUND:
                prod += "%-8s%-28s" % ("{true,", terms[ndx][1] + "},")
            else:
                prod += "%-8s%-28s" % ("{false,", item + "},")
            cnt += 1

        if(cnt > 0 and cnt%3 == 0):
            prod += "\n\t"

    return cnt, prod 


def main( ):

    if len(sys.argv) < 3:
        raw_input("Usage: cppGenerator.py productionFileName cppFile")  
    else:
        terms, prodArray = parseFile( sys.argv[1] )
        generateCPP( sys.argv[2], terms, prodArray )
        raw_input("Done! Press any key to go away...")
        
        
main()
