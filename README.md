STINGER
=======
[![Build Status](https://travis-ci.org/robmccoll/stinger.png?branch=master)](https://travis-ci.org/robmccoll/stinger)

Learn more at [stingergraph.com](http://stingergraph.com).

Directory Structure
===================

    .
    ├── CMakeLists.txt
    ├── doc
    ├── doxygen
    ├── flask
    ├── html
    ├── README.md
    ├── SOURCEME.sh
    ├── src
    │   ├── bin
    │   │   ├── clients
    │   │   │   ├── algorithms
    │   │   │   │   ├── betweenness
    │   │   │   │   ├── clustering_coefficients
    │   │   │   │   ├── kcore
    │   │   │   │   ├── pagerank
    │   │   │   │   ├── pagerank_updating
    │   │   │   │   ├── rate_monitor
    │   │   │   │   ├── simple_communities
    │   │   │   │   ├── spmspv_test
    │   │   │   │   ├── static_components
    │   │   │   │   ├── test_alg
    │   │   │   │   └── weakly_connected_components
    │   │   │   ├── streams
    │   │   │   │   ├── csv_stream
    │   │   │   │   ├── human_edge_generator
    │   │   │   │   ├── json_stream
    │   │   │   │   ├── mongodb_stream
    │   │   │   │   ├── netflow_stream
    │   │   │   │   ├── random_edge_generator
    │   │   │   │   └── rmat_edge_generator
    │   │   │   └── tools
    │   │   │       ├── alg_to_mongo
    │   │   │       ├── dump_graph_to_disk
    │   │   │       ├── json_rpc_server
    │   │   │       ├── scrape_alg_data
    │   │   │       ├── scrape_vertex_data
    │   │   │       ├── sql_client
    │   │   │       ├── test_client
    │   │   │       ├── udp_query
    │   │   │       ├── vquery
    │   │   │       └── web_server
    │   │   ├── server
    │   │   ├── standalone
    │   │   └── tests
    │   ├── lib
    │   │   ├── compat
    │   │   ├── fmemopen
    │   │   ├── int_hm_seq
    │   │   ├── int_ht_seq
    │   │   ├── intvec
    │   │   ├── kv_store
    │   │   ├── mongo_c_driver
    │   │   ├── mongoose
    │   │   ├── protobuf
    │   │   ├── pugixml
    │   │   ├── rapidjson
    │   │   ├── stinger_core
    │   │   ├── stinger_net
    │   │   ├── stinger_utils
    │   │   ├── string
    │   │   └── vtx_set
    │   ├── py
    │   │   └── stinger
    │   └── templates
    │       └── json


Building
========

STINGER is built using [CMake](http://www.cmake.org).  From the root of STINGER, first create a build directory:

    mkdir build && cd build
    . ../SOURCEME.sh

Then call CMake from that build directory to automatically configure the build and to create a Makefile:

    ccmake .. -DCMAKE_BUILD_TYPE=Release

Change `Release` to `Debug` for a debugging build during development.  Finally, call make to build all libraries and executable targets (or call make and the name of an executable or library to build):

    make

Note:  multithreaded make is discouraged until CMakeLists issues are corrected.

All binary targets will be built and placed in build/bin.  They are named according to the folder from which they were 
built (so src/bin/server produces build/bin/stinger\_server, src/bin/clients/tools/json\_rpc\_server produces 
build/bin/stinger\_json\_rpc\_server, etc.).  If you ran SOURCEME.sh from the build directory as instructed above, the build/bin
directory is appended to your path.

All library targets are built as both static and shared libraries by default and are placed in build/lib as .so and .a files
(or .dylib on Mac).  Headers for these libraries are copied into build/include/library\_name.  The build/include directory
is in the include path of all targets.

Executable Targets
==================

As indicated by the directory structure, there are three primary types of executable targets: clients, server, and standalone, 
and subtypes in the case of clients.

The STINGER server maintains a STINGER graph in memory and can maintain multiple connections with client streams, algorithms, and monitors.

Client streams can send edges consisting of source, destination, weight, time, and type where some fields are optional 
and others can optionally be text strings.

Client algorithms will receive these batches of updates in a 
synchronous manner, as well as shared-memory read-only access to the complete graph.  The server provides the capability
for client algorithms to request a shared memory space to store results and communicate with other algorithms.  
Client algorithms declare dependencies when they connect and receive the mapped data in the returned structure.
The server guarantees that all of an algorithm's dependencies will finish processing before that algorithm is executed.
Clients algorithms are required to provide a description string that indicates what data is stored and the type of the data.  

Client tools are intended to be read-only, but are notified of all running algorithms and shared data.  An example
of this kind of client is the JSON-RPC server (src/bin/clients/tools/json\_rpc\_server).  This server provides access
to shared algorithm data via JSON-RPC calls over HTTP.  Additionally, some primitive operations are provided to support
selecting the top vertices as scored by a particular algorithm or obtaining the shortest paths between two vertices, for example.

Standalone executables are generally self-contained and use the STINGER
libraries for the graph structure and supporting functions.  Most of the existing standalone executables demonstrate
a single streaming or static algorithm on a synthetic R-MAT graph and edge stream.

Running
=======

Using the Server
----------------
To run an example using the server and five terminals:

    term1:build$ env STINGER_MAX_MEMSIZE=1G ./bin/stinger_server
    term2:build$ ./bin/stinger_json_rpc_server
    term3:build$ ./bin/stinger_static_components
    term4:build$ ./bin/stinger_pagerank
    term5:build$ ./bin/stinger_rmat_edge_generator -n 100000 -x 10000

This will start a stream of R-MAT edges over 100,000 vertices in batches of 10,000 edges.  A connected component labeling
and PageRank scoring will be maintained.  The JSON-RPC server will host interactive web pages at 
http://localhost:8088/full.html are powered by the live streaming analysis.  The total memory usage of the dynamic graph is limited to 1 GiB.

Example: Parsing Twitter
------------------------

Given a stream of Tweets in Twitter's default format (a stream of JSON objects, one per line), it is fairly easy to pipe
the user mentions / retweets graph into STINGER using the json\_stream.  The json\_stream is a templated JSON stream parser
designed to consume one object per line like the Twitter stream and to produce edges from this stream based on a template.

The templates can use the following variables (where one of the two source and one of the two destination variables 
must be used):

    $source_str         - The source vertex name.
    $source             - The source of the edge as a number (must be able to parse as an integer 
                          less than the maximum vertex ID in the STINGER server).
    $source_type        - A string representing the type of the source vertex.
    $source_weight      - A number to be added to the weight of the source vertex (vertex weights
                          start at zero).
    $destination_str    - The destination vertex name
    $destination        - The destination of the edge as a number (must be able to parse as an 
                          integer less than the maximum vertex ID in the STINGER server).
    $destination_type   - A string representing the type of the destination vertex
    $destination_weight - A number to be added to the weight of the destination vertex (vertex 
                          weights start at zero).
    $type_str           - The edge type as a string
    $weight             - The weight of the edge (must be able to parse as an integer).
    $time               - The time of the edge (must be able to parse as an integer).
    $time_ttr           - Must be a string of either the form "Mon Sep 24 03:35:21 +0000 2012" or 
                          "Sun, 28 Oct 2012 17:32:08 +0000".  These will be converted internally
                          into integers of the form YYYYMMDDHHMMSS.  Note that this does not currently support
                          setting a constant value.

For example, the simplest template for Twitter mentions and retweets would be (we'll call this template.json):

    {
      "user": {
        "screen_name": "$source_str1"
      },
      "entities": {
        "user_mentions": [
          {
            "screen_name": "$destination_str1"
          }
        ]
      },
      "this_doesnt_matter": "$source_type=user",
      "same_here": "$destination_type=user",
      "and_here": "$type=mention"
    }

To parse a Twitter stream into STINGER using this template:

    cat twitter_sample.json | ./bin/stinger_json_stream template.json

You can replace the 'cat twitter\_sample.json' command with one of the curl commands from the Twitter developer
API page to directly inject a live Twitter stream (obviously you should go to dev.twitter.com to get your 
own OAuth data):

    curl --request 'POST' 'https://stream.twitter.com/1.1/statuses/sample.json' --header 
    'Authorization: OAuth oauth_consumer_key="KEYKEYKEY", oauth_nonce="NONCENONCENONCE", 
    oauth_signature="SIGSIGSIG", oauth_signature_method="HMAC-SHA1", oauth_timestamp="ts", 
    oauth_token="TOKENTOKENTOKEN", oauth_version="1.0"' --verbose | ./bin/json_stream template.json

Example: Parsing CSV Files / Streams
---------------------------

The csv\_stream parser follows a similar templated format to the json parser, so parsing edges out of a file might look like:

    id,email_a,config_a,email_b,config_b,unix_time,length
    na,$source_str1,na,$destination_str1,na,$time1,$weight1, $source_type1=email, $destination_type1=email

This file would create edges between email addresses using the length field as the weight and the Unix timestamp field as the time.  To use this template, pipe the file or stream into the parser and pass the template as a parameter like so:

    cat emails.csv | ./bin/stinger_csv_stream template.csv

Please be aware that the CSV parser and the underlying code to parse CSV files does not currently trim whitespace, and does not treat quoted strings of any kind as quoted.

Using a Standalone Client
-------------------------
To create a toy R-MAT graph (256K vertices and 2M undirected edges) and run the insert-remove benchmark:

    term1:build$ stinger_rmat_graph_generator -s 18 -e 8 -n 100000
    term1:build$ stinger_insert_remove_benchmark -n 1 -b 100000 g.18.8.bin a.18.8.100000.bin
    
    
[![githalytics.com alpha](https://cruel-carlota.pagodabox.com/a7d82e7aa670122314238336dbbd7c89 "githalytics.com")](http://githalytics.com/robmccoll/stinger)

Handling Common Errors
======================

Runtime Issues
--------------

STINGER allocates and manages its own memory. When STINGER starts, it allocates one large block of memory (enough to hold its maximum size), and then manages its own memory allocation from that pool.  The server version of STINGER does this in shared memory so that multiple processes can see the graph.  Unfortunately, the error handling for memory allocations is not particularly user-friendly at the moment.  Changing the way that this works is on the issues list (see https://github.com/robmccoll/stinger/issues/8).

- "Bus error" when running the server: The size of STINGER that the server is trying to allocate is too large for your memory.  Reduce the size of your STINGER and recompile.
- "XXX: eb pool exhausted" or "STINGER has run out of internal storage space" when running the server, standalone executables, or anything else using stinger\_core: you have run out of internal edge storage.  Increase the size of STINGER and recompile.

Compile-time definitions can be adjusted in ccmake to increase or decrease memory consumption.  Note that adjusting these values will require you to rebuild STINGER.

- STINGER\_MAX\_LVERTICES: maximum number of vertices to support, often written as a power of two: 1L<<22
- STINGER\_EDGEBLOCKSIZE: number of edges per block, default: 14  
- STINGER\_NUMETYPES: maximum number of edge types to support, default: 5
- STINGER\_NUMVTYPES maximum number of vertex types to support, default: 128

STINGER\_EDGEBLOCKSIZE determines how many edges are in each edge block (there are 4 * STINGER\_MAX\_LVERTICES edge blocks, so 4 * STINGER\_MAX\_LVERTICES * STINGER\_EDGEBLOCKSIZE is the maximum number of directed edges).

These are listed in the order of how much of an affect they will have on the size of STINGER.

Build Problems
--------------

First check the STINGER GitHub page to verify that the build is passing (icon immediately under the title).  If it is not passing, the issue resides within the current version itself.  Please checkout a previous revision - the build should be fixed shortly as failing builds sent notifications to the authors.  

Build problems after pulling updates are frequently the result of changes to the Protocol Buffer formats used in STINGER.  These are currently unavoidable as an unfortunate side effect of how we distribute PB and tie it into CMake.  To fix, remove your build directory and build STINGER from scratch.

Additionally, this version of the STINGER tool suite is tested almost exclusively on Linux machines running later version of Ubuntu and Fedora.  While we  would like to have multi-platform compatibility with Mac (via "real" GCC) and Windows (via GCC on cygwin), these are lower priority for our team - unless a project sponsor requires it :-)  

