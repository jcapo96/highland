"""
"""
import os
import re
import sys
import ROOT as rt
from array import array
from collections import namedtuple, defaultdict

__author__ = 'Anselmo Cervera'
__email__ = 'acervera [at] ific.uv.es'

#######################################################
def process(versionName, fileName):
    
    """  comments
    """

    if os.path.exists(versionName):
        print "Folder ", versionName, " already exists. Please choose a different name "
        return
        
    print "Opening file ", fileName
    f = rt.TFile(fileName);

    print "MakeProject(\"LArSoftReader\",\"*\",\"nocompilation+\") ..."
    f.MakeProject("LArSoftReader","*","nocompilation+")

    print "mv LArSoftReader ", versionName
    os.rename("LArSoftReader",versionName)

    print "rm -f MAKEP"
    os.remove(os.path.join(versionName,"MAKEP"))

#######################################################        
def usage():
    print "USAGE: python createVersion.py <version_name> <root_file>"


#######################################################
if __name__ == '__main__':

    if (len(sys.argv) !=3):
        usage()
        sys.exit(0)
    
    process(sys.argv[1],sys.argv[2])

