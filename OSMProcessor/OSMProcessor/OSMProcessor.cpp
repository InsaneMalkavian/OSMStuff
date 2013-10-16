// OSMProcessor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <cstring>
#include <string>
#include <vector>

class OSMXMLTag {
public:
    std::string key;
    std::string value;
    std::vector<std::string> values;
    void Clean() {key.clear(); value.clear(); values.clear();};
};

class OSMXMLNode {
public:
    long long id;
    std::vector<OSMXMLTag> tags;
    double lat;
    double lon;
    void Clean() {id = -1; lat = lon = 999; tags.clear();};
};

int _tmain(int argc, _TCHAR* argv[])
{
    FILE * pFile;
    FILE * pFileOut;
    long lSize;
    //char * buffer;
    size_t result;
    std::vector<OSMXMLNode> nodes;
    std::vector<std::string> tags;
    OSMXMLNode readnode;
    //OSMXMLTag readtag;
    static const char str_node[] = "node";
    static const char str_end[] = "/>";
    static const char str_amenity[] = "amenity";
    static const char str_amenity_fuel[] = "fuel";
    printf("Commandline arguments:\n");
    for (int i = 0; i<argc; i++) {
        printf("%d: %ws\n", i, argv[i]);
    }

  char path[512];
  for (int i = 1; i<argc; i++) {
      nodes.clear();
      tags.clear();
      sprintf(path, "%ws.osm", argv[i]);
      pFile = fopen ( path , "r" );
      sprintf(path, "%ws.xml", argv[i]);
      if (pFile==NULL) {fputs ("File error",stderr);}
      else {
          pFileOut = fopen (path, "w");
          printf("Both files are opened\n");
          // obtain file size:
          fseek (pFile , 0 , SEEK_END);
          lSize = ftell (pFile);
          rewind (pFile);

          char buffer [512];
          char buffer_root [512];
          long count = 0;
          fputs ("<osm>\n", pFileOut);
          while ( ! feof (pFile) ) {
            if ( fgets (buffer , 512 , pFile) == NULL ) break;
            if (strstr(buffer, str_node) && strstr(buffer, str_end)==NULL) { // node stores tags, look for it
                strcpy(buffer_root, buffer);
                readnode.Clean();
                tags.clear();
                int bingo = 0;
                do
                {
                    if ( fgets (buffer , 512 , pFile) == NULL ) break;
                    tags.push_back(buffer);
                    if (strstr(buffer, str_amenity) && strstr(buffer, str_amenity_fuel)) {                
                        bingo = 1;
                        count++;
                    }
                }
                while (!strstr(buffer, str_node));
                if (bingo == 1) {
                    fputs (buffer_root, pFileOut);
                    for (int i = 0; i<tags.size(); i++) {
                        //fputs (tags.at(i).c_str() , stdout);
                        fputs (tags.at(i).c_str() , pFileOut);                
                    }
                }
            }
          }
          fputs ("</osm>", pFileOut);
          printf("Total count: %ld\n", count);
          //fprintf(pFileOut, "Total count: %ld", count);
          fclose (pFile);
          fclose (pFileOut);
      }
  }
  // allocate memory to contain the whole file:
  /*buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

  /* the whole file is now loaded in the memory buffer. */

  // terminate
  //fclose (pFile);
  //free (buffer);
    return 0;
}

