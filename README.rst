################################################
Test kernel memory lock capability on Qserv node
################################################

***************
Build container
***************

On a Linux workstation run:

.. code-block:: bash

   git clone https://github.com/lsst/qserv_testmemlock.git
   cd qserv_testmemlock
   docker build container -t qserv/test_memlock
   docker push qserv/test_memlock

*****************************
Test memlock on Qserv cluster
*****************************

On target node, run:

.. code-block:: bash

   docker pull qserv/test_memlock
   docker run --rm --name memlock --volume=/mnt/qserv:/qserv qserv/test_memlock

Output should be:

.. code-block::

   Checking memory lock feature for /qserv/data/mysql/LSST/Object_0.frm
   memory_lock: 20782 bytes (20.2K) locked
   memory_lock: kernel interface (/proc/10/status) returns: VmLck:       24 kB
   Checking memory lock feature for /qserv/data/mysql/LSST/Source_0.MYD
   memory_lock: 8920917625 bytes (8.3G) locked
   memory_lock: kernel interface (/proc/11/status) returns: VmLck:  8711836 kB


************************************
Test memlock/mmap between containers
************************************

Pre-requisite: create two large files (~10GB), see `qserv_testmemlock/container/src/create_large_files.cc` for example.

.. code-block:: bash

   docker pull qserv/test_memlock
   # Put a large file in shared memory and lock it in RAM
   # NOTE: add --volume=$PWD:/home/qserv/qserv_testmemlock for development purpose 
   docker run -it --rm --ulimit memlock=12884901888 --name memlock --volume=/tmp/qserv:/qserv qserv/test_memlock -f -l /qserv/big-file-1.txt
   docker run -it --rm --name memmap --volume=/tmp/qserv:/qserv --entrypoint /qserv_testmlk/memory_map qserv/test_memlock /qserv/big-file-1.txt
   # check
   docker exec -it -- memlock lsof -ad mem,txt /qserv/big-file.data
   docker exec -it -- memmap lsof -ad mem,txt /qserv/big-file.data

************************************************
Test memlock/mmap versus read between containers
************************************************

.. code-block:: bash

   docker pull qserv/test_memlock
   # Put a large file in shared memory and lock it in RAM
   docker run -it --rm --ulimit memlock=12884901888 --name memlock --volume=/tmp/qserv:/qserv qserv/test_memlock -f -l /qserv/big-file-1.txt
   # In a second terminal
   iotop -aoP
   # In a third terminal
   # free memory cache
   sudo sh -c 'free && sync && echo 3 > /proc/sys/vm/drop_caches && free'
   docker run -it --rm --volume=/tmp/qserv:/qserv -- debian sh -c "od -c /qserv/big-file-2.txt"
   # In a fourth terminal
   docker run -it --rm --volume=/tmp/qserv:/qserv -- debian sh -c "od -c /qserv/big-file-2.txt"

   # iostat does not report I/O here
   od -c /qserv/big-file-1.txt
   # iostat reports I/O here
   od -c /qserv/big-file-2.txt

